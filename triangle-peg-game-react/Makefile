# triangle-peg-game-react/Makefile

PRECOMPUTE := python3 precompute/precompute.py --export-json tpg-spa/src/assets/states.json
NPM := npm

.PHONY: all prebuild build dist clean example-demo

all: build

prebuild:
	@echo "Generating precomputed states..."
	@$(PRECOMPUTE)

build: prebuild
	@echo "Installing dependencies and building React app..."
	@cd tpg-spa && $(NPM) ci && $(NPM) run build

dist: build
	@echo "Dist artifacts are in tpg-spa/dist"

examples:
	@echo "No compilation necessary for examples."

example-demo:
	@echo "No examples to run for triangle-peg-game-react yet."

tests:
	@echo "No compilation necessary for tests."

test: tests
	@echo "Running sanity checks (states.json)..."
	@cd tpg-spa && npm test

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf tpg-spa/dist
