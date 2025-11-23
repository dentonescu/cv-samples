<!-- WARNING: Do not manually edit this pom.xml. It will be overwritten by a preprocessed pom.xml.tpl.-->
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>dev.dmotservices</groupId>
    <artifactId>libdmotservices-parent</artifactId>
    <version>@@VERSION@@</version>
    <packaging>pom</packaging>

    <name>${project.groupId}:${project.artifactId}</name>
    <description>Parent POM for libdmotservices Java modules.</description>
    <url>https://gitlab.com/dmotservices</url>

    <modules>
        <module>dmot-core</module>
        <module>dmot-servlet-javax</module>
    </modules>

    <properties>
        <revision>@@VERSION@@</revision>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <java.version>17</java.version>
        <maven.compiler.source>${java.version}</maven.compiler.source>
        <maven.compiler.target>${java.version}</maven.compiler.target>
        <skipTests>false</skipTests>
    </properties>
</project>
