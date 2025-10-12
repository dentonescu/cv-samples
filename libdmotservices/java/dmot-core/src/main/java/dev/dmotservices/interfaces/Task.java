package dev.dmotservices.interfaces;

import dev.dmotservices.beans.Status;

public interface Task {

    Long id();

    Status execute();

}
