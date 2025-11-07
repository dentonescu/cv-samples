document.addEventListener('DOMContentLoaded', () => {
    // Handles the "Fetch certificate by URL" form interactions.

    /*
     * Imports
     */
    const pkixwa = window.pkixwa || {};
    const {
        constants: {
            EVENT_CLICK,
            HEADER_CONTENT_TYPE,
            HTTP_METHOD_POST,
            MIME_TYPE_JSON,
        } = {},
        logger = console,
    } = pkixwa;

    if (!EVENT_CLICK || !HTTP_METHOD_POST) {
        logger.error("pkixwa globals unavailable; aborting certificate URL wiring.");
        return;
    }

    /*
     * Elements
     */

    const urlForm = document.querySelector("form[data-endpoint]");

    /*
     * Event listeners
     */

    if (urlForm) {
        urlForm.addEventListener("submit", async event => {
            event.preventDefault();
            const endpointCertsFromUrl = urlForm.dataset.endpoint;
            const urlInput = urlForm.querySelector("input[type='url']");
            const payload = { url: urlInput?.value?.trim() };

            if (!payload.url) return;

            try {
                const res = await fetch(endpointCertsFromUrl, {
                    method: HTTP_METHOD_POST,
                    headers: { [HEADER_CONTENT_TYPE]: MIME_TYPE_JSON },
                    body: JSON.stringify(payload), // e.g., { "url": "https://example.com"}
                });
                if (!res.ok) throw new Error(`Fetch failed (${res.status})`);
                logger.debug("URL fetch queued", await res.json());
            } catch (err) {
                logger.error("URL fetch error", err);
            }
        });

        const fetchButton = urlForm.querySelector("button");
        if (fetchButton) {
            fetchButton.addEventListener(EVENT_CLICK, () => urlForm.requestSubmit());
        }
    }

    /*
     * Functions
     */

    // none yet


});
