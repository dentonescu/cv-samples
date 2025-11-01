package dev.dmotservices.business;

import dev.dmotservices.beans.Status;

import java.util.ArrayList;
import java.util.List;

public class GenericProcess {

    public static Status execute(Workflow workflow) {
        final Status processStatus = new Status();
        final List<Object> objectList = new ArrayList<>();
        final StringBuilder messageBuilder = new StringBuilder("\n");
        final long startTime = System.nanoTime();
        for (Status status : workflow.execute()) {
            processStatus.setSuccesses(processStatus.getSuccesses() + status.getSuccesses());
            processStatus.setFailures(processStatus.getFailures() + status.getFailures());
            objectList.add(status.getStatusObject());
            messageBuilder.append(status.getMessage()).append("\n");
        }
        final long endTime = System.nanoTime();
        final long elapsedTimeInSeconds = (endTime - startTime) / 1_000_000_000;
        final long hours = elapsedTimeInSeconds / 3600;
        final long minutes = (elapsedTimeInSeconds % 3600) / 60;
        final long seconds = elapsedTimeInSeconds % 60;
        processStatus.setStatusObject(objectList);
        processStatus.setMessage(messageBuilder.toString());
        processStatus.appendTimestampedMessage(
                String.format("(PROCESS DURATION) Elapsed time: %02d:%02d:%02d\n",
                        hours, minutes, seconds));
        return processStatus;
    }

}