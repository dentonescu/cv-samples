(function (root) {
  "use strict";

  const globalScope = root || (typeof globalThis !== "undefined" ? globalThis : window);
  const dmot = globalScope.dmot || (globalScope.dmot = {});
  const DEFAULT_FIELD = "certificates";

  function attachFilesToFormData(files, targetFormData, fieldName) {
    if (!files || !files.length) {
      throw new Error("attachFilesToFormData requires at least one file.");
    }
    const formData = targetFormData ?? new FormData();
    const resolvedField = fieldName || DEFAULT_FIELD;
    Array.from(files).forEach((file) => formData.append(resolvedField, file));
    return formData;
  }

  dmot.attachFilesToFormData = attachFilesToFormData;
  dmot.constants = Object.assign({}, dmot.constants, {
    FIELD_NAME_CERTIFICATES: DEFAULT_FIELD,
  });
})(typeof globalThis !== "undefined" ? globalThis : this);
