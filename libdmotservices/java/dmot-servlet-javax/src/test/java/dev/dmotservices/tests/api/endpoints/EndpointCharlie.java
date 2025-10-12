package dev.dmotservices.tests.api.endpoints;

import dev.dmotservices.api.Endpoint;
import dev.dmotservices.api.annotations.OperationId;
import org.json.JSONObject;

public class EndpointCharlie extends Endpoint {
    @Override
    public Object answer() {
        return sayCharlie();
    }

    @OperationId("sayCharlie")
    public JSONObject sayCharlie() {
        return new JSONObject("{\"test\":, \"charlie\"}");
    }
}
