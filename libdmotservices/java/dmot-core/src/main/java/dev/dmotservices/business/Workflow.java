package dev.dmotservices.business;

import dev.dmotservices.beans.Status;
import dev.dmotservices.interfaces.Task;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Workflow {

    protected static final Map<Long, Task> idTaskMap = new ConcurrentHashMap<>();

    protected List<Task> tasks = new ArrayList<>();

    public List<Task> getTasks() {
        return tasks;
    }

    public void setTasks(List<Task> tasks) {
        this.tasks = tasks;
    }

    @SuppressWarnings("UnusedReturnValue")
    public boolean addTask(Task task) {
        if (null == task)
            throw new IllegalArgumentException("A task must be provided.");
        idTaskMap.put(task.id(), task);
        return tasks.add(task);
    }

    public void removeTask(Long id) {
        idTaskMap.remove(id);
    }

    public int size() {
        return tasks.size();
    }

    public Task get(Long id) {
        return idTaskMap.get(id);
    }

    public List<Status> execute() {
        final List<Status> statuses = new ArrayList<>();
        for (Task task : tasks) {
            statuses.add(task.execute());
        }
        return statuses;
    }

}
