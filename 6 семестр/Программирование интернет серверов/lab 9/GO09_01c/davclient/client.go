package davclient

import (
	"fmt"
	"io"
	"net/http"
	"net/url"
	"os"
	"path"
	"strings"
)

type Client struct {
	BaseURL string
	HTTP    *http.Client
	User    string
	Pass    string
}

func New(baseURL, user, pass string) *Client {
	return &Client{
		BaseURL: strings.TrimRight(baseURL, "/"),
		HTTP:    http.DefaultClient,
		User:    user,
		Pass:    pass,
	}
}

func (c *Client) Mkcol(remote string) error {
	req, err := c.newRequest("MKCOL", remote, nil)
	if err != nil {
		return err
	}
	return c.doNoBody(req, http.StatusCreated)
}

func (c *Client) Put(local, remote string) error {
	file, err := os.Open(local)
	if err != nil {
		return err
	}
	defer file.Close()

	req, err := c.newRequest(http.MethodPut, remote, file)
	if err != nil {
		return err
	}
	return c.doNoBody(req, http.StatusCreated, http.StatusNoContent)
}

func (c *Client) Get(remote, local string) error {
	req, err := c.newRequest(http.MethodGet, remote, nil)
	if err != nil {
		return err
	}
	resp, err := c.httpClient().Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()
	if !statusAllowed(resp.StatusCode, http.StatusOK) {
		return responseError(resp)
	}

	out, err := os.OpenFile(local, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, 0o644)
	if err != nil {
		return err
	}
	defer out.Close()
	_, err = io.Copy(out, resp.Body)
	return err
}

func (c *Client) Copy(src, dst string) error {
	return c.copyOrMove("COPY", src, dst)
}

func (c *Client) Move(src, dst string) error {
	return c.copyOrMove("MOVE", src, dst)
}

func (c *Client) Delete(remote string) error {
	req, err := c.newRequest(http.MethodDelete, remote, nil)
	if err != nil {
		return err
	}
	return c.doNoBody(req, http.StatusNoContent)
}

func (c *Client) copyOrMove(method, src, dst string) error {
	req, err := c.newRequest(method, src, nil)
	if err != nil {
		return err
	}
	destination, err := c.remoteURL(dst)
	if err != nil {
		return err
	}
	req.Header.Set("Destination", destination)
	return c.doNoBody(req, http.StatusCreated, http.StatusNoContent)
}

func (c *Client) newRequest(method, remote string, body io.Reader) (*http.Request, error) {
	target, err := c.remoteURL(remote)
	if err != nil {
		return nil, err
	}
	req, err := http.NewRequest(method, target, body)
	if err != nil {
		return nil, err
	}
	if c.User != "" || c.Pass != "" {
		req.SetBasicAuth(c.User, c.Pass)
	}
	return req, nil
}

func (c *Client) remoteURL(remote string) (string, error) {
	base, err := url.Parse(c.BaseURL)
	if err != nil {
		return "", err
	}
	if base.Scheme == "" || base.Host == "" {
		return "", fmt.Errorf("base URL must be absolute: %s", c.BaseURL)
	}

	cleanRemote := path.Clean("/" + remote)
	if cleanRemote == "/" {
		return strings.TrimRight(base.String(), "/"), nil
	}
	base.Path = strings.TrimRight(base.Path, "/") + cleanRemote
	base.RawQuery = ""
	base.Fragment = ""
	return base.String(), nil
}

func (c *Client) doNoBody(req *http.Request, allowed ...int) error {
	resp, err := c.httpClient().Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()
	if !statusAllowed(resp.StatusCode, allowed...) {
		return responseError(resp)
	}
	io.Copy(io.Discard, resp.Body)
	return nil
}

func (c *Client) httpClient() *http.Client {
	if c.HTTP != nil {
		return c.HTTP
	}
	return http.DefaultClient
}

func statusAllowed(got int, allowed ...int) bool {
	for _, status := range allowed {
		if got == status {
			return true
		}
	}
	return false
}

func responseError(resp *http.Response) error {
	data, _ := io.ReadAll(resp.Body)
	return fmt.Errorf("%s %s: status %d: %s", resp.Request.Method, resp.Request.URL, resp.StatusCode, strings.TrimSpace(string(data)))
}
