package dev.dmotservices.tests.api.endpoints;

import dev.dmotservices.api.Endpoint;
import dev.dmotservices.api.annotations.OperationId;
import org.json.JSONObject;

public class EndpointAlpha extends Endpoint {
    @Override
    public Object answer() {
        return sayAlpha();
    }

    @OperationId("sayAlpha")
    public JSONObject sayAlpha() {
        return new JSONObject("{\"test\":, \"alpha\"}");
    }
}
