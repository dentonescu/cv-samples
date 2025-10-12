package dev.dmotservices.tests.collection;

import dev.dmotservices.datatypes.collection.Relation;
import dev.dmotservices.datatypes.collection.Tuple;
import dev.dmotservices.datatypes.correspondence.RegisterChain;
import static org.junit.jupiter.api.Assertions.*;


import java.util.Set;

public class CollectionTests {

    @org.junit.jupiter.api.Test
    public void testRelationAndTuples() {
        System.out.println("\n\ntestRelationAndTuples():");
        final Tuple tuple1 = new Tuple();
        final Tuple tuple2 = new Tuple();
        final Tuple tuple3 = new Tuple();
        tuple1.put("employeeNumber", "000345");
        tuple1.put("firstName", "Fred");
        tuple1.put("lastName", "Flintstone");
        tuple2.put("employeeNumber", "010101");
        tuple2.put("firstName", "Peter");
        tuple2.put("lastName", "Parker");
        tuple2.put("email", "spiderman@spider.co.jp");
        tuple3.put("employeeNumber", "123456");
        tuple3.put("firstName", "Helena");
        tuple3.put("lastName", "Adam");
        tuple3.put("username", "hadam");
        final Relation relation = new Relation();
        relation.put(1, tuple1);
        relation.put(2, tuple2);
        relation.put(3, tuple3);
        System.out.format("relation = %s\n", relation);
        System.out.format("* tuple1 = %s\n", tuple1);
        System.out.format("* tuple2 = %s\n", tuple2);
        System.out.format("* tuple3 = %s\n", tuple3);
        assertEquals(3, relation.size());
        assertEquals(3, tuple1.size());
        assertEquals(4, tuple2.size());
        assertEquals(4, tuple3.size());
    }

    @org.junit.jupiter.api.Test
    public void testTupleAttributes() {
        System.out.println("\n\ntestTupleAttributes():");
        final Tuple tuple = new Tuple();
        tuple.put("city", "Heidenheim");
        tuple.put("state", "Baden-Württemberg");
        tuple.put("country", "Germany");
        final Set<String> attributeSet = tuple.getAttributes();
        System.out.format("tuple=%s\n", tuple);
        System.out.format("attribute set=%s\n", attributeSet);
        assertEquals(3, tuple.size());
        assertEquals(3, attributeSet.size());
    }

    @org.junit.jupiter.api.Test
    public void testRegisterChain() {
        System.out.println("\n\ntestRegisterChain():");
        final RegisterChain registerChain = new RegisterChain();
        registerChain.push("Library", 5);
        registerChain.push("Digital", 2);
        registerChain.push("Audio", 1);
        registerChain.pushNames("Electronic", "Synthwave", "New Retro Wave");
        registerChain.pushPositions(3, 6);

        System.out.format("registerChain=%s\n\n", registerChain);
        System.out.println(registerChain.toPrettyString());
        assertEquals(6, registerChain.size());
    }

}
