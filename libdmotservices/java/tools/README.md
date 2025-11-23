# Version tools

`VersionManager.java` stamps Maven `pom.xml` files from their `.tpl` templates using the version in `../VERSION` (the `libdmotservices` root). It is invoked automatically by Ant/Make/CMake before builds; you can also run it manually for troubleshooting:

```bash
cd libdmotservices/java/tools
java VersionManager.java ../..   # libdmotservices/ as base; defaults to ../../ if omitted
```

If the generated POM content matches what is already on disk, the tool leaves the file untouched to avoid noisy diffs. Ensure a Java 11+ runtime is available on `PATH` (the builds use `${JAVA_HOME}/bin/java`).
