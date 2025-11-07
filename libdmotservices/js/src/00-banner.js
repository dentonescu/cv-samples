(function (root) {
  "use strict";
  // note: globalThis is a built-in added in ES2020
  const globalScope = root || (typeof globalThis !== "undefined" ? globalThis : window);
  if (!globalScope) {
    throw new Error("Unable to determine global scope for libdmotservices.");
  }
  if (!globalScope.dmot) {
    globalScope.dmot = {}; // establish the dmot namespace if nonexistent
  }
  const dmot = globalScope.dmot;
  dmot.jsVersion = dmot.jsVersion || "0.0.0";
})(typeof globalThis !== "undefined" ? globalThis : this);
