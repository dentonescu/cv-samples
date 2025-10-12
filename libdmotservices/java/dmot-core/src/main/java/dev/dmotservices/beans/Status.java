package dev.dmotservices.beans;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Objects;

public class Status {
    protected int successes = 0;
    protected int failures = 0;
    protected Object statusObject;
    protected StringBuilder messageBuilder = new StringBuilder();

    public String getMessage() {
        return messageBuilder.toString();
    }

    public void setMessage(String message) {
        messageBuilder = new StringBuilder(message);
    }

    public void appendMessage(String message) {
        messageBuilder.append(message);
    }

    public void appendTimestampedMessage(String message) {
        final LocalDateTime now = LocalDateTime.now();
        final DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        final String formattedTimestamp = now.format(formatter);
        messageBuilder.append("[").append(formattedTimestamp).append("] ").append(message);
    }

    public void incrementSuccesses() {
        ++successes;
    }

    public void incrementFailures() {
        ++failures;
    }

    /*
     * generated
     */

    public int getSuccesses() {
        return successes;
    }

    public void setSuccesses(int successes) {
        this.successes = successes;
    }

    public int getFailures() {
        return failures;
    }

    public void setFailures(int failures) {
        this.failures = failures;
    }

    public Object getStatusObject() {
        return statusObject;
    }

    public void setStatusObject(Object statusObject) {
        this.statusObject = statusObject;
    }

    public StringBuilder getMessageBuilder() {
        return messageBuilder;
    }

    public void setMessageBuilder(StringBuilder messageBuilder) {
        this.messageBuilder = messageBuilder;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Status status = (Status) o;
        return successes == status.successes && failures == status.failures && Objects.equals(statusObject, status.statusObject);
    }

    @Override
    public int hashCode() {
        return Objects.hash(successes, failures, statusObject);
    }

    @Override
    public String toString() {
        return "Status{" +
                "successes=" + successes +
                ", failures=" + failures +
                ", statusObject=" + statusObject +
                ", messageBuilder=" + messageBuilder +
                '}';
    }
}
