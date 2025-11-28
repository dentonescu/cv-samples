# heapmonj/Makefile

.PHONY: docs docs-openapi backend-build frontend-build all clean test example-demo

ROOT := $(CURDIR)
OPENAPI := $(ROOT)/api/openapi.yaml
DOC_DIR_API := $(ROOT)/docs/api
DOC_API_HTML := $(DOC_DIR_API)/index.html
BACKEND_DIR := $(ROOT)/backend/heapmonj
FRONTEND_DIR := $(ROOT)/frontend/heapmonj-frontend

docs: docs-openapi

docs-openapi: $(DOC_DIR_API) $(OPENAPI)
	@echo "=== Generating OpenAPI HTML docs ==="
	@if ! npx --no-install @redocly/cli --version >/dev/null 2>&1; then \
		echo "Installing @redocly/cli (dev dependency)..."; \
		npm install --save-dev @redocly/cli >/dev/null; \
	fi
	@npx @redocly/cli build-docs "$(OPENAPI)" --output "$(DOC_API_HTML)"

$(DOC_DIR_API):
	@mkdir -p "$@"

backend-build:
	@echo "=== Building backend (Gradle) ==="
	@cd "$(BACKEND_DIR)" && ./gradlew clean build

frontend-build:
	@echo "=== Building frontend (Angular) ==="
	@cd "$(FRONTEND_DIR)" && npm ci && npm run build

all: backend-build frontend-build docs

clean:
	@echo "=== Cleaning backend and frontend artifacts ==="
	@cd "$(BACKEND_DIR)" && ./gradlew clean
	@cd "$(FRONTEND_DIR)" && rm -rf dist out-tsc
	@echo "=== Removing backend data directory ==="
	@rm -rf "$(BACKEND_DIR)/data"

test:
	@echo "=== Running backend tests ==="
	@cd "$(BACKEND_DIR)" && ./gradlew test --rerun-tasks
	@echo "=== Running frontend tests ==="
	@cd "$(FRONTEND_DIR)" && npm test -- --watch=false

example-demo:
	@echo "No runnable examples/demos for heapmonj yet (API + UI only)."
