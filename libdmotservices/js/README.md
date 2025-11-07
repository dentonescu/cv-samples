# libdmotservices/js

Front-end friendly helpers shared across the pkixwebadm and slideshow-server projects. The initial slice exports:

- `attachFilesToFormData(files, [formData], [fieldName])` – appends dropped/selected files to a `FormData` instance (defaults to the `certificates` field).
- `Logger` / `dmot.logger` – lightweight console wrapper with prefix and level controls.

## Layout

```
js/
├─ src/                  # Source modules (concatenated during build)
├─ dist/index.js         # Browser/Node-ready bundle (generated)
├─ vendor/libdmotservices/index.js  # Artifact consumed by projects (generated)
├─ tests/*.test.js       # Node's built-in test runner (`node --test`)
└─ Makefile              # build/test/vendor/clean helpers
```

## Usage

```bash
make -C libdmotservices/js build    # generates dist/index.js
make -C libdmotservices/js vendor   # copies dist → vendor/libdmotservices/index.js
make -C libdmotservices/js test     # installs deps (first run), lints, then runs node --test
```

> Tip: the `node_modules/` directory is created automatically during the first `make ... test`; if you are offline, run `npm install` ahead of time in a connected environment.

In the browser:

```html
<script src="/static/vendor/libdmotservices/index.js"></script>
<script>
  dmot.logger.setPrefix("MyApp");
  dmot.logger.setLevel(dmot.logger.DEBUG);
  dmot.logger.debug("This is a test!)
  const formData = dmot.attachFilesToFormData(fileInput.files);
</script>
```

Node/CommonJS can `require` the bundle as well:

```js
const { Logger, attachFilesToFormData } = require('./dist/index.js');
```
