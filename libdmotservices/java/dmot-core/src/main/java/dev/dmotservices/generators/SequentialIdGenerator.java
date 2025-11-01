package dev.dmotservices.generators;

import java.util.concurrent.atomic.AtomicLong;

public class SequentialIdGenerator {

    protected static AtomicLong id;

    public static void init(long startValue) {
        id = new AtomicLong(startValue);
    }

    public static long nextNumeric() {
        if (null == id)
            init(0);
        return id.incrementAndGet();
    }

}
