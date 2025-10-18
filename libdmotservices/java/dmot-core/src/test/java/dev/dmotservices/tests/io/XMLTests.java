package dev.dmotservices.tests.io;

import dev.dmotservices.io.FileUtils;
import dev.dmotservices.xml.DOMParser;
import dev.dmotservices.xml.NodeProcessor;
import static org.junit.jupiter.api.Assertions.*;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.List;

public class XMLTests {

    public static final String TEST_RESOURCE = "xml/example1.xml";

    @org.junit.jupiter.api.Test
    public void testNodeProcessor() throws IOException, ParserConfigurationException, SAXException {
        System.out.println("\n\ntestNodeProcessor():");
        final String xml = FileUtils.loadResourceAsString(getClass(), TEST_RESOURCE);
        NodeProcessor<String[]> nodeProcessor = new NodeProcessor<>() {
            private String[] result;

            @Override
            public String[] getResult() {
                return result;
            }

            @Override
            public void process(Node node) {
                final String id = DOMParser.fetchFromNodeUsingLeftTraversal(node, "id");
                final String countryName = DOMParser.fetchFromNodeUsingLeftTraversal(node, "countryName");
                final String provinceName = DOMParser.fetchFromNodeUsingLeftTraversal(node, "provinces", "province", "name");
                result = new String[]{id, countryName, provinceName};
            }
        };
        DOMParser<String[]> domParser = new DOMParser<>(xml, nodeProcessor, StandardCharsets.UTF_8);
        domParser.setOuterNodeName("countries");
        domParser.parse();
        List<String[]> result = domParser.getParsedResultList();
        for (String[] sArr : result) {
            System.out.format("%s\n", String.join(",", sArr));
        }
    }

    @org.junit.jupiter.api.Test
    public void testLeftTraversalFetch() throws ParserConfigurationException, IOException, SAXException {
        System.out.println("\n\ntestLeftTraversalFetch():");
        final String xml = FileUtils.loadResourceAsString(getClass(), TEST_RESOURCE);
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        ByteArrayInputStream is = new ByteArrayInputStream(xml.getBytes(StandardCharsets.UTF_8));
        Document doc = builder.parse(is);
        doc.getDocumentElement().normalize();
        NodeList nodeList = doc.getElementsByTagName("countries");
        Node node = nodeList.item(0);
        final String attrValue1a = DOMParser.fetchFromNodeUsingLeftTraversal(node, "country", "id");
        final String attrValue2a = DOMParser.fetchFromNodeUsingLeftTraversal(node, "country", "provinces", "province", "name");
        final String tagValue1a = DOMParser.fetchFromNodeUsingLeftTraversal(node, "country", "countryName");
        final String tagValue2a = DOMParser.fetchFromNodeUsingLeftTraversal(node, "country", "provinces", "province", "capital");
        final String attrValue1b = DOMParser.fetchFromNodeUsingLeftTraversal(node, "id");
        final String attrValue2b = DOMParser.fetchFromNodeUsingLeftTraversal(node, "name");
        final String tagValue1b = DOMParser.fetchFromNodeUsingLeftTraversal(node, "countryName");
        final String tagValue2b = DOMParser.fetchFromNodeUsingLeftTraversal(node, "capital");
        System.out.println("Full path specified:");
        System.out.format("attrValue1a = %s\n", attrValue1a);
        System.out.format("attrValue2a = %s\n", attrValue2a);
        System.out.format("tagValue1a = %s\n", tagValue1a);
        System.out.format("tagValue2a = %s\n", tagValue2a);
        System.out.println("Only leaf specified (-> attributes won't be found):");
        System.out.format("attrValue1b = %s\n", attrValue1b);
        System.out.format("attrValue2b = %s\n", attrValue2b);
        System.out.format("tagValue1b = %s\n", tagValue1b);
        System.out.format("tagValue2b = %s\n", tagValue2b);
        assertEquals("de", attrValue1a);
        assertEquals("Bavaria", attrValue2a);
        assertEquals("Germany", tagValue1a);
        assertEquals("Munich", tagValue2a);
        assertNull(attrValue1b);
        assertNull(attrValue2b);
        assertEquals("Germany", tagValue1b);
        assertEquals("Munich", tagValue2b);
    }

}
