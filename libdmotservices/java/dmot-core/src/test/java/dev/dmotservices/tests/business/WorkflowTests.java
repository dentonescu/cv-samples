package dev.dmotservices.tests.business;

import dev.dmotservices.beans.Status;
import dev.dmotservices.business.GenericProcess;
import dev.dmotservices.business.Workflow;
import dev.dmotservices.generators.SequentialIdGenerator;
import dev.dmotservices.interfaces.Task;
import static org.junit.jupiter.api.Assertions.*;


import java.util.Date;
import java.util.Random;

public class WorkflowTests {

    protected static class DummyTask implements Task {

        final long id;
        final Date date;

        public DummyTask() {
            id = SequentialIdGenerator.nextNumeric();
            date = new Date();
        }

        @Override
        public Long id() {
            return id;
        }

        @Override
        public Status execute() {
            final Status status = new Status();
            status.setSuccesses(new Random().nextInt(100) + 1);
            status.setFailures(100 - status.getSuccesses());
            status.setStatusObject(date);
            status.setMessage(String.format("Task id: %d", id()));
            return status;
        }

        @Override
        public String toString() {
            return "DummyTask{" +
                    "date=" + date +
                    '}';
        }
    }

    @org.junit.jupiter.api.Test
    public void testWorkflow() {
        System.out.println("\n\ntestWorkflow():");
        final Workflow workflow = new Workflow();
        for (int i = 0; i < 5; ++i) {
            final DummyTask dummyTask = new DummyTask();
            System.out.format("Adding dummy task to workflow: %s\n", dummyTask);
            assertNotNull(dummyTask);
            workflow.addTask(dummyTask);
        }
        final Status status = GenericProcess.execute(workflow);
        assertNotNull(status);
    }

}
