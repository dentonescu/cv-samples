# heapmonj/Makefile

.PHONY: docs docs-openapi

ROOT := $(CURDIR)
OPENAPI := $(ROOT)/api/openapi.yaml
DOC_DIR_API := $(ROOT)/docs/api
DOC_API_HTML := $(DOC_DIR_API)/index.html

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
