package dev.dmotservices.tests.api.endpoints;

import dev.dmotservices.api.Endpoint;
import dev.dmotservices.api.annotations.OperationId;
import org.json.JSONObject;

public class EndpointBravo extends Endpoint {
    @Override
    public Object answer() {
        return sayBravo();
    }

    @OperationId("sayBravo")
    public JSONObject sayBravo() {
        return new JSONObject("{\"test\":, \"bravo\"}");
    }
}
