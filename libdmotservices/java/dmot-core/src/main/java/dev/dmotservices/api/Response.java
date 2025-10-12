package dev.dmotservices.api;

import java.io.Serializable;
import java.net.HttpURLConnection;

public class Response implements Serializable {

    protected String body;
    protected int statusCode;
    protected String statusMessage;

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public int getStatusCode() {
        return statusCode;
    }

    public void setStatusCode(int statusCode) {
        this.statusCode = statusCode;
    }

    public String getStatusMessage() {
        return statusMessage;
    }

    public void setStatusMessage(String statusMessage) {
        this.statusMessage = statusMessage;
    }

    public boolean isOK() {
        final int absDiff = Math.abs(statusCode - HttpURLConnection.HTTP_OK);
        return absDiff < 100;
    }

    @Override
    public String toString() {
        return "Response{" +
                "body='" + body + '\'' +
                ", statusCode=" + statusCode +
                ", statusMessage='" + statusMessage + '\'' +
                '}';
    }
}
