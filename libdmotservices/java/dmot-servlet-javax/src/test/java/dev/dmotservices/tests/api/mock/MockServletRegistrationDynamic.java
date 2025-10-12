package dev.dmotservices.tests.api.mock;

import javax.servlet.MultipartConfigElement;
import javax.servlet.ServletRegistration;
import javax.servlet.ServletSecurityElement;
import java.util.*;

public class MockServletRegistrationDynamic implements ServletRegistration.Dynamic {

    final Set<String> urlPatterns = new HashSet<>();

    @Override
    public void setLoadOnStartup(int loadOnStartup) {

    }

    @Override
    public Set<String> setServletSecurity(ServletSecurityElement constraint) {
        return null;
    }

    @Override
    public void setMultipartConfig(MultipartConfigElement multipartConfig) {

    }

    @Override
    public void setRunAsRole(String roleName) {

    }

    @Override
    public void setAsyncSupported(boolean isAsyncSupported) {

    }

    @Override
    public Set<String> addMapping(String... urlPatterns) {
        this.urlPatterns.addAll(Arrays.asList(urlPatterns));
        return this.urlPatterns;
    }

    @Override
    public Collection<String> getMappings() {
        return null;
    }

    @Override
    public String getRunAsRole() {
        return null;
    }

    @Override
    public String getName() {
        return null;
    }

    @Override
    public String getClassName() {
        return null;
    }

    @Override
    public boolean setInitParameter(String name, String value) {
        return false;
    }

    @Override
    public String getInitParameter(String name) {
        return null;
    }

    @Override
    public Set<String> setInitParameters(Map<String, String> initParameters) {
        return null;
    }

    @Override
    public Map<String, String> getInitParameters() {
        return null;
    }
}
