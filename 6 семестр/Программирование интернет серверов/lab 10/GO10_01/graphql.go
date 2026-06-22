package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"

	"github.com/graphql-go/graphql"
)

type graphQLRequest struct {
	Query         string         `json:"query"`
	Variables     map[string]any `json:"variables"`
	OperationName string         `json:"operationName"`
}

func NewRouter(store CelebrityStore) http.Handler {
	schema, err := NewSchema(store)
	if err != nil {
		panic(err)
	}

	mux := http.NewServeMux()
	mux.Handle("/graphql", graphQLHandler(schema))
	return mux
}

func NewSchema(store CelebrityStore) (graphql.Schema, error) {
	celebrityType := graphql.NewObject(graphql.ObjectConfig{
		Name: "Celebrity",
		Fields: graphql.Fields{
			"id": &graphql.Field{
				Type:    graphql.NewNonNull(graphql.Int),
				Resolve: resolveCelebrityField(func(item Celebrity) any { return item.Id }),
			},
			"fullName": &graphql.Field{
				Type:    graphql.NewNonNull(graphql.String),
				Resolve: resolveCelebrityField(func(item Celebrity) any { return item.FullName }),
			},
			"nationality": &graphql.Field{
				Type:    graphql.NewNonNull(graphql.String),
				Resolve: resolveCelebrityField(func(item Celebrity) any { return item.Nationality }),
			},
			"reqPhotoPath": &graphql.Field{
				Type:    graphql.NewNonNull(graphql.String),
				Resolve: resolveCelebrityField(func(item Celebrity) any { return item.ReqPhotoPath }),
			},
		},
	})

	celebrityInputType := graphql.NewInputObject(graphql.InputObjectConfig{
		Name: "CelebrityInput",
		Fields: graphql.InputObjectConfigFieldMap{
			"id":           &graphql.InputObjectFieldConfig{Type: graphql.Int},
			"fullName":     &graphql.InputObjectFieldConfig{Type: graphql.NewNonNull(graphql.String)},
			"nationality":  &graphql.InputObjectFieldConfig{Type: graphql.NewNonNull(graphql.String)},
			"reqPhotoPath": &graphql.InputObjectFieldConfig{Type: graphql.NewNonNull(graphql.String)},
		},
	})

	queryType := graphql.NewObject(graphql.ObjectConfig{
		Name: "Query",
		Fields: graphql.Fields{
			"celebrities": &graphql.Field{
				Type: graphql.NewNonNull(graphql.NewList(graphql.NewNonNull(celebrityType))),
				Resolve: func(p graphql.ResolveParams) (any, error) {
					return store.All()
				},
			},
			"celebrity": &graphql.Field{
				Type: celebrityType,
				Args: graphql.FieldConfigArgument{
					"id": &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.Int)},
				},
				Resolve: func(p graphql.ResolveParams) (any, error) {
					return store.ByID(p.Args["id"].(int))
				},
			},
		},
	})

	mutationType := graphql.NewObject(graphql.ObjectConfig{
		Name: "Mutation",
		Fields: graphql.Fields{
			"createCelebrity": &graphql.Field{
				Type: celebrityType,
				Args: graphql.FieldConfigArgument{
					"input": &graphql.ArgumentConfig{Type: graphql.NewNonNull(celebrityInputType)},
				},
				Resolve: func(p graphql.ResolveParams) (any, error) {
					item, err := celebrityFromInput(p.Args["input"])
					if err != nil {
						return nil, err
					}
					if item.Id == 0 {
						return nil, errors.New("id is required")
					}
					if err := store.Create(item); err != nil {
						return nil, err
					}
					return item, nil
				},
			},
			"updateCelebrity": &graphql.Field{
				Type: celebrityType,
				Args: graphql.FieldConfigArgument{
					"id":    &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.Int)},
					"input": &graphql.ArgumentConfig{Type: graphql.NewNonNull(celebrityInputType)},
				},
				Resolve: func(p graphql.ResolveParams) (any, error) {
					id := p.Args["id"].(int)
					item, err := celebrityFromInput(p.Args["input"])
					if err != nil {
						return nil, err
					}
					item.Id = id
					if err := store.Update(id, item); err != nil {
						return nil, err
					}
					return item, nil
				},
			},
			"deleteCelebrity": &graphql.Field{
				Type: graphql.NewNonNull(graphql.Boolean),
				Args: graphql.FieldConfigArgument{
					"id": &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.Int)},
				},
				Resolve: func(p graphql.ResolveParams) (any, error) {
					if err := store.Delete(p.Args["id"].(int)); err != nil {
						return nil, err
					}
					return true, nil
				},
			},
		},
	})

	return graphql.NewSchema(graphql.SchemaConfig{
		Query:    queryType,
		Mutation: mutationType,
	})
}

func graphQLHandler(schema graphql.Schema) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if r.Method != http.MethodPost {
			http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
			return
		}

		var request graphQLRequest
		if err := json.NewDecoder(r.Body).Decode(&request); err != nil {
			http.Error(w, "invalid json body", http.StatusBadRequest)
			return
		}

		result := graphql.Do(graphql.Params{
			Schema:         schema,
			RequestString:  request.Query,
			VariableValues: request.Variables,
			OperationName:  request.OperationName,
		})

		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		_ = json.NewEncoder(w).Encode(result)
	})
}

func resolveCelebrityField(resolve func(Celebrity) any) graphql.FieldResolveFn {
	return func(p graphql.ResolveParams) (any, error) {
		switch item := p.Source.(type) {
		case Celebrity:
			return resolve(item), nil
		case *Celebrity:
			return resolve(*item), nil
		default:
			return nil, fmt.Errorf("unexpected celebrity source %T", p.Source)
		}
	}
}

func celebrityFromInput(value any) (Celebrity, error) {
	input, ok := value.(map[string]any)
	if !ok {
		return Celebrity{}, errors.New("invalid celebrity input")
	}

	item := Celebrity{}
	if rawID, ok := input["id"]; ok && rawID != nil {
		id, ok := rawID.(int)
		if !ok {
			return Celebrity{}, errors.New("invalid celebrity id")
		}
		item.Id = id
	}
	item.FullName = input["fullName"].(string)
	item.Nationality = input["nationality"].(string)
	item.ReqPhotoPath = input["reqPhotoPath"].(string)
	return item, nil
}
