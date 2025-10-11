package dev.dmotservices.datatypes.organization;

public enum DepartmentLevel {
    COMPANY(1, "COMPANY"),
    DIVISION(2, "DIVISION"),
    DEPARTMENT(3, "DEPARTMENT"),
    SUBDEPARTMENT(4, "SUBDEPARTMENT"),
    TEAM(5, "TEAM"),
    COMMITTEE(6, "COMMITTEE"),
    SUBCOMMITTEE(7, "SUBCOMMITTEE"),
    GROUP(8, "GROUP"),
    SUBGROUP(9, "SUBGROUP");

    private final int level;
    private final String identifier;

    DepartmentLevel(int level, String identifier) {
        this.level = level;
        this.identifier = identifier;
    }

    /*
     * generated
     */

    public int getLevel() {
        return level;
    }

    public String getIdentifier() {
        return identifier;
    }

    @Override
    public String toString() {
        return "DepartmentLevel{" +
                "level=" + level +
                ", identifier='" + identifier + '\'' +
                '}';
    }
}
