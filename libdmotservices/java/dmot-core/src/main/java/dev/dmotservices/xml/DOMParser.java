package dev.dmotservices.xml;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

public class DOMParser<T> {

    protected String xml;
    protected NodeProcessor<T> nodeProcessor;
    protected Charset charset;
    protected String outerNodeName;

    protected final List<T> parsedResultList = new ArrayList<>();

    public DOMParser(String xml, NodeProcessor<T> nodeProcessor, Charset charset) {
        if (null == xml)
            throw new IllegalArgumentException("The XML body is required.");
        if (null == nodeProcessor)
            throw new IllegalArgumentException("A node processor is required.");
        if (null == charset)
            throw new IllegalArgumentException("You must specify a character set.");
        this.xml = xml;
        this.nodeProcessor = nodeProcessor;
        this.charset = charset;
    }

    public void setXml(String xml) {
        this.xml = xml;
    }

    public void setNodeProcessor(NodeProcessor<T> nodeProcessor) {
        this.nodeProcessor = nodeProcessor;
    }

    public void setCharset(Charset charset) {
        this.charset = charset;
    }

    public void setOuterNodeName(String outerNodeName) {
        this.outerNodeName = outerNodeName;
    }

    public void parse() throws ParserConfigurationException, IOException, SAXException {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        ByteArrayInputStream is = new ByteArrayInputStream(xml.getBytes(charset));
        Document doc = builder.parse(is);
        doc.getDocumentElement().normalize();
        final boolean wasSpecifiedOuterNodeName = (null != outerNodeName && !outerNodeName.isBlank());
        NodeList nodeList = wasSpecifiedOuterNodeName ?
                doc.getElementsByTagName(outerNodeName) : // treat the supplied node name as the top level node
                doc.getChildNodes() // no outer level node name specified, get the top level nodes
                ;

        for (int i = 0; i < nodeList.getLength(); ++i) {
            Node node = nodeList.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE) {
                NodeList subNodeList = node.getChildNodes(); // get all the sub nodes for each outer node
                for (int j = 0; j < subNodeList.getLength(); ++j) {
                    Node subNode = subNodeList.item(j);
                    if (subNode.getNodeType() == Node.ELEMENT_NODE) {
                        nodeProcessor.process(subNode);
                        parsedResultList.add(nodeProcessor.getResult());
                    }
                }
            }
        }
    }

    public List<T> getParsedResultList() {
        return parsedResultList;
    }

    /**
     * Traverses the nodes using a left-first depth search to retrieve an attribute or tag value.
     * <p>
     * Example:
     * company.country[].region[].branch[].name
     * <p>
     * The traversal will always use:
     * company.country[0].region[0].branch[0].name
     */
    @SuppressWarnings("GrazieInspection")
    public static String fetchFromNodeUsingLeftTraversal(Node node, String... args) {
        String obtained;
        if (null == node)
            return null;
        if (null == args)
            return null;
        if (node.getNodeType() != Node.ELEMENT_NODE)
            return null;
        Element subElement = (Element) node;
        NodeList subElementList;
        for (int i = 0; i < args.length - 1; ++i) {
            subElementList = subElement.getElementsByTagName(args[i]);
            subElement = (Element) subElementList.item(0);
            if (null == subElement)
                return null;
        }
        final String leafName = args[args.length - 1];
        obtained = subElement.getAttribute(leafName);
        if (obtained.isBlank()) {
            subElementList = subElement.getElementsByTagName(leafName);
            subElement = (Element) subElementList.item(0);
            if (null == subElement)
                return null;
            obtained = subElement.getTextContent();
        }
        return obtained;
    }
}
