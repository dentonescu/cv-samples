package dev.dmotservices.datatypes.collection;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

/**
 * A simplistic representation of a tuple and its attributes.
 */
public class Tuple {

    protected final Map<String, Object> attributes = new HashMap<>();

    public void put(String attribute, Object value) {
        if (null == attribute || attribute.isBlank())
            throw new IllegalArgumentException("The attribute to be stored cannot be empty.");
        attributes.put(attribute, value);
    }

    public Object get(String attribute) {
        return attributes.get(attribute);
    }

    public Set<String> getAttributes() {
        return attributes.keySet();
    }

    public int size() {
        return attributes.size();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Tuple tuple = (Tuple) o;
        return Objects.equals(attributes, tuple.attributes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(attributes);
    }

    @Override
    public String toString() {
        return "Tuple{" +
                "attributes=" + attributes +
                '}';
    }
}
