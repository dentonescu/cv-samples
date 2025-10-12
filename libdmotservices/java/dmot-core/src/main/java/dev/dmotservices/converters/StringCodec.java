package dev.dmotservices.converters;

public interface StringCodec {
    // see: https://docs.oracle.com/javase/8/docs/technotes/guides/intl/encoding.doc.html
    String process(String input);

}
