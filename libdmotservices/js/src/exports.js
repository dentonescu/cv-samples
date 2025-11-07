(function (root) {
  "use strict";

  const globalScope = root || (typeof globalThis !== "undefined" ? globalThis : window);
  const dmot = globalScope.dmot || (globalScope.dmot = {});

  if (typeof module !== "undefined" && module.exports) {
    // if node's present, export libdmotservices
    module.exports = dmot;
  }
})(typeof globalThis !== "undefined" ? globalThis : this);
