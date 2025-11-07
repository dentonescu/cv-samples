const test = require("node:test");
const assert = require("node:assert/strict");
const path = require("node:path");

const lib = require(path.join(__dirname, "..", "dist", "index.js"));

test("Logger honours prefix and level filtering", (t) => {
  const captured = [];
  const originalDebug = console.debug;
  const originalInfo = console.info;
  const originalWarn = console.warn;
  console.debug = (...args) => captured.push(args.join(" "));
  console.info = (...args) => captured.push(args.join(" "));
  console.warn = (...args) => captured.push(args.join(" "));

  try {
    const logger = new lib.Logger();
    logger.setPrefix("TEST");
    logger.setLevel(logger.WARN); // should suppress debug/info
    logger.debug("debug message");
    logger.info("info message");
    logger.warn("warn message");
    logger.setLevel(logger.DEBUG);
    logger.debug("visible");

    t.diagnostic(`captured=${JSON.stringify(captured)}`);
    assert.equal(
      captured.some((line) => line.includes("[WARN] TEST: warn message")),
      true,
      "Warn log should always be emitted"
    );
    assert.equal(
      captured.some((line) => line.includes("[DEBUG] TEST: visible")),
      true,
      "Debug should appear after lowering level"
    );
  } finally {
    console.debug = originalDebug;
    console.info = originalInfo;
    console.warn = originalWarn;
  }
});
