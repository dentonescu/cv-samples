package dev.dmotservices.io;

import java.io.*;
import java.net.URL;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.logging.Level;
import java.util.logging.Logger;

public class FileUtils {

    protected static final Logger logger = Logger.getLogger(FileUtils.class.getName());

    public static String determinePathInFileSystemOrJar(java.lang.Class<?> classObj, String filename) {
        StringBuilder sbPaths = new StringBuilder();
        Path pathUnmodified, pathAbs, pathSrcMain, pathSrcTest;

        try {
            pathUnmodified = Paths.get(filename);
            sbPaths.append("\t").append(pathUnmodified).append("\n");
        } catch (InvalidPathException e) {
            pathUnmodified = null;
        }

        try {
            pathAbs = Paths.get(System.getProperty("user.dir") + "/" + filename);
            sbPaths.append("\t").append(pathAbs).append("\n");
        } catch (InvalidPathException e) {
            pathAbs = null;
        }

        try {
            pathSrcMain = Paths.get("src/main/resources/" + filename);
            sbPaths.append("\t").append(classObj.getName()).append(":")
                    .append(pathSrcMain).append("\n");
        } catch (InvalidPathException e) {
            pathSrcMain = null;
        }

        try {
            pathSrcTest = Paths.get("src/test/resources/" + filename);
            sbPaths.append("\t").append(classObj.getName()).append(":")
                    .append(pathSrcTest).append("\n");
        } catch (InvalidPathException e) {
            pathSrcTest = null;
        }

        if (null != pathUnmodified && Files.exists(pathUnmodified))
            return pathUnmodified.toString();
        if (null != pathAbs && Files.exists(pathAbs))
            return pathAbs.toString();
        else if (null != pathSrcMain && Files.exists(pathSrcMain))
            return pathSrcMain.toString();
        else if (null != pathSrcTest && Files.exists(pathSrcTest))
            return pathSrcTest.toString();

        logger.log(Level.FINE, String.format("Could not locate the file at any of the following paths: %s\n", sbPaths));
        return null;
    }

    // resource as string
    public static String loadResourceAsString(java.lang.Class<?> classObj, String filename) throws IOException {
        return loadResourceAsString(classObj, filename, StandardCharsets.UTF_8);
    }

    public static String loadResourceAsString(java.lang.Class<?> classObj, String filename, Charset charset) throws IOException {
        if (filename.contains("://")) // TODO: Improve this matching condition.
            return loadNetworkResourceAsString(classObj, filename, charset);
        return loadLocalResourceAsString(classObj, filename, charset);
    }

    private static void loadResourceAsStringValidation(java.lang.Class<?> classObj, String filename, Charset charset) {
        if (null == classObj)
            throw new IllegalArgumentException("A class object must be provided.");
        if (null == filename || filename.isEmpty())
            throw new IllegalArgumentException("The filename argument was not supplied.");
        if (null == charset)
            throw new IllegalArgumentException("A character set must be specified.");
    }

    /**
     * Loads a file at the URL specified.
     */
    public static String loadNetworkResourceAsString(java.lang.Class<?> classObj, String url, Charset charset) throws IOException {
        loadResourceAsStringValidation(classObj, url, charset);
        try (InputStream in = new URL(url).openStream()) {
            return new String(in.readAllBytes(), charset);
        }
    }

    /**
     * Loads a file from the file system. If it does not exist there, an attempt is made to load the file from the JAR.
     */
    public static String loadLocalResourceAsString(java.lang.Class<?> classObj, String filename, Charset charset)
            throws IOException {
        return loadLocalResourceAsString(classObj, filename, charset, false);
    }

    /**
     * Loads a file from the file system. If it does not exist there, an attempt is made to load the file from the JAR.
     */
    public static String loadLocalResourceAsString(java.lang.Class<?> classObj, String filename, Charset charset,
                                                   boolean readFromJarOnly)
            throws IOException {
        loadResourceAsStringValidation(classObj, filename, charset);
        StringBuilder sb;
        String body;
        String filePath = null;
        if (!readFromJarOnly)
            filePath = determinePathInFileSystemOrJar(classObj, filename);
        try (InputStream in = (readFromJarOnly || null == filePath) ?
                classObj.getClassLoader().getResourceAsStream(filename) // load from the JAR
                :
                new FileInputStream(filePath)   // load from the file system
        ) {
            if (null == in)
                throw new FileNotFoundException(String.format("The resource at %s could not be loaded.", filename));
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(in, charset))) {
                sb = new StringBuilder();
                String line;
                while (null != (line = reader.readLine())) {
                    sb.append(line).append("\n");
                }
                body = sb.toString();
            } catch (NullPointerException | IOException e) {
                throw new IllegalStateException(String.format("Could not read file: %s\n%s",
                        filePath, e));
            }
        } catch (FileNotFoundException | SecurityException e) {
            throw new IllegalStateException(String.format("Could not locate %s. Current path: %s\n%s",
                    filename, System.getProperty("user.dir"), e));
        }
        return body;
    }
}
