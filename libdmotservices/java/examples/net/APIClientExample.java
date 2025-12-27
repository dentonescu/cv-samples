import java.io.OutputStream;
import java.net.URI;

import dev.dmotservices.api.*;
import dev.dmotservices.api.json.JsonAPIClient;
import dev.dmotservices.pkix.PKIX;
import org.json.JSONObject;

public class APIClientExample {

    protected static final URI BASE_URI = URI.create("https://timeapi.io/api/");
    
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

    protected static Response translateDateTime(OutputStream outputStream) {
        final APIClient apiClient = new TranslateDateTime("2021-03-14 17:45:00", "de");
        if (null != outputStream) {
            apiClient.setAlternateOutputStream(outputStream);
            apiClient.setOnlyAlternateOutputStream(true);
        }
        return apiClient.execute();
    }

    public static void demoTimeAPI_GET() {
        System.out.println("\n\ndemoTimeAPI_GET():");
        final Response response = new GetTimeClient("Europe/Berlin").execute();
        System.out.format("response=%s\n", response);
    }

    public static void demoTimeAPI_POST() {
        System.out.println("\n\ndemoTimeAPI_POST():");
        final Response response = translateDateTime(null);
        System.out.format("response=%s\n", response);
    }

    public static void main(String[] args) {
        demoTimeAPI_GET();
        demoTimeAPI_POST();
    }
    
}