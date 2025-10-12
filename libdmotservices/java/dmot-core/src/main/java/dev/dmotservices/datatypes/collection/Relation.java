package dev.dmotservices.datatypes.collection;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

/**
 * A simplistic representation of a relation and its tuples.
 */
public class Relation {

    protected final Map<Integer, Tuple> tuples = new HashMap<>();

    public void put(int row, Tuple tuple) {
        if (null == tuple || tuple.size() < 1)
            throw new IllegalArgumentException("An empty tuple cannot be stored.");
        tuples.put(row, tuple);
    }

    public Tuple get(int row) {
        return tuples.get(row);
    }

    public int size() {
        return tuples.size();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Relation relation = (Relation) o;
        return Objects.equals(tuples, relation.tuples);
    }

    @Override
    public int hashCode() {
        return Objects.hash(tuples);
    }

    @Override
    public String toString() {
        return "Relation{" +
                "tuples=" + tuples +
                '}';
    }
}
