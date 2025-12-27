document.addEventListener('DOMContentLoaded', () => {
    // Handles drag-and-drop file uploads for certificate ingestion.

    /*
     * Imports
     */
    const pkixwa = window.pkixwa || {};
    const {
        constants: {
            ALLOWED_CERTIFICATE_EXTENSIONS_AS_CSV,
            CSS_IS_ACTIVE,
            EVENT_CLICK,
            EVENT_DRAG_ENTER,
            EVENT_DRAG_OVER,
            EVENT_DRAG_LEAVE,
            EVENT_DROP,
            FIELD_NAME_CERTIFICATES,
            HTTP_METHOD_POST,
        } = {},
        attachFilesToFormData,
        logger = console,
    } = pkixwa;

    if (!attachFilesToFormData || !EVENT_DRAG_ENTER || !EVENT_CLICK) {
        logger.error("pkixwa globals unavailable; aborting drag-and-drop wiring.");
        return;
    }

    /*
     * Elements
     */

    const dropZone = document.querySelector(".drop-zone");


    /*
     * Event listeners
     */

    if (dropZone) {
        const endpointFileDrop = dropZone.dataset.endpoint;

        const preventDefaultAction = event => {
            event.preventDefault();
            event.stopPropagation();
        };

        [EVENT_DRAG_ENTER, EVENT_DRAG_OVER, EVENT_DRAG_LEAVE, EVENT_DROP].forEach(event =>
            dropZone.addEventListener(event, preventDefaultAction, false)
        );

        [EVENT_DRAG_ENTER, EVENT_DRAG_OVER].forEach(event =>
            dropZone.addEventListener(event, () => dropZone.classList.add(CSS_IS_ACTIVE))
        );
        [EVENT_DRAG_LEAVE, EVENT_DROP].forEach(event =>
            dropZone.addEventListener(event, () => dropZone.classList.remove(CSS_IS_ACTIVE))
        );

        dropZone.addEventListener(EVENT_DROP, async event => {
            const files = event.dataTransfer?.files;
            if (!files?.length) return;

            /*
             * ### Example of the multipart/form-data body ###
             * 
             * Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryABC123
             * 
             * ------WebKitFormBoundaryABC123
             * Content-Disposition: form-data; name="certificates"; filename="cert1.pem"
             * Content-Type: application/octet-stream
             *
             * <binary contents of cert1.pem>
             * ------WebKitFormBoundaryABC123
             * Content-Disposition: form-data; name="certificates"; filename="cert2.pfx"
             * Content-Type: application/octet-stream
             * 
             * <binary contents of cert2.pfx>
             * ------WebKitFormBoundaryABC123--
             * 
             */
            let formDataDrop;
            try {
                formDataDrop = attachFilesToFormData(files, FIELD_NAME_CERTIFICATES);
            } catch (error) {
                logger.error("Failed to build FormData for dropped files.", error);
                return;
            }

            try {
                const res = await fetch(endpointFileDrop, {
                    method: HTTP_METHOD_POST,
                    body: formDataDrop,
                });
                if (!res.ok) throw new Error(`Upload failed (${res.status})`);
                logger.debug("Upload success", await res.json());
            } catch (err) {
                logger.error("Upload error", err);
            }
        });

        dropZone.addEventListener(EVENT_CLICK, () => {
            const input = document.createElement("input");
            input.type = "file";
            input.accept = ALLOWED_CERTIFICATE_EXTENSIONS_AS_CSV;
            input.multiple = true;
            input.onchange = event => {
                const files = event.target.files;
                if (!files?.length) return;
                let formDataClick;
                try {
                    formDataClick = attachFilesToFormData(files, FIELD_NAME_CERTIFICATES);
                } catch (error) {
                    logger.error("Failed to build FormData for selected files.", error);
                    return;
                }
                fetch(endpointFileDrop, { method: HTTP_METHOD_POST, body: formDataClick })
                    .then(response => {
                        if (!response.ok) {
                            throw new Error(`Upload failed (${response.status})`);
                        }
                        return response.json();
                    })
                    .then(payload => logger.debug("Upload success", payload))
                    .catch(error => logger.error("Upload error", error));
            };
            input.click();
        });
    }

    /*
     * Functions
     */
    // none yet


});
