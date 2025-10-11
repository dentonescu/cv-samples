package dev.dmotservices.api;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public abstract class APIClient {

    protected static final String CRLF = "\r\n";
    public static final String DELIMITER = "--";

    protected OutputStream alternateOutputStream = null;
    protected boolean onlyAlternateOutputStream = false;
    protected Request request;
    protected Response response;

    protected APIClient() {
        // prohibit the default constructor
    }

    public APIClient(Request request) {
        this.request = request;
    }

    public abstract Response execute();

    protected boolean isConnectionStatusOK(HttpURLConnection conn) throws IOException {
        int responseCode = conn.getResponseCode();
        // 2XX -> OK (by default)
        int absDiff = Math.abs(responseCode - HttpURLConnection.HTTP_OK);
        return (absDiff < 100);
    }

    public Response getResponse() throws IOException {
        return getResponse(StandardCharsets.UTF_8);
    }

    public Response getResponse(Charset charset) throws IOException {

        StringBuilder sbBody = new StringBuilder();
        response = new Response();
        HttpURLConnection conn = null;
        StringBuilder sbValidation = new StringBuilder();
        URL url = request.getEndpointUrl();
        String requestMethod = request.getRequestMethod();

        // validation
        if (null == requestMethod || requestMethod.trim().isEmpty()) {
            requestMethod = "GET";
        }

        if (null == url) {
            sbValidation.append("ERROR: an endpoint URL has not been set.\n");
        }

        if (!sbValidation.isEmpty()) {
            throw new IllegalStateException(sbValidation.toString());
        }

        assert url != null : "A URL must be provided.";

        // append query parameters to the URL, if any
        StringBuilder sb = new StringBuilder(url.toString());
        Map<String, String> paramMap = request.getParams();
        int i = 0;
        for (Map.Entry<String, String> entry : paramMap.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue();
            sb.append((i <= 0) ? "?" : "&");
            sb.append(key).append("=").append(value);
            ++i;
        }
        if (i > 0)
            url = new URL(sb.toString());

        // establish a connection to the URL
        try {
            conn = (HttpURLConnection) url.openConnection();
            if (null == conn) {
                throw new IOException("Could not open a connection to the URL: " + url);
            }

            // request headers
            Map<String, String> headerMap = request.getHeaders();
            for (Map.Entry<String, String> entry : headerMap.entrySet()) {
                String key = entry.getKey();
                String value = entry.getValue();
                conn.setRequestProperty(key, value);
            }

            // if POST/PUT, write body
            conn.setRequestMethod(requestMethod);
            requestMethod = requestMethod.trim().toUpperCase();
            if (requestMethod.contains("POST") || requestMethod.contains("PUT")) {
                conn.setDoOutput(true);
                final List<OutputStream> outputStreamList = new ArrayList<>();
                if (null != alternateOutputStream)
                    outputStreamList.add(alternateOutputStream);
                if (!onlyAlternateOutputStream)
                    outputStreamList.add(conn.getOutputStream());
                for (OutputStream outputStream : outputStreamList) {
                    if (outputStream instanceof PrintStream) {
                        // We want to avoid using a try-with-resources with System.out. It would crash this routine.
                        final OutputStreamWriter outputStreamWriter = new OutputStreamWriter(outputStream, charset);
                        writePostBody(outputStream, outputStreamWriter, charset);
                        outputStreamWriter.flush();
                    } else
                        try (OutputStreamWriter outputStreamWriter = new OutputStreamWriter(outputStream, charset)) {
                            writePostBody(outputStream, outputStreamWriter, charset);
                            outputStreamWriter.flush();
                        }
                }
            }

            // read the response
            if (onlyAlternateOutputStream) {
                response = null;
            } else {
                try (BufferedReader br = new BufferedReader(new InputStreamReader(
                        isConnectionStatusOK(conn) ?
                                conn.getInputStream() // if HTTP_OK, use the input stream
                                :
                                conn.getErrorStream() // if not HTTP_OK, use the error stream
                        , charset))) {
                    String line;
                    while (null != (line = br.readLine())) {
                        sbBody.append(line);
                    }
                    response.setBody(sbBody.toString());
                }
            }
        } finally {
            if (null != conn && !onlyAlternateOutputStream) {
                response.setStatusCode(conn.getResponseCode());
                response.setStatusMessage(conn.getResponseMessage());
                conn.disconnect();
            }
        }
        return response;
    }

    protected void writePostBody(OutputStream outputStream,
                                 OutputStreamWriter outputStreamWriter, Charset charset) throws IOException {
        if (request instanceof MultiPartFormDataRequest) {
            // multipart form data
            for (MultiPartFormDataRequest.Segment segment : ((MultiPartFormDataRequest) request).getSegments()) {
                outputStreamWriter.append(DELIMITER).append(MultiPartFormDataRequest.BOUNDARY).append(CRLF);
                outputStreamWriter.append("Content-Disposition: form-data; name=\"")
                        .append(segment.getFieldName()).append("\"");
                if (null != segment.getFileName() && !segment.getFileName().isBlank())
                    outputStreamWriter.append("; filename=\"").append(segment.getFileName()).append("\"");
                outputStreamWriter.append(CRLF);
                if (null != segment.getMimeType())  // only include the mime type if it's available
                    outputStreamWriter.append("Content-Type: ").append(segment.getMimeType().getMimeType()).append(CRLF);
                outputStreamWriter.append(CRLF);
                if (segment.getData() instanceof File) {
                    outputStreamWriter.flush();
                    try (FileInputStream is = new FileInputStream((File) segment.getData())) {
                        byte[] buffer = new byte[4096];
                        int bytesRead;
                        while ((bytesRead = is.read(buffer)) != -1) {
                            final String decodedString = new String(buffer, 0, bytesRead, charset);
                            outputStreamWriter.write(decodedString);
                        }
                    }
                } else if (segment.getData() instanceof byte[]) {
                    outputStreamWriter.flush();
                    outputStream.write((byte[]) segment.getData());
                    outputStream.flush();
                } else {
                    outputStreamWriter.append(String.valueOf(segment.getData()));
                }
                outputStreamWriter.append(CRLF);
            }
            // close with the boundary
            outputStreamWriter.append(DELIMITER).append(MultiPartFormDataRequest.BOUNDARY).append(DELIMITER).append(CRLF);
        } else {
            // standard post body
            outputStreamWriter.write(request.getPostBody());
        }
    }

    /*
     * generated
     */

    public OutputStream getAlternateOutputStream() {
        return alternateOutputStream;
    }

    public void setAlternateOutputStream(OutputStream alternateOutputStream) {
        this.alternateOutputStream = alternateOutputStream;
    }

    public Request getRequest() {
        return request;
    }

    public void setRequest(Request request) {
        this.request = request;
    }

    public boolean isOnlyAlternateOutputStream() {
        return onlyAlternateOutputStream;
    }

    public void setOnlyAlternateOutputStream(boolean onlyAlternateOutputStream) {
        this.onlyAlternateOutputStream = onlyAlternateOutputStream;
    }

    public void setResponse(Response response) {
        this.response = response;
    }
}
