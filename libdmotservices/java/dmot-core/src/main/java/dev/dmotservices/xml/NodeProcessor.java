package dev.dmotservices.xml;

import org.w3c.dom.Node;

public interface NodeProcessor<T> {
    T getResult();
    void process(Node node);
}
