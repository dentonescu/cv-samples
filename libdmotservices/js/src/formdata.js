(function (root) {
  "use strict";

  const globalScope = root || (typeof globalThis !== "undefined" ? globalThis : window);
  const dmot = globalScope.dmot || (globalScope.dmot = {});
  const DEFAULT_FIELD = "files";

  function attachFilesToFormData(files, fieldName, targetFormData) {
    if (!files || !files.length) {
      throw new Error("attachFilesToFormData requires at least one file.");
    }
    const formData = targetFormData ?? new FormData();
    const resolvedField = fieldName || DEFAULT_FIELD;
    Array.from(files).forEach((file) => formData.append(resolvedField, file));
    return formData;
  }

  dmot.attachFilesToFormData = attachFilesToFormData;
})(typeof globalThis !== "undefined" ? globalThis : this);
