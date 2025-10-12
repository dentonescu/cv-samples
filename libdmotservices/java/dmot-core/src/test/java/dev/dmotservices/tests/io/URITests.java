package dev.dmotservices.tests.io;

import dev.dmotservices.io.UriUtils;
import static org.junit.jupiter.api.Assertions.*;


import java.net.URISyntaxException;
import java.util.Map;

public class URITests {

    @org.junit.jupiter.api.Test
    public void testLastPathSegement() throws URISyntaxException {
        System.out.println("\n\ntestLastPathSegement():");
        final java.net.URI a = new java.net.URI("https://blah.de/gaga/barf/narf");
        final java.net.URI b = new java.net.URI("http://example.com/foo/bar/wonderful?param=true");
        final java.net.URI c = new java.net.URI("http://dsafjsadof.com/x/odfdasf/test1/");
        final java.net.URI d = new java.net.URI("http://dsafjsadof.com/x/odfdasf/test2#");
        final java.net.URI e = new java.net.URI("");
        final java.net.URI f = new java.net.URI("/");
        final String lpsA = UriUtils.getLastPathSegement(a);
        final String lpsB = UriUtils.getLastPathSegement(b);
        final String lpsC = UriUtils.getLastPathSegement(c);
        final String lpsD = UriUtils.getLastPathSegement(d);
        final String lpsE = UriUtils.getLastPathSegement(e);
        final String lpsF = UriUtils.getLastPathSegement(f);
        System.out.format("%s => %s\n", a, lpsA);
        System.out.format("%s => %s\n", b, lpsB);
        System.out.format("%s => %s\n", c, lpsC);
        System.out.format("%s => %s\n", d, lpsD);
        System.out.format("%s => %s\n", e, lpsE);
        System.out.format("%s => %s\n", f, lpsF);
        assertEquals("narf", lpsA);
        assertEquals("wonderful", lpsB);
        assertEquals("test1", lpsC);
        assertEquals("test2", lpsD);
        assertEquals("", lpsE);
        assertEquals("", lpsF);
    }

    @org.junit.jupiter.api.Test
    public void testQueryMap() {
        System.out.println("\n\ntestQueryMap():");
        final String QUERY_STRING = "a=1234&b=5678&hello=world&x=y";
        final Map<String, String> map1 = UriUtils.getQueryMap(QUERY_STRING);
        System.out.format("QUERY_STRING=%s, map1=%s\n", QUERY_STRING, map1);
        assertEquals(4, map1.size());
        final Map<String, String> map2 = UriUtils.getQueryMap(null);
        System.out.format("QUERY_STRING=null, map2=%s\n", map2);
        assertEquals(0, map2.size());

    }

    @org.junit.jupiter.api.Test
    public void testUriConcatenate() {
        System.out.println("\n\ntestUriConcatenate():");
        final java.net.URI baseURI = java.net.URI.create("https://dpm.cloud.veda.net/horizon/api/v3/");
        final java.net.URI uri = UriUtils.concatenate(baseURI, "persons", "?", "company-short-name=1", "&", "employee-number=9999");
        System.out.format("uri=%s\n", uri);
        assertEquals("https://dpm.cloud.veda.net/horizon/api/v3/persons?company-short-name=1&employee-number=9999",
                uri.toString());
    }

}