package dev.dmotservices.api.json;

import dev.dmotservices.api.Response;
import org.json.JSONArray;

import java.io.Serializable;

public class JsonResponse extends Response implements Serializable {

    protected final JSONArray json;

    public JsonResponse(Response response) {
        if (null == response || null == response.getBody())
            throw new NullPointerException(
                    String.format("%s: There was no body in the response present for the creation of a JSON array.",
                            getClass().getName())
            );
        this.body = response.getBody();
        this.statusCode = response.getStatusCode();
        this.statusMessage = response.getStatusMessage();
        String jsonBody = body.trim();
        if (!jsonBody.startsWith("["))
            jsonBody = "[" + jsonBody + "]";
        this.json = new JSONArray(jsonBody);
    }

    public JSONArray getJson() {
        return json;
    }

    @Override
    public String toString() {
        return "JsonResponse{" +
                "json=" + json +
                ", body='" + body + '\'' +
                ", statusCode=" + statusCode +
                ", statusMessage='" + statusMessage + '\'' +
                '}';
    }
}
