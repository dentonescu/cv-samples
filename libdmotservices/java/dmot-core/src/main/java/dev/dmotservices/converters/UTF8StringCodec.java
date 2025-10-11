package dev.dmotservices.converters;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class UTF8StringCodec implements StringCodec {
    protected String encodingAsString;

    public UTF8StringCodec() {
        this.encodingAsString = Encoding.IBM850.getCodePage();
    }

    public UTF8StringCodec(Encoding encoding) {
        if (null == encoding)
            throw new IllegalArgumentException("The encoding cannot be empty.");
        this.encodingAsString = encoding.getCodePage();
    }

    public UTF8StringCodec(String customEncoding) {
        if (null == customEncoding || customEncoding.isEmpty())
            throw new IllegalArgumentException("A value must be supplied for the encoding.");
        this.encodingAsString = customEncoding;
    }

    @Override
    public String process(String input) {
        if (null == input)
            return null;

        byte[] bytesProcessed = new byte[0];
        final String converted;
        try {
            bytesProcessed = input.getBytes(encodingAsString);
            converted = new String(bytesProcessed, StandardCharsets.UTF_8);
        } catch (IOException e) {
            throw new IllegalStateException("The supplied string could not be converted from " + encodingAsString +
                    " to UTF-8. bytesProcessed =  " + Arrays.toString(bytesProcessed) + " , input: " + input);
        }

        return converted;
    }

    public Encoding getEncoding() {
        return Encoding.fromString(encodingAsString);
    }

    public void setEncoding(Encoding encoding) {
        if (null == encoding)
            throw new IllegalArgumentException("An encoding must be supplied.");
        this.encodingAsString = encoding.getCodePage();
    }

    @Override
    public String toString() {
        return "IBMtoUTF8StringCodecImpl{" +
                "encodingAsString=" + encodingAsString +
                '}';
    }
}
