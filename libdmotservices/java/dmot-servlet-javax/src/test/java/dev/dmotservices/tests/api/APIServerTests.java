package dev.dmotservices.tests.api;

import dev.dmotservices.api.APIServer;
import dev.dmotservices.api.Endpoint;
import dev.dmotservices.api.MimeType;
import dev.dmotservices.api.RequestMethod;
import dev.dmotservices.api.annotations.OperationId;
import dev.dmotservices.api.yaml.YAMLContextPathFinder;
import dev.dmotservices.reflection.AnnotationHandler;
import dev.dmotservices.reflection.Reflection;
import dev.dmotservices.tests.api.endpoints.EndpointAlpha;
import dev.dmotservices.tests.api.mock.MockServletContextEvent;
import org.directwebremoting.util.DelegatingServletInputStream;
import org.json.JSONObject;
import org.junit.jupiter.api.BeforeEach;
import org.mockito.Mockito;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class APIServerTests {

    public static final String YAML_PATH = "yaml/libdmotservices.yaml";

    protected static final String ENDPOINT_CONTEXT_PATH = "/tests/endpoint";
    protected static final String JSON_TEST_STRING = "{\"test\": \"this is a test\"}";
    protected static final String HTML_TEST_STRING = "<html><body><p>Hello, World!</p></body></html>";
    protected static HttpServletRequest request;
    protected static HttpServletResponse response;
    protected PrintWriter writer;
    protected StringWriter stringWriter;


    @BeforeEach
    public void init() throws IOException {
        request = mock(HttpServletRequest.class);
        response = mock(HttpServletResponse.class);
        stringWriter = new StringWriter();
        writer = new PrintWriter(stringWriter);
        when(request.getCharacterEncoding()).thenReturn("UTF-8");
        when(response.getWriter()).thenReturn(writer);
        Mockito.doNothing().when(response).setStatus(Mockito.anyInt());
        Mockito.doNothing().when(response).setCharacterEncoding(Mockito.anyString());
    }

    private void fillEndpoint(Endpoint endpoint) {
        MimeType mimeType = MimeType.fromString(request.getContentType());
        Map<String, Object> keyValueMap = new HashMap<>();
        keyValueMap.put("body", mimeType == MimeType.JSON ? JSON_TEST_STRING : HTML_TEST_STRING);
        keyValueMap.put("contentType", mimeType);
        keyValueMap.put("contextPath", request.getContextPath());
        keyValueMap.put("requestMethod", request.getMethod());
        keyValueMap.put("req", request);
        keyValueMap.put("resp", response);
        Reflection.massInjectBruteForce(endpoint, keyValueMap);
    }

    @org.junit.jupiter.api.Test
    public void testMimeType() {
        System.out.println("\n\ntestMimeType():");
        final MimeType html = MimeType.fromString("text/html");
        final MimeType json = MimeType.fromString("application/json");
        final MimeType text = MimeType.fromString("text/plain");
        final MimeType unknown = MimeType.fromString("this mime type does not exist");
        final MimeType xml = MimeType.fromString("application/xml");
        System.out.format("html => %s\n", html);
        System.out.format("json => %s\n", json);
        System.out.format("text => %s\n", text);
        System.out.format("unknown => %s\n", unknown);
        System.out.format("xml => %s\n", xml);
        assertEquals("text/html", html.getMimeType());
        assertEquals("application/json", json.getMimeType());
        assertEquals("text/plain", text.getMimeType());
        assertNull(unknown);
        assertEquals("application/xml", xml.getMimeType());
    }

    @org.junit.jupiter.api.Test
    public void testRequestMethod() {
        System.out.println("\n\ntestRequestMethod():");
        final RequestMethod get = RequestMethod.fromString("get");
        final RequestMethod post = RequestMethod.fromString("post");
        final RequestMethod put = RequestMethod.fromString("put");
        final RequestMethod unknown = RequestMethod.fromString("this is not a real request method");
        System.out.format("get => %s\n", get);
        System.out.format("post => %s\n", post);
        System.out.format("put => %s\n", put);
        System.out.format("unknown => %s\n", unknown);
        assertEquals("GET", get.getRequestMethod());
        assertEquals("POST", post.getRequestMethod());
        assertEquals("PUT", put.getRequestMethod());
        assertNull(unknown);
    }

    @org.junit.jupiter.api.Test
    public void testJsonEndpoint() throws IOException {
        System.out.println("\n\ntestJsonEndpoint():");
        when(request.getContentType()).thenReturn(MimeType.JSON.getMimeType());
        when(request.getContextPath()).thenReturn(ENDPOINT_CONTEXT_PATH);
        when(request.getInputStream()).thenReturn(
                new DelegatingServletInputStream(
                        new ByteArrayInputStream(JSON_TEST_STRING.getBytes(StandardCharsets.UTF_8))
                )
        );
        when(request.getReader()).thenReturn(
                new BufferedReader(new StringReader(JSON_TEST_STRING))
        );
        final Endpoint endpoint = new Endpoint() {
            @Override
            public Object answer() {
                return getJson();
            }
        };
        fillEndpoint(endpoint);
        Reflection.injectValueBruteForce(endpoint, "requestMethod", RequestMethod.PUT);
        Object o = endpoint.answer();
        final String endpointText = endpoint.toString();
        final String answerText = String.valueOf(o);
        System.out.format("endpoint = %s\n", endpoint);
        System.out.format("endpoint.answer() = %s\n", answerText);
        assertTrue(endpointText.contains("contextPath='" + ENDPOINT_CONTEXT_PATH + "'"));
        assertTrue(endpointText.contains("charset=UTF-8"));
        assertTrue(endpointText.contains("requestMethod=PUT"));
        assertTrue(endpointText.contains("status=200"));
        assertTrue(answerText.contains("this is a test"));
    }

    @org.junit.jupiter.api.Test
    public void testHtmlEndpoint() throws IOException {
        System.out.println("\n\ntestHtmlEndpoint():");
        when(request.getContentType()).thenReturn(MimeType.HTML.getMimeType());
        when(request.getContextPath()).thenReturn(ENDPOINT_CONTEXT_PATH);
        when(request.getInputStream()).thenReturn(
                new DelegatingServletInputStream(
                        new ByteArrayInputStream(HTML_TEST_STRING.getBytes(StandardCharsets.UTF_8))
                )
        );
        when(request.getReader()).thenReturn(
                new BufferedReader(new StringReader(HTML_TEST_STRING))
        );
        final Endpoint endpoint = new Endpoint() {
            @Override
            public Object answer() {
                return getBody();
            }
        };
        fillEndpoint(endpoint);
        Reflection.injectValueBruteForce(endpoint, "requestMethod", RequestMethod.GET);
        Object o = endpoint.answer();
        final String endpointText = endpoint.toString();
        final String answerText = String.valueOf(o);
        System.out.format("endpoint = %s\n", endpoint);
        System.out.format("endpoint.answer() = %s\n", answerText);
        assertTrue(endpointText.contains("contextPath='" + ENDPOINT_CONTEXT_PATH + "'"));
        assertTrue(endpointText.contains("charset=UTF-8"));
        assertTrue(endpointText.contains("requestMethod=GET"));
        assertTrue(endpointText.contains("status=200"));
        assertTrue(answerText.contains("Hello, World"));
    }

    @org.junit.jupiter.api.Test
    public void testAnnotations() {
        System.out.println("\n\ntestAnnotations():");
        //noinspection SameReturnValue
        @SuppressWarnings("SameReturnValue") final Endpoint endpoint = new Endpoint() {

            @Override
            public Object answer() {
                return switch (requestMethod) {
                    case GET -> getSomeThing();
                    case PUT -> putSomeThing();
                    default -> {
                        setStatus(HttpServletResponse.SC_BAD_REQUEST);
                        yield null;
                    }
                };
            }

            @OperationId("getSomeThing")
            public JSONObject getSomeThing() {
                return new JSONObject("{\"test\": \"getSomeThing\"}");
            }

            @OperationId("putSomeThing")
            public JSONObject putSomeThing() {
                return new JSONObject("{\"test\": \"putSomeThing\"}");
            }

            @SuppressWarnings("unused")
            public String unannotatedMethod() {
                return String.valueOf(System.currentTimeMillis());
            }
        };

        Reflection.processAnnotation(endpoint, OperationId.class, new AnnotationHandler() {
            @Override
            public void process(Field field) {
                System.out.format("Discovered field: %s\n", field.getName());
            }

            @Override
            public void process(Method method) {
                System.out.format("Discovered method: %s\n", method.getName());
            }
        });
    }

    @org.junit.jupiter.api.Test
    public void registerYAMLEndpoints() {
        System.out.println("\n\nregisterYAMLEndpoints():");
        TestAPIServer testAPIServer = new TestAPIServer();
        testAPIServer.registerEndpoints();

        boolean exceptionCaught = false;
        try {
            // Duplicate test: Should just show a warning but not throw an exception.
            testAPIServer.addEndpoint(EndpointAlpha.class, "/public/api/1.0/alpha");
        } catch (Exception e) {
            exceptionCaught = true;
        } finally {
            assertFalse(exceptionCaught);
        }
    }

    public static class TestAPIServer extends APIServer {

        public TestAPIServer() {
            consoleLoggerMode = true;
            javaLoggerMode = false;
            servletContextEvent = new MockServletContextEvent();
        }

        @Override
        public void registerEndpoints() {
            YAMLContextPathFinder yamlContextPathFinder = new YAMLContextPathFinder(YAML_PATH);
            yamlContextPathFinder.setBaseContextPath("/public/api/1.0");
            registerPackage("dev.dmotservices.tests.api.endpoints", yamlContextPathFinder);
        }
    }

}
