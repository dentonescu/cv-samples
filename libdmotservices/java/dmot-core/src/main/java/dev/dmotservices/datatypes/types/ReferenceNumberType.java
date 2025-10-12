package dev.dmotservices.datatypes.types;

public enum ReferenceNumberType {
    DOCUMENT("DOCUMENT"),
    EMAIL("EMAIL"),
    EMPLOYEE("EMPLOYEE");

    private final String identifier;

    ReferenceNumberType(String identifier) {
        this.identifier = identifier;
    }

    public static ReferenceNumberType fromColumnName(String columnName) {
        if (null == columnName || columnName.isBlank())
            return null;
        String columnNameTrimmed = columnName.trim();
        for (ReferenceNumberType referenceNumber : ReferenceNumberType.values()) {
            if (referenceNumber.getIdentifier().equalsIgnoreCase(columnNameTrimmed)) {
                return referenceNumber;
            }
        }
        return null;
    }

    /*
     * generated
     */

    public String getIdentifier() {
        return identifier;
    }

    @Override
    public String toString() {
        return "ReferenceNumberType{" +
                "identifier='" + identifier + '\'' +
                '}';
    }
}
