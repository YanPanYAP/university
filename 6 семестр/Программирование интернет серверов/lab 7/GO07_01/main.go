package main

import (
	"bytes"
	"encoding/json"
	"errors"
	"log"
	"math"
	"net/http"
	"strconv"
	"sync"

	"github.com/gorilla/mux"
)

type rpcRequest struct {
	JSONRPC string          `json:"jsonrpc"`
	Method  string          `json:"method"`
	Params  json.RawMessage `json:"params,omitempty"`
	ID      json.RawMessage `json:"id,omitempty"`
}

type rpcResponse struct {
	JSONRPC string          `json:"jsonrpc"`
	Result  json.RawMessage `json:"result,omitempty"`
	ID      json.RawMessage `json:"id,omitempty"`
}

type rpcErrorObject struct {
	Code    int    `json:"code"`
	Message string `json:"message"`
}

type rpcErrorResponse struct {
	JSONRPC string          `json:"jsonrpc"`
	Error   *rpcErrorObject `json:"error"`
	ID      json.RawMessage `json:"id"`
}

type pairParams struct {
	X float64 `json:"x"`
	Y float64 `json:"y"`
}

type precisionParams struct {
	N int `json:"N"`
}

type RPCServer struct {
	mu        sync.RWMutex
	precision int
}

func NewRPCServer() *RPCServer {
	return &RPCServer{precision: 2}
}

func NewRouter(server *RPCServer) http.Handler {
	router := mux.NewRouter()
	router.HandleFunc("/rpc", server.handleRPC).Methods(http.MethodPost)
	return router
}

func (s *RPCServer) handleRPC(w http.ResponseWriter, r *http.Request) {
	log.Printf("request %s %s", r.Method, r.URL.Path)

	body, err := readBody(r)
	if err != nil {
		writeError(w, rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32700, Message: "parse error"},
			ID:      json.RawMessage("null"),
		})
		return
	}

	trimmed := bytes.TrimSpace(body)
	if len(trimmed) == 0 {
		writeError(w, rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32700, Message: "parse error"},
			ID:      json.RawMessage("null"),
		})
		return
	}

	w.Header().Set("Content-Type", "application/json")

	if trimmed[0] == '[' {
		s.handleBatch(w, trimmed)
		return
	}

	response, isNotification := s.execute(trimmed)
	if isNotification {
		w.WriteHeader(http.StatusNoContent)
		return
	}

	writeJSON(w, http.StatusOK, response)
}

func (s *RPCServer) handleBatch(w http.ResponseWriter, body []byte) {
	var requests []json.RawMessage
	if err := json.Unmarshal(body, &requests); err != nil {
		writeError(w, rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32700, Message: "parse error"},
			ID:      json.RawMessage("null"),
		})
		return
	}

	if len(requests) == 0 {
		writeError(w, rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32600, Message: "invalid request"},
			ID:      json.RawMessage("null"),
		})
		return
	}

	log.Printf("batch request count=%d", len(requests))

	responses := make([]any, 0, len(requests))
	for _, item := range requests {
		response, isNotification := s.execute(item)
		if isNotification {
			continue
		}
		responses = append(responses, response)
	}

	if len(responses) == 0 {
		w.WriteHeader(http.StatusNoContent)
		return
	}

	writeJSON(w, http.StatusOK, responses)
}

