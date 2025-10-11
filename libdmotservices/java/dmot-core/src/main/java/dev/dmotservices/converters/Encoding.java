package dev.dmotservices.converters;

public enum Encoding {
    IBM437("IBM437"),
    IBM850("IBM850"),
    UTF8("UTF-8");

    private final String codePage;

    Encoding(String getCodePage) {
        this.codePage = getCodePage;
    }

    public String getCodePage() {
        return codePage;
    }

    public static Encoding fromString(String text) {
        for (Encoding encoding : Encoding.values()) {
            if (encoding.codePage.equalsIgnoreCase(text)) {
                return encoding;
            }
        }
        return null;
    }
}
