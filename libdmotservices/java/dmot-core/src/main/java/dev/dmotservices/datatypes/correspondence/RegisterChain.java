package dev.dmotservices.datatypes.correspondence;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class RegisterChain {

    protected final List<String> registerNames = new ArrayList<>();
    protected final List<Integer> registerPositions = new ArrayList<>();

    public void push(String name, Integer position) {
        pushName(name);
        pushPosition(position);
    }

    public void pushName(String name) {
        if (null == name || name.isBlank())
            throw new IllegalArgumentException("A valid register name must be supplied.");
        registerNames.add(name);
    }

    public void pushNames(String... names) {
        if (null == names)
            throw new IllegalArgumentException("No name parameters were supplied.");
        for (String s : names) {
            if (null != s && !s.isBlank())
                pushName(s);
        }
    }

    public void pushPosition(Integer position) {
        if (null == position)
            throw new IllegalArgumentException("A valid numeric position must be supplied.");
        registerPositions.add(position);
    }

    public void pushPositions(Integer... positions) {
        if (null == positions)
            throw new IllegalArgumentException("No position parameters were supplied.");
        for (Integer i : positions) {
            if (null != i)
                pushPosition(i);
        }
    }

    public String getRegisterName(int i) {
        return registerNames.get(i);
    }

    public Integer getRegisterPosition(int i) {
        return registerPositions.get(i);
    }

    public int size() {
        return registerNames.size();
    }

    public String toPrettyString() {
        StringBuilder sb = new StringBuilder();
        String spacer = "";
        for (int i = 0; i < registerNames.size(); ++i) {
            sb.append(getRegisterName(i));
            if (i < registerPositions.size())
                sb.append(" (").append(getRegisterPosition(i)).append(")");
            if (i < registerNames.size() - 1) {
                sb.append("\n").append(spacer).append("|-- ");
                spacer += "    ";
            }
        }
        return sb.toString();
    }

    /*
     * generated
     */

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        RegisterChain that = (RegisterChain) o;
        return Objects.equals(registerNames, that.registerNames) && Objects.equals(registerPositions, that.registerPositions);
    }

    @Override
    public int hashCode() {
        return Objects.hash(registerNames, registerPositions);
    }

    @Override
    public String toString() {
        return "RegisterChain{" +
                "registerNames=" + registerNames +
                ", registerPositions=" + registerPositions +
                '}';
    }
}