func (s *RPCServer) execute(raw []byte) (any, bool) {
	var req rpcRequest
	if err := json.Unmarshal(raw, &req); err != nil {
		log.Printf("invalid request: cannot decode json")
		return rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32600, Message: "invalid request"},
			ID:      json.RawMessage("null"),
		}, false
	}

	if req.JSONRPC != "2.0" || req.Method == "" {
		log.Printf("invalid request: jsonrpc=%q method=%q", req.JSONRPC, req.Method)
		return rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32600, Message: "invalid request"},
			ID:      normalizeID(req.ID),
		}, false
	}

	isNotification := len(bytes.TrimSpace(req.ID)) == 0
	if isNotification {
		log.Printf("method=%s type=notification", req.Method)
	} else {
		log.Printf("method=%s type=request id=%s", req.Method, string(normalizeID(req.ID)))
	}

	switch req.Method {
	case "sum":
		return s.makeMathResponse(req, func(x, y float64) float64 { return x + y }), false
	case "sub":
		return s.makeMathResponse(req, func(x, y float64) float64 { return x - y }), false
	case "mul":
		return s.makeMathResponse(req, func(x, y float64) float64 { return x * y }), false
	case "div":
		return s.makeMathResponse(req, func(x, y float64) float64 { return x / y }), false
	case "pre":
		if err := s.setPrecision(req.Params); err != nil {
			if isNotification {
				return nil, true
			}
			return rpcErrorResponse{
				JSONRPC: "2.0",
				Error:   &rpcErrorObject{Code: -32602, Message: "invalid params"},
				ID:      normalizeID(req.ID),
			}, false
		}
		if isNotification {
			return nil, true
		}
		return rpcResponse{
			JSONRPC: "2.0",
			Result:  json.RawMessage(`"ok"`),
			ID:      normalizeID(req.ID),
		}, false
	default:
		log.Printf("method=%s error=method not found", req.Method)
		return rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32601, Message: "method not found"},
			ID:      normalizeID(req.ID),
		}, false
	}
}

func (s *RPCServer) makeMathResponse(req rpcRequest, operation func(float64, float64) float64) any {
	x, y, err := parsePairParams(req.Params)
	if err != nil {
		log.Printf("%s error=invalid params", req.Method)
		return rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32602, Message: "invalid params"},
			ID:      normalizeID(req.ID),
		}
	}

	result := operation(x, y)
	if math.IsNaN(result) || math.IsInf(result, 0) {
		log.Printf("%s x=%v y=%v invalid numeric result", req.Method, x, y)
		return rpcErrorResponse{
			JSONRPC: "2.0",
			Error:   &rpcErrorObject{Code: -32602, Message: "invalid params"},
			ID:      normalizeID(req.ID),
		}
	}

	formatted := s.formatNumber(result)
	log.Printf("%s x=%v y=%v result=%s", req.Method, x, y, formatted)
	return rpcResponse{
		JSONRPC: "2.0",
		Result:  json.RawMessage(formatted),
		ID:      normalizeID(req.ID),
	}
}

func parsePairParams(raw json.RawMessage) (float64, float64, error) {
	var list []float64
	if len(raw) > 0 && raw[0] == '[' {
		if err := json.Unmarshal(raw, &list); err != nil {
			return 0, 0, err
		}
		if len(list) != 2 {
			return 0, 0, errors.New("expected two params")
		}
		return list[0], list[1], nil
	}

	var params pairParams
	if err := json.Unmarshal(raw, &params); err != nil {
		return 0, 0, err
	}
	return params.X, params.Y, nil
}

func (s *RPCServer) setPrecision(raw json.RawMessage) error {
	var params precisionParams
	if err := json.Unmarshal(raw, &params); err != nil {
		return err
	}
	if params.N < 0 {
		return errors.New("precision must be non-negative")
	}

	s.mu.Lock()
	s.precision = params.N
	s.mu.Unlock()
	log.Printf("precision set to %d", params.N)
	return nil
}

func (s *RPCServer) formatNumber(value float64) string {
	s.mu.RLock()
	precision := s.precision
	s.mu.RUnlock()

	scale := math.Pow10(precision)
	rounded := math.Round(value*scale) / scale
	return strconv.FormatFloat(rounded, 'f', precision, 64)
}

func normalizeID(id json.RawMessage) json.RawMessage {
	if len(bytes.TrimSpace(id)) == 0 {
		return json.RawMessage("null")
	}
	return id
}

func readBody(r *http.Request) ([]byte, error) {
	defer r.Body.Close()

	var body bytes.Buffer
	if _, err := body.ReadFrom(r.Body); err != nil {
		return nil, err
	}
	return body.Bytes(), nil
}

func writeJSON(w http.ResponseWriter, status int, payload any) {
	w.WriteHeader(status)
	if err := json.NewEncoder(w).Encode(payload); err != nil {
		log.Printf("encode response error: %v", err)
	}
}

func writeError(w http.ResponseWriter, payload rpcErrorResponse) {
	writeJSON(w, http.StatusOK, payload)
}

func main() {
	server := NewRPCServer()
	router := NewRouter(server)

	log.Printf("GO07_01 listening on :3000")
	if err := http.ListenAndServe(":3000", router); err != nil {
		log.Fatalf("server stopped: %v", err)
	}
}
