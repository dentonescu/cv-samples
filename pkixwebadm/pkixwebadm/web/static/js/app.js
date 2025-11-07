(function (root) {
    /**
     * Bootstrap the pkixwebadm front-end namespace.
     *
     * Exposes shared constants, logging, and helper wiring under `window.pkixwa`
     * while leaving the global namespace untouched.
     */
    "use strict";

    /*
     * Constants
     */
    const ALLOWED_CERTIFICATE_EXTENSIONS = ['.pem', '.cer', '.crt', '.der', '.pfx', '.p12'];
    const ALLOWED_CERTIFICATE_EXTENSIONS_AS_CSV = ALLOWED_CERTIFICATE_EXTENSIONS.join(',');
    const CSS_IS_ACTIVE = "is-active";
    const EVENT_CLICK = "click";
    const EVENT_DRAG_ENTER = "dragenter";
    const EVENT_DRAG_OVER = "dragover";
    const EVENT_DRAG_LEAVE = "dragleave";
    const EVENT_DROP = "drop";
    const HEADER_CONTENT_TYPE = "Content-Type";
    const HTTP_METHOD_GET = "GET";
    const HTTP_METHOD_POST = "POST";
    const MIME_TYPE_JSON = "application/json";

    /*
     * Namespaces
     */
    const pkixwa = root.pkixwa || (root.pkixwa = {});
    const dmot = root.dmot || {};
    const logger = dmot.logger || console;
    if (logger.setPrefix) {
        logger.setPrefix("PKIXWEBADM");
    }
    if (logger.setLevel && logger.DEBUG) {
        logger.setLevel(logger.DEBUG);
    }

    pkixwa.logger = logger;
    pkixwa.attachFilesToFormData = dmot.attachFilesToFormData || (() => {
        throw new Error("dmot.attachFilesToFormData is unavailable.");
    });
    pkixwa.constants = {
        ALLOWED_CERTIFICATE_EXTENSIONS,
        ALLOWED_CERTIFICATE_EXTENSIONS_AS_CSV,
        CSS_IS_ACTIVE,
        EVENT_CLICK,
        EVENT_DRAG_ENTER,
        EVENT_DRAG_OVER,
        EVENT_DRAG_LEAVE,
        EVENT_DROP,
        HEADER_CONTENT_TYPE,
        HTTP_METHOD_GET,
        HTTP_METHOD_POST,
        MIME_TYPE_JSON,
    };

    /*
     * Exports scoped via pkixwa namespace (avoid polluting window)
     */
})(typeof window !== "undefined" ? window : globalThis);
