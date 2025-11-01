package dev.dmotservices.interfaces.impl;

import dev.dmotservices.interfaces.ContentReader;

import java.util.Base64;

public class Base64ContentReader implements ContentReader {

    protected final byte[] originalRepresentation;

    public Base64ContentReader(byte[] originalRepresentation) {
        this.originalRepresentation = originalRepresentation.clone();
    }

    public Base64ContentReader(String originalRepresentation) {
        this.originalRepresentation = originalRepresentation.getBytes();
    }

    @Override
    public byte[] getContent() {
        return Base64.getDecoder().decode(originalRepresentation);
    }

    @Override
    public byte[] getOriginalRepresentation() {
        return originalRepresentation;
    }
}
