package dev.dmotservices.tests.api;

import dev.dmotservices.api.*;
import dev.dmotservices.api.json.JsonAPIClient;
import dev.dmotservices.pkix.PKIX;
import org.json.JSONObject;
import org.junit.jupiter.api.BeforeEach;

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
}