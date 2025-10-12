package dev.dmotservices.tests.api;

import dev.dmotservices.api.*;
import dev.dmotservices.api.json.JsonAPIClient;
import dev.dmotservices.pkix.PKIX;
import org.json.JSONObject;
import org.junit.jupiter.api.BeforeEach;

import static org.junit.jupiter.api.Assertions.*;



import java.io.File;
import java.io.OutputStream;
import java.net.URI;

public class APIClientTests {

    protected static final URI BASE_URI = URI.create("https://timeapi.io/api/");

    @BeforeEach
    public void init() {
        PKIX.disableCertificateValidation();
    }

    @org.junit.jupiter.api.Test
    public void testTimeAPI_GET() {
        System.out.println("\n\ntestTimeAPI_GET():");
        final Response response = new GetTimeClient("Europe/Berlin").execute();
        System.out.format("response=%s\n", response);
        assertTrue(response.isOK());
    }

    @org.junit.jupiter.api.Test
    public void testTimeAPI_POST() {
        System.out.println("\n\ntestTimeAPI_POST():");
        final Response response = translateDateTime(null);
        System.out.format("response=%s\n", response);
        assertTrue(response.isOK());
    }


    @org.junit.jupiter.api.Test
    public void testAlternateOutputStreamWriter() {
        System.out.println("\n\ntestAlternateOututStreamWriter():");
        final OutputStream outputStream = System.out;
        Response response = null;
        try {
            response = translateDateTime(outputStream);
        } catch (Exception e) {
            System.out.println("\nNo response received.");
        }
        if (null != response) {
            System.out.format("\nresponse=%s\n", response);
        }
    }

    @org.junit.jupiter.api.Test
    public void testMultiPartFormData() {
        System.out.println("\n\ntestMultiPartFormData():");
        final OutputStream outputStream = System.out;
        final APIClient apiClient = new DummyClient();
        apiClient.setAlternateOutputStream(outputStream);
        apiClient.setOnlyAlternateOutputStream(true);
        System.out.format("headers=%s\n", apiClient.getRequest().getHeaders());
        try {
            apiClient.execute();
        } catch (Exception e) {
            // There will be no response.
            System.out.println();
        }
    }

    // see: https://timeapi.io/swagger/index.html
    public static class GetTimeClient extends JsonAPIClient {
        protected final String timeZone;

        public GetTimeClient(String timeZone) {
            super(new Request());
            this.timeZone = timeZone;
            final URI endpointURI = URI.create(BASE_URI + "Time/current/zone");
            request.setRequestMethod("GET");
            request.setBaseUri(BASE_URI);
            request.setEndpointUri(endpointURI);
            request.setParam("timeZone", timeZone);
        }

        @Override
        public Response execute() {
            return getResponse();
        }
    }

    public static class TranslateDateTime extends JsonAPIClient {

        protected final String dateTime;
        protected final String languageCode;

        public TranslateDateTime(String dateTime, String languageCode) {
            super(new Request());
            this.dateTime = dateTime;
            this.languageCode = languageCode;
            final URI endpointURI = URI.create(BASE_URI + "Conversion/Translate");
            request.setHeader("Content-Type", MimeType.JSON.getMimeType());
            request.setRequestMethod("POST");
            request.setBaseUri(BASE_URI);
            request.setEndpointUri(endpointURI);
            final JSONObject jo = new JSONObject();
            jo.put("dateTime", dateTime);
            jo.put("languageCode", languageCode);
            request.setPostBody(jo.toString());
        }

        @Override
        public Response execute() {
            return getResponse();
        }
    }

    public static class DummyClient extends JsonAPIClient {


        public DummyClient() {
            super(new Request());
            final URI baseURI = URI.create("https://dummy-host/api");
            final URI endpointURI = URI.create("https://dummy-host/api/does_not_exist");
            request = new MultiPartFormDataRequest();
            request.setBaseUri(baseURI);
            request.setEndpointUri(endpointURI);
            final MultiPartFormDataRequest.Segment[] segment = new MultiPartFormDataRequest.Segment[3];
            for (int i = 0; i < 3; ++i) {
                segment[i] = new MultiPartFormDataRequest.Segment();
                final String payloadPath = "src/test/java/resources/txt/payload.txt";
                switch (i) {
                    case 0:
                        segment[i].setFieldName("file");
                        segment[i].setFileName("payload.txt");
                        segment[i].setMimeType(MimeType.TEXT);
                        segment[i].setData(new File(payloadPath));
                        break;
                    case 1:
                        segment[i].setFieldName("metaData");
                        segment[i].setMimeType(MimeType.JSON);
                        final JSONObject jo = new JSONObject();
                        jo.put("payloadPath", payloadPath);
                        jo.put("size", 28);
                        segment[i].setData(jo);
                        break;
                    case 2:
                        segment[i].setFieldName("blob");
                        segment[i].setFileName("BLOBDATA");
                        segment[i].setMimeType(MimeType.TEXT);
                        segment[i].setData("Reconstructed from a byte array.".getBytes());
                        break;
                    default:
                        throw new IllegalStateException("Invalid segment.");
                }
                ((MultiPartFormDataRequest) request).attachData(segment[i]);
            }

        }

        @Override
        public Response execute() {
            return getResponse();
        }
    }

    protected Response translateDateTime(OutputStream outputStream) {
        final APIClient apiClient = new TranslateDateTime("2021-03-14 17:45:00", "de");
        if (null != outputStream) {
            apiClient.setAlternateOutputStream(outputStream);
            apiClient.setOnlyAlternateOutputStream(true);
        }
        return apiClient.execute();
    }
}