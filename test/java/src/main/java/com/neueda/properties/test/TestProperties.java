/*
 * Copyright 2018 Neueda
 * 
 */
package com.neueda.properties.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import java.io.PrintWriter;
import java.io.File;

import com.neueda.properties.Properties;

public class TestProperties {
	
	private Properties props;
    private String basicProperties = "property1=val1\n"
        + "ns.property2=val2\n"
        + "ns.clas.property3=val3\n"
        + "ns.clas.instance.property4=val4\n"
        + "ns.clas.instance.property5=wrong\n"
        + "property5=val5\n";
	
	@Before
	public void setUp() {
        PrintWriter writer = null;
        try {
            writer = new PrintWriter("test_data.properties", "UTF-8");
        } catch (Exception e) {
            fail();
        }

        String [] lines = basicProperties.split("\n");
        for (String l: lines)
        {
            writer.println(l);
        }

        writer.close();

        props = new Properties();
        try {
            props.loadFromFile("test_data.properties");
        } catch (Exception e) {
            System.out.println("failed loadFromFile: " + e.getMessage());
            fail();
        }
	}
	
    @After
    public void tearDown() {
        File f = new File("test_data.properties");
        if (f.exists())
        {
            f.delete();
        }
    }

	@Test
	public void testGetProperty() {
		try {
			assertNotNull(props);
	   		assertEquals(props.get("property1"), "val1");
	   		assertEquals(props.get("ns.property2"), "val2");
	   		assertEquals(props.get("ns.clas.property3"), "val3");
	   		assertEquals(props.get("ns.clas.instance.property4"), "val4");
		} catch (Exception e) {
			System.out.println("ERROR: " + e.getMessage());
			fail();
		}
	}

	@Test
	public void testHierarchy() {
		try {
			assertNotNull(props);
	   		assertEquals(props.get("property5"), "val5");
		} catch (Exception e) {
			System.out.println("ERROR: " + e.getMessage());
			fail();
		}
	}
}
