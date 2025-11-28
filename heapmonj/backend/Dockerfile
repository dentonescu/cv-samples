# syntax=docker/dockerfile:1

## Build stage: compile Spring Boot app and dependencies
FROM eclipse-temurin:21-jdk AS build

WORKDIR /workspace
# Bring the whole repo so the Gradle hook can reach libdmotservices.
COPY . .
WORKDIR /workspace/heapmonj/backend/heapmonj

RUN apt-get update && apt-get install -y --no-install-recommends maven && \
    rm -rf /var/lib/apt/lists/*
RUN chmod +x ./gradlew
# Preinstall libdmotservices into the local Maven repo so Gradle can resolve it
RUN mvn -DskipTests -Dmaven.repo.local=/root/.m2/repository -f /workspace/libdmotservices/java/pom.xml install
# Build the fat jar (no daemon keeps the layer stable)
RUN ./gradlew clean bootJar --no-daemon

## Runtime stage: slim JRE with the application
FROM eclipse-temurin:21-jre

WORKDIR /opt/heapmonj
COPY --from=build /workspace/heapmonj/backend/heapmonj/build/libs/*.jar ./app.jar
# Ensure the data directory exists for the on-disk H2 database
RUN mkdir -p /opt/heapmonj/data

EXPOSE 8080
ENV JAVA_OPTS=""

ENTRYPOINT ["sh", "-c", "java $JAVA_OPTS -jar /opt/heapmonj/app.jar"]
