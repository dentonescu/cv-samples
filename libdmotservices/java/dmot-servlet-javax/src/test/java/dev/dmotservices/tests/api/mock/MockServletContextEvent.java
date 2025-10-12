package dev.dmotservices.tests.api.mock;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;

public class MockServletContextEvent extends ServletContextEvent {

    protected final ServletContext servletContext;

    public MockServletContextEvent() {
        this(new MockServletContext());
    }

    /**
     * Construct a ServletContextEvent from the given context.
     *
     * @param source - the ServletContext that is sending the event.
     */
    public MockServletContextEvent(ServletContext source) {
        super(source);
        this.servletContext = source;
    }
}
