package dev.dmotservices.api;

import java.io.Serializable;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

public class Request implements Serializable {

    public static final String ERROR_URL_TO_URI = "The URL could not be converted to a URI type.\n%s";
    public static final String ERROR_URI_TO_URL = "The URI provided could not be converted a URL type.\n%s";
    protected URL baseUrl;
    protected URL endpointUrl;
    protected Map<String, String> headers = new HashMap<>();
    protected Map<String, String> params = new HashMap<>();
    protected String postBody = "";
    protected String requestMethod;

    public URL getBaseUrl() {
        return baseUrl;
    }

    public URI getBaseUri() {
        try {
            return baseUrl.toURI();
        } catch (URISyntaxException e) {
            throw new IllegalArgumentException(String.format(ERROR_URL_TO_URI, e));
        }
    }

    public void setBaseUrl(URL baseUrl) {
        this.baseUrl = baseUrl;
    }

    public void setBaseUri(URI baseUri) {
        try {
            setBaseUrl(baseUri.toURL());
        } catch (MalformedURLException e) {
            throw new IllegalArgumentException(String.format(ERROR_URI_TO_URL, e));
        }
    }

    public String getRequestMethod() {
        return requestMethod;
    }

    public void setRequestMethod(String requestMethod) {
        this.requestMethod = requestMethod;
    }

    public URL getEndpointUrl() {
        return endpointUrl;
    }

    public URI getEndpointUri() {
        try {
            return endpointUrl.toURI();
        } catch (URISyntaxException e) {
            throw new IllegalArgumentException(String.format(ERROR_URL_TO_URI, e));
        }
    }

    public void setEndpointUrl(URL endpointUrl) {
        this.endpointUrl = endpointUrl;
    }

    public void setEndpointUri(URI endpointUri) {
        try {
            setEndpointUrl(endpointUri.toURL());
        } catch (MalformedURLException e) {
            throw new IllegalArgumentException(String.format(ERROR_URI_TO_URL, e));
        }
    }

    public String getPostBody() {
        return postBody;
    }

    public void setPostBody(String postBody) {
        this.postBody = postBody;
    }

    public Map<String, String> getHeaders() {
        return headers;
    }

    public void setHeaders(Map<String, String> headers) {
        this.headers = headers;
    }

    public Map<String, String> getParams() {
        return params;
    }

    public void setParams(Map<String, String> params) {
        this.params = params;
    }

    public int sizeHeaders() {
        return headers.size();
    }

    public void setHeader(String key, String value) {
        headers.put(key, value);
    }

    public void setParam(String key, String value) {
        params.put(key, value);
    }

    @Override
    public String toString() {
        return "Request{" +
                "baseUrl=" + baseUrl +
                ", endpointUrl=" + endpointUrl +
                ", headers=" + headers +
                ", params=" + params +
                ", postBody='" + postBody + '\'' +
                ", requestMethod='" + requestMethod + '\'' +
                '}';
    }
}
