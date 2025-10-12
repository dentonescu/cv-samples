package dev.dmotservices.api.json;

import dev.dmotservices.api.APIClient;
import dev.dmotservices.api.Request;
import dev.dmotservices.api.Response;

public abstract class JsonAPIClient extends APIClient {

    protected JsonAPIClient() {
        super();
        // prevent instantiation with new
    }

    public JsonAPIClient(Request request) {
        super(request);
    }

    public Response getResponse() {
        Response superResponse = null, jsonResponse;
        try {
            superResponse = super.getResponse();
            jsonResponse = new JsonResponse(superResponse);
        } catch (Exception e) {
            throw new IllegalStateException(
                    String.format("""
                                    The response could not be converted to a JSON response.
                                    response = %s
                                    superResponse = %s
                                    jsonResponse variable could not be instantiated.
                                    %s""",
                            response,
                            superResponse,
                            e));
        }
        return jsonResponse;
    }

    @SuppressWarnings("EmptyMethod")
    public Request getRequest() {
        return super.getRequest();
    }

    @SuppressWarnings("unused")
    public void setRequest(Request request) {
        super.setRequest(request);
    }
}
