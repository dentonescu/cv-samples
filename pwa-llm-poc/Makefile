# pwa-llm-poc/Makefile

NPM := npm

.PHONY: all build dist clean example-demo docs

all: build

build:
	@echo "Installing dependencies and building React app..."
	@cd frontend/llm-spa && $(NPM) ci && $(NPM) run build

dist: build
	@echo "Dist artifacts are in frontend/llm-spa/dist"

examples:
	@echo "No compilation necessary for examples."

example-demo:
	@echo "No examples to run for pwa-llm-poc yet."

tests:
	@echo "No compilation necessary for tests."

test: tests
	@echo "No unit tests for pwa-llm-poc yet."

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf frontend/llm-spa/dist

run:
	@echo "Launching the frontend and backend..."
	./run-pwa-llm.sh

docs:
	@echo "Generating OpenAPI HTML docs..."
	@mkdir -p docs/api
	@npx --yes @redocly/cli build-docs api/openapi.yaml -o docs/api/index.html
