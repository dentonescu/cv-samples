import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Simple helper to stamp POM templates with the current VERSION value.
 */
public class VersionManager {

    private static final Charset DEFAULT_ENCODING = StandardCharsets.UTF_8;
    private static final String PLACEHOLDER = "@@VERSION@@";

    private static final String[] POMS = {
            "pom.xml",
            Paths.get("dmot-core", "pom.xml").toString(),
            Paths.get("dmot-servlet-javax", "pom.xml").toString()
    };

    private static String getVersionString(Path libHome) {
        Path versionFile = libHome.resolve("VERSION");
        String version = "0.0.0";
        try {
            version = Files.readString(versionFile, DEFAULT_ENCODING).trim();
        } catch (IOException e) {
            System.err.format("Failed to read %s. Using the default value: %s%n", versionFile, version);
        }
        return version;
    }

    private static void updateVersion(Path libHome, String filename, String newVersion) {
        Path baseDir = libHome.resolve("java");
        Path pathTpl = baseDir.resolve(filename + ".tpl");
        Path pathPom = baseDir.resolve(filename);

        try {
            String template = Files.readString(pathTpl, DEFAULT_ENCODING);
            String updated = template.replace(PLACEHOLDER, newVersion);

            String current = Files.exists(pathPom) ? Files.readString(pathPom, DEFAULT_ENCODING) : "";
            if (updated.equals(current)) {
                System.out.format("No change for %s%n", pathPom);
                return;
            }

            Files.writeString(pathPom, updated, DEFAULT_ENCODING);
            System.out.format("Updated %s to version %s%n", pathPom, newVersion);
        } catch (IOException e) {
            System.err.format("Failed to update the version in: %s (%s)%n", pathPom, e.getMessage());
        }
    }

    private static Path resolveLibHome(String[] args) {
        if (args.length > 0) {
            return Paths.get(args[0]).toAbsolutePath().normalize();
        }
        // Default: invoked from libdmotservices/java/tools → go up two levels
        Path cwd = Paths.get("").toAbsolutePath().normalize();
        return cwd.getParent() != null && cwd.getParent().getParent() != null
                ? cwd.getParent().getParent()
                : cwd;
    }

    public static void main(String[] args) {
        Path libHome = resolveLibHome(args);
        String version = getVersionString(libHome);
        System.out.format("VERSION: %s%n", version);
        System.out.format("library home: %s%n", libHome);
        for (String pom : POMS) {
            updateVersion(libHome, pom, version);
        }
    }
}
