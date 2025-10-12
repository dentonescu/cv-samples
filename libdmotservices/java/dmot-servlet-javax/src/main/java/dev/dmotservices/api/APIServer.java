package dev.dmotservices.api;

import dev.dmotservices.api.annotations.OperationId;
import dev.dmotservices.io.ClassUtils;
import dev.dmotservices.reflection.Reflection;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.ServletRegistration.Dynamic;
import javax.servlet.http.HttpServlet;
import java.lang.annotation.Annotation;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

public abstract class APIServer implements ServletContextListener, EndpointRegistrar {

    protected Logger logger;
    protected ServletContextEvent servletContextEvent;
    protected boolean consoleLoggerMode = false;
    protected boolean javaLoggerMode = true;
    protected final List<java.lang.Class<? extends HttpServlet>> registeredEndpoints = new ArrayList<>();


    @SuppressWarnings("MismatchedQueryAndUpdateOfCollection")
    private final Map<String, ServletContext> nameServletContextMap = new ConcurrentHashMap<>();

    public APIServer() {
        logger = Logger.getLogger(getClass().getName());
    }

    public void setLogger(Logger logger) {
        this.logger = logger;
    }

    public void registerPackage(String packageName, ContextPathFinder contextPathFinder) {
        if (null == packageName)
            throw new IllegalArgumentException("The package name cannot be empty.");
        final Set<java.lang.Class<?>> classSet = ClassUtils.findAllClassesInPackage(packageName);
        registerPackage(classSet, contextPathFinder);
    }

    public void registerPackage(Set<java.lang.Class<?>> classSet, ContextPathFinder contextPathFinder) {
        multiLogger(Level.FINE, String.format("registerPackage() has started. classSet=%s, contextPathFinder=%s",
                classSet, contextPathFinder));
        if (null == classSet)
            throw new IllegalArgumentException("The class set cannot be empty.");
        if (null == contextPathFinder)
            throw new IllegalArgumentException("A ContextPathFinder object is required.");
        try {
            for (java.lang.Class<?> clazz : classSet) {
                Annotation[] annotations = Reflection.getCombinedAnnotations(clazz);
                multiLogger(Level.FINE, String.format("Scanning for OperationId annotation in %d annotations.", annotations.length));
                for (Annotation annotation : annotations) {
                    if (annotation.annotationType().equals(OperationId.class)) {
                        final OperationId operationId = (OperationId) annotation;
                        final String contextPath = contextPathFinder.findContextPathOfMethod(operationId.value());
                        multiLogger(Level.INFO, String.format("Registering %s to class %s ...", contextPath, clazz.getName()));
                        //noinspection unchecked
                        addEndpoint((java.lang.Class<? extends HttpServlet>) clazz, contextPath);
                    }
                }
            }
        } finally {
            multiLogger(Level.FINE, String.format("registerPackage() has ended. classSet=%s", classSet));
        }
    }

    @Override
    public void contextInitialized(ServletContextEvent servletContextEvent) {
        multiLogger(Level.FINE, "contextInitialized() has started.");
        this.servletContextEvent = servletContextEvent;
        registerEndpoints();
        multiLogger(Level.FINE, "contextInitialized() has ended.");
    }

    public void addEndpoint(java.lang.Class<? extends HttpServlet> clazz, String contextPath) {
        multiLogger(Level.FINE, String.format("addEndpoint() has started. clazz=%s, contextPath=%s",
                clazz, contextPath));
        if (null == clazz || null == contextPath)
            throw new IllegalArgumentException(String.format("Empty parameters are not allowed. clazz=%s, contextPath=%s",
                    clazz, contextPath));
        final String simpleName = clazz.getSimpleName();
        if (null == servletContextEvent) {
            multiLogger(Level.SEVERE, String.format("Could not register the endpoint %s to context path %s " +
                    "because a servlet context event was not found.", simpleName, contextPath));
            return;
        }
        if (registeredEndpoints.contains(clazz)) {
            multiLogger(Level.WARNING, String.format("Skipped adding this endpoint, %s, because it has already been registered.\n", clazz.getName()));
            return;
        }
        ServletContext context = servletContextEvent.getServletContext();
        if (null == context)
            throw new IllegalStateException("Failed to obtain a ServletContext object.");
        multiLogger(Level.FINE, String.format("real context path = %s", context.getRealPath(contextPath)));
        nameServletContextMap.put(simpleName, context);
        Dynamic dynamic = context.addServlet(simpleName, clazz);
        dynamic.addMapping(contextPath);
        registeredEndpoints.add(clazz);
        multiLogger(Level.FINE, String.format("Added the endpoint %s having context path %s. ", simpleName, contextPath));
        multiLogger(Level.FINE, "addEndpoint() has ended.");
    }

    @Override
    public void contextDestroyed(ServletContextEvent servletContextEvent) {
        multiLogger(Level.FINE, "contextDestroyed() has started.");
        nameServletContextMap.clear();
        multiLogger(Level.FINE, "contextDestroyed() has ended.");
    }

    protected void multiLogger(Level level, String message) {
        if (javaLoggerMode)
            logger.log(level, message);
        if (consoleLoggerMode)
            consoleLogger(level, message);
    }

    protected void consoleLogger(Level level, String message) {
        if (level == Level.SEVERE)
            System.err.println(message);
        else
            System.out.println(message);
    }
}
