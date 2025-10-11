package dev.dmotservices.api.yaml;

import dev.dmotservices.api.ContextPathFinder;
import dev.dmotservices.io.FileUtils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class YAMLContextPathFinder implements ContextPathFinder {

    public static final String REGEX_OPERATION_ID = "operationId:\\s*([\\w]+)\\s*";
    public static final String REGEX_SERVER_BASE_URL = "servers:\\s+-\\s+url:\\s*\"([^\"]*)\"";
    public static final String REGEX_CONTEXT_PATH = "\\s+(/[^:]+):\\s*";

    protected String yaml;
    protected String baseURL;
    protected String baseContextPath = "";
    protected final List<String> contextPathList = new ArrayList<>();
    protected final List<String> operationIdList = new ArrayList<>();
    protected final Map<String, String> operationIdContextPathMap = new HashMap<>();

    public String getBaseContextPath() {
        return baseContextPath;
    }

    public void setBaseContextPath(String baseContextPath) {
        this.baseContextPath = baseContextPath;
    }

    public YAMLContextPathFinder(String yamlFilePath) {
        try {
            this.yaml = FileUtils.loadResourceAsString(getClass(), yamlFilePath);
        } catch (IOException e) {
            throw new IllegalStateException(String.format("Could not start the YAML context path finder as the YAML file could not be loaded: %s %s", yamlFilePath, e));
        }
        if (null == yaml || yaml.isEmpty())
            throw new IllegalStateException(String.format("The YAML file appears to be empty: %s", yamlFilePath));
        populate();
        if (contextPathList.isEmpty() || operationIdList.isEmpty() || operationIdContextPathMap.isEmpty())
            throw new IllegalStateException(String.format("The collections cannot be empty: contextPathList=%s, operationIdList=%s, operationIdContextPathMap=%s",
                    contextPathList, operationIdList, operationIdContextPathMap));
    }

    protected void populate() {
        fillBaseURL();
        fillOperationsAndPaths();
    }

    protected void fillBaseURL() {
        Pattern pattern = Pattern.compile(REGEX_SERVER_BASE_URL);
        Matcher matcher = pattern.matcher(yaml);
        String baseUrl = null;
        if (matcher.find() && matcher.groupCount() >= 1) {
            baseUrl = matcher.group(1);
        }
        this.baseURL = baseUrl;
    }

    protected void fillOperationsAndPaths() {
        int iteration = 0;
        String operationId = null;
        String path = null;
        for (String chunk : yaml.replace("paths:", "operationId:").split("operationId:")) {
            chunk = "operationId:" + chunk;
            Pattern patternOperationId = Pattern.compile(REGEX_OPERATION_ID);
            Matcher matcherOperationId = patternOperationId.matcher(chunk);
            if (matcherOperationId.find()) {
                operationId = matcherOperationId.group(1);
            }
            if (iteration >= 1) {
                if (null != path && operationId != null) {
                    operationIdList.add(operationId);
                    contextPathList.add(path);
                    operationIdContextPathMap.put(operationId, path);
                }
                Pattern patternContextPath = Pattern.compile(REGEX_CONTEXT_PATH);
                Matcher matcherContextPath = patternContextPath.matcher(chunk);
                if (matcherContextPath.find()) {
                    path = matcherContextPath.group(1);
                }
            }
            ++iteration;
        }
    }

    @Override
    public String findContextPathOfMethod(String methodName) {
        if (null == methodName)
            return null;
        String trimmed = methodName.trim();
        if (!operationIdList.contains(trimmed))
            return null;
        final String contextPath = operationIdContextPathMap.get(trimmed);
        StringBuilder contextPathBuilder = new StringBuilder();
        if (baseContextPath.length() > 1) {
            contextPathBuilder.append(baseContextPath);
            if (!contextPath.startsWith("/"))
                contextPathBuilder.append("/");
        }
        contextPathBuilder.append(contextPath);
        return contextPathBuilder.toString();
    }

    @Override
    public String toString() {
        return "YAMLContextPathFinder{" +
                "yaml='" + yaml + '\'' +
                ", baseURL='" + baseURL + '\'' +
                ", baseContextPath='" + baseContextPath + '\'' +
                ", contextPathList=" + contextPathList +
                ", operationIdList=" + operationIdList +
                ", operationIdContextPathMap=" + operationIdContextPathMap +
                '}';
    }
}
