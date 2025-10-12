package dev.dmotservices.api;

import com.google.common.io.ByteStreams;
import org.json.JSONArray;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.logging.Level;
import java.util.logging.Logger;

public abstract class Endpoint extends HttpServlet {

    public abstract Object answer();

    // user specifiable
    protected String contextPath;
    protected Charset charset = StandardCharsets.UTF_8;
    protected MimeType contentType = MimeType.JSON;
    protected Logger logger;

    // internals
    protected HttpServletRequest req;
    protected HttpServletResponse resp;

    protected String body;
    protected RequestMethod requestMethod;

    protected int status = HttpServletResponse.SC_OK;

    public Endpoint() {
        logger = Logger.getLogger(getClass().getName());
    }

    public void setLogger(Logger logger) {
        this.logger = logger;
    }

    public String getContextPath() {
        return contextPath;
    }

    public MimeType getContentType() {
        return contentType;
    }

    public void setContentType(MimeType contentType) {
        this.contentType = contentType;
    }

    public Charset getCharset() {
        return charset;
    }

    public void setCharset(Charset charset) {
        this.charset = charset;
    }

    @Override
    public void doDelete(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        this.requestMethod = RequestMethod.DELETE;
        execute(req, resp);
    }

    @Override
    public void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        this.requestMethod = RequestMethod.GET;
        execute(req, resp);
    }

    @Override
    public void doHead(HttpServletRequest req, HttpServletResponse resp) {
        this.requestMethod = RequestMethod.HEAD;
    }

    @Override
    public void doOptions(HttpServletRequest req, HttpServletResponse resp) {
        this.requestMethod = RequestMethod.OPTIONS;
    }

    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        this.requestMethod = RequestMethod.POST;
        execute(req, resp);
    }

    @Override
    public void doPut(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        this.requestMethod = RequestMethod.PUT;
        execute(req, resp);
    }

    @Override
    public void doTrace(HttpServletRequest req, HttpServletResponse resp) {
        this.requestMethod = RequestMethod.TRACE;
    }

    public void enableCORS() {
        enableCORS(req, resp, logger);
    }

    public static void enableCORS(HttpServletRequest req, HttpServletResponse resp, Logger logger) {
        if (null == logger)
            logger = Logger.getGlobal();
        logger.log(Level.FINE, "enableCORS() has started.");
        String origin = req.getHeader("Origin");
        logger.log(Level.FINE, String.format("enableCORS(): Origin = %s", origin));
        if (null == origin || origin.isBlank())
            origin = "*";
        logger.log(Level.FINE, String.format("enableCORS(): setting Access-Control-Allow-Origin to: %s", origin));
        resp.setHeader("Access-Control-Allow-Origin", origin);
        resp.setHeader("Access-Control-Allow-Credentials", "true");
        resp.setHeader("Access-Control-Allow-Methods", "GET, OPTIONS, HEAD, PUT, POST, DELETE, TRACE");
        resp.setHeader("Access-Control-Max-Age", "3600");
        resp.setHeader("Access-Control-Allow-Headers", "*");
        if (req.getMethod().equalsIgnoreCase("OPTIONS")) {
            resp.setStatus(HttpServletResponse.SC_ACCEPTED);
        }
        logger.log(Level.FINE, "enableCORS() has ended.");
    }

    protected void execute(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        this.req = req;
        this.resp = resp;
        this.requestMethod = RequestMethod.fromString(req.getMethod());
        this.contextPath = req.getContextPath();
        this.body = readInputStream();
        setStatus(status);
        writeResponse(answer());
    }

    protected String readInputStream() throws IOException {
        InputStream inputStream = req.getInputStream();
        return new String(ByteStreams.toByteArray(inputStream), charset);
    }

    protected String getBody() {
        if (null == body) {
            try {
                readInputStream();
            } catch (IOException e) {
                return null;
            }
        }
        return body;
    }

    protected JSONArray getJson() {
        if (null == body) {
            try {
                body = readInputStream();
            } catch (IOException e) {
                return null;
            }
        }
        if (contentType != MimeType.JSON || null == body || body.isEmpty())
            return null;
        String jsonString = getBody().trim();
        if (!jsonString.startsWith("[") && !jsonString.endsWith("]"))
            jsonString = "[" + jsonString + "]";
        return new JSONArray(jsonString);
    }

    protected void setStatus(int statusCode) {
        this.status = statusCode;
        resp.setCharacterEncoding(String.valueOf(charset));
        resp.setStatus(statusCode);
    }

    protected void writeResponse(Object o) throws IOException {
        resp.setContentType(contentType.getMimeType());
        resp.setCharacterEncoding(String.valueOf(charset));
        resp.setStatus(status);
        try (PrintWriter pw = resp.getWriter()) {
            pw.println(o);
        }
    }

    @Override
    public String toString() {
        return "Endpoint{" +
                "contextPath='" + contextPath + '\'' +
                ", charset=" + charset +
                ", contentType=" + contentType +
                ", req=" + req +
                ", resp=" + resp +
                ", body='" + body + '\'' +
                ", requestMethod=" + requestMethod +
                ", status=" + status +
                '}';
    }
}