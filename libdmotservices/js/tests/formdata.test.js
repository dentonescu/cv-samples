const test = require("node:test");
const assert = require("node:assert/strict");
const path = require("node:path");

const lib = require(path.join(__dirname, "..", "dist", "index.js"));

class FilePolyfill extends Blob {
  constructor(bits, name, options = {}) {
    super(bits, options);
    this.name = name;
    this.lastModified = options.lastModified ?? Date.now();
  }
}

const FileCtor = globalThis.File || FilePolyfill;

test("attachFilesToFormData throws when no files supplied", () => {
  assert.throws(() => lib.attachFilesToFormData(), /requires at least one file/i);
});

test("attachFilesToFormData appends files under certificates field", () => {
  const file = new FileCtor(["-----BEGIN CERT-----"], "cert.pem", {
    type: "application/x-pem-file",
  });
  const formData = lib.attachFilesToFormData([file]);
  const entries = formData.getAll(lib.constants.FIELD_NAME_CERTIFICATES);
  assert.equal(entries.length, 1);
});
