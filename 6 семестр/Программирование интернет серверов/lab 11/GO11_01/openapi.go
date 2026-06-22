package main

import (
	"net/http"
)

const openAPIJSON = `{
  "openapi": "3.0.3",
  "info": {
    "title": "GO11_01 Celebrities API",
    "version": "1.0.0"
  },
  "servers": [
    {
      "url": "http://localhost:3000"
    }
  ],
  "paths": {
    "/Celebrities/All": {
      "get": {
        "summary": "List celebrities",
        "responses": {
          "200": {
            "description": "Celebrities list",
            "content": {
              "application/json": {
                "schema": {
                  "type": "array",
                  "items": {
                    "$ref": "#/components/schemas/Celebrity"
                  }
                }
              }
            }
          }
        }
      }
    },
    "/Celebrities/{id}": {
      "get": {
        "summary": "Get celebrity by id",
        "parameters": [
          {
            "$ref": "#/components/parameters/CelebrityId"
          }
        ],
        "responses": {
          "200": {
            "description": "Celebrity",
            "content": {
              "application/json": {
                "schema": {
                  "$ref": "#/components/schemas/Celebrity"
                }
              }
            }
          },
          "400": {
            "description": "Invalid id"
          },
          "404": {
            "description": "Celebrity not found"
          }
        }
      },
      "put": {
        "summary": "Update celebrity",
        "parameters": [
          {
            "$ref": "#/components/parameters/CelebrityId"
          }
        ],
        "requestBody": {
          "$ref": "#/components/requestBodies/CelebrityBody"
        },
        "responses": {
          "200": {
            "description": "Updated celebrity",
            "content": {
              "application/json": {
                "schema": {
                  "$ref": "#/components/schemas/Celebrity"
                }
              }
            }
          },
          "400": {
            "description": "Invalid id or JSON"
          },
          "404": {
            "description": "Celebrity not found"
          }
        }
      },
      "delete": {
        "summary": "Delete celebrity",
        "parameters": [
          {
            "$ref": "#/components/parameters/CelebrityId"
          }
        ],
        "responses": {
          "204": {
            "description": "Deleted"
          },
          "400": {
            "description": "Invalid id"
          },
          "404": {
            "description": "Celebrity not found"
          }
        }
      }
    },
    "/Celebrities": {
      "post": {
        "summary": "Create celebrity",
        "requestBody": {
          "$ref": "#/components/requestBodies/CelebrityBody"
        },
        "responses": {
          "201": {
            "description": "Created celebrity",
            "content": {
              "application/json": {
                "schema": {
                  "$ref": "#/components/schemas/Celebrity"
                }
              }
            }
          },
          "400": {
            "description": "Invalid JSON"
          },
          "409": {
            "description": "Celebrity already exists"
          }
        }
      }
    }
  },
  "components": {
    "parameters": {
      "CelebrityId": {
        "name": "id",
        "in": "path",
        "required": true,
        "schema": {
          "type": "integer"
        }
      }
    },
    "requestBodies": {
      "CelebrityBody": {
        "required": true,
        "content": {
          "application/json": {
            "schema": {
              "$ref": "#/components/schemas/Celebrity"
            }
          }
        }
      }
    },
    "schemas": {
      "Celebrity": {
        "type": "object",
        "required": [
          "id",
          "fullName",
          "nationality",
          "reqPhotoPath"
        ],
        "properties": {
          "id": {
            "type": "integer",
            "example": 1
          },
          "fullName": {
            "type": "string",
            "example": "Keanu Reeves"
          },
          "nationality": {
            "type": "string",
            "example": "Canadian"
          },
          "reqPhotoPath": {
            "type": "string",
            "example": "/img/keanu.jpg"
          }
        }
      }
    }
  }
}`

const openAPIUIHTML = `<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>GO11_01 OpenAPI UI</title>
  <link rel="stylesheet" href="https://unpkg.com/swagger-ui-dist@5/swagger-ui.css">
</head>
<body>
  <div id="swagger-ui"></div>
  <script src="https://unpkg.com/swagger-ui-dist@5/swagger-ui-bundle.js"></script>
  <script>
    window.onload = function() {
      SwaggerUIBundle({
        url: "/openapi.json",
        dom_id: "#swagger-ui"
      });
    };
  </script>
</body>
</html>`

func openAPIHandler() http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		_, _ = w.Write([]byte(openAPIJSON))
	}
}

func openAPIUIHandler() http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "text/html; charset=utf-8")
		w.WriteHeader(http.StatusOK)
		_, _ = w.Write([]byte(openAPIUIHTML))
	}
}
