(function (root) {
  "use strict";

  const globalScope = root || (typeof globalThis !== "undefined" ? globalThis : window);
  const dmot = globalScope.dmot || (globalScope.dmot = {});
  const DEFAULT_PREFIX = "libdmotservices";

  const LEVELS = {
    DEBUG: { value: 10, label: "DEBUG", method: "debug" },
    INFO: { value: 20, label: "INFO", method: "info" },
    WARN: { value: 30, label: "WARN", method: "warn" },
    ERROR: { value: 40, label: "ERROR", method: "error" },
  };

  function resolveConsoleMethod(method) {
    if (typeof console === "undefined") {
      return function noop() { };
    }
    // console methods rely on being called with console as their "this", hence the bind
    return console[method] ? console[method].bind(console) : console.log.bind(console);
  }

  class Logger {
    constructor(prefix = DEFAULT_PREFIX, level = LEVELS.INFO) {
      this.prefix = typeof prefix === "string" ? prefix : DEFAULT_PREFIX;
      this.level = level;
    }

    setPrefix(prefix) {
      this.prefix = typeof prefix === "string" ? prefix : DEFAULT_PREFIX;
    }

    setLevel(level) {
      if (level && typeof level.value === "number") {
        this.level = level;
      }
    }

    shouldLog(level) {
      return level.value >= this.level.value;
    }

    formatLabel(level) {
      const prefix = this.prefix ? ` ${this.prefix}:` : "";
      return `[${level.label}]${prefix}`;
    }

    log(level, ...args) {
      if (!this.shouldLog(level)) {
        return;
      }
      const writer = resolveConsoleMethod(level.method || "log");
      writer(this.formatLabel(level), ...args);
    }

    debug(...args) {
      this.log(LEVELS.DEBUG, ...args);
    }

    info(...args) {
      this.log(LEVELS.INFO, ...args);
    }

    warn(...args) {
      this.log(LEVELS.WARN, ...args);
    }

    error(...args) {
      this.log(LEVELS.ERROR, ...args);
    }
  }

  Logger.LEVELS = LEVELS;
  /*
   * Add read-only getters for the level constants on every Logger instance.
   * This way, consumers don't have to import Logger.LEVELS or use the global object.
   * E.g.,
   *  const logger = new Logger();
   *  logger.setLevel(logger.WARN);
   */
  Object.defineProperties(Logger.prototype, {
    DEBUG: { get: () => LEVELS.DEBUG },
    INFO: { get: () => LEVELS.INFO },
    WARN: { get: () => LEVELS.WARN },
    ERROR: { get: () => LEVELS.ERROR },
  });

  dmot.Logger = Logger;
  if (!dmot.logger) {
    dmot.logger = new Logger();
  }
})(typeof globalThis !== "undefined" ? globalThis : this);
