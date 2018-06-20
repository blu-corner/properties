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

public class TestNamespaceProperties {
	
	private Properties props;
    private String nsProperties = "ns.clas.instance.property1=val1\n"
        + "ns.clas.instance.property2=val2\n"
        + "property3=wrong\n"
        + "ns.property3=wrong\n"
        + "ns.clas.property3=wrong\n"
        + "ns.clas.instance.property3=val3\n"
        + "ns.clas.instance.property4=true\n"
        + "ns.clas.instance.property5=false\n"
        + "property6=wrong\n"
        + "ns.property6=wrong\n"
        + "ns.clas.property6=val6\n"
        + "property7=wrong\n"
        + "ns.property7=val7\n"
        + "property8=val8\n"
        + "ns.clas.instance.bool_true1=true\n"
        + "ns.clas.instance.bool_true2=TRUE\n"
        + "ns.clas.instance.bool_true3=y\n"
        + "ns.clas.instance.bool_false1=false\n"
        + "ns.clas.instance.bool_false2=False\n"
        + "ns.clas.instance.bool_false3=n\n"
        + "ns.clas.instance.integer1=100\n"
        + "ns.clas.instance.integer2=a\n"
        + "ns.clas.instance.double1=10.19\n"
        + "ns.clas.instance.double2=a\n"
        + "newns.newclas.newinstance.property1=value1\n";
	
	@Before
	public void setUp() {
        PrintWriter writer = null;
        try {
            writer = new PrintWriter("test_data.properties", "UTF-8");
        } catch (Exception e) {
            fail();
        }

        String [] lines = nsProperties.split("\n");
        for (String l: lines)
        {
            writer.println(l);
        }

        writer.close();

        props = new Properties("ns", "clas", "instance");
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

    @Test(expected=RuntimeException.class)
    public void testLoadFromFileFail() {
        try {
            Properties p = new Properties();
            p.loadFromFile("file-that-doesnt-exist.txt");
        } catch(RuntimeException re) {
            throw re;
        } catch(Exception e) {
            fail();
        }
    }

    @Test(expected=RuntimeException.class)
    public void testGetFail() {
        try {
            props.get("doesnt-exist");
        } catch(RuntimeException re) {
            throw re;
        } catch(Exception e) {
            fail();
        }
    }

    @Test(expected=RuntimeException.class)
    public void testGetBoolFail() {
        try {
            props.getBool("property1");
        } catch(RuntimeException re) {
            throw re;
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testGet() {
        try {
            assertEquals(props.get("property1"), "val1");
            assertEquals(props.get("property2"), "val2");
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testGetHierarchy() {
        try {
            assertEquals(props.get("property3"), "val3");
            assertEquals(props.get("property6"), "val6");
            assertEquals(props.get("property7"), "val7");
            assertEquals(props.get("property8"), "val8");
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testGetDefault() {
        try {
            assertEquals(props.get("yyy", "100"), "100");
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testSet() {
        try {
            props.setProperty("new", "100");
            assertEquals(props.get("new"), "100");
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testBool() {
        try {
            assertTrue(props.getBool("bool_true1"));
            assertTrue(props.getBool("bool_true2"));
            assertTrue(props.getBool("bool_true3"));

            assertFalse(props.getBool("bool_false1"));
            assertFalse(props.getBool("bool_false2"));
            assertFalse(props.getBool("bool_false3"));
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testGetInteger() {
        try {
            assertEquals(props.getInteger("integer1"), 100);
        } catch(Exception e) {
            fail();
        }
    }

    @Test(expected=RuntimeException.class)
    public void testGetIntegerFail() {
        try {
            props.getInteger("integer2");
        } catch(RuntimeException re) {
            throw re;
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testGetDouble() {
        assertEquals(props.getDouble("double1"), 10.19);
    }

    @Test(expected=RuntimeException.class)
    public void testGetDoubleFail() {
        try {
            props.getInteger("double2");
        } catch(RuntimeException re) {
            throw re;
        } catch(Exception e) {
            fail();
        }
    }

    @Test
    public void testSetNamespace() {
        Properties p = new Properties(props, "newns", "newclas", "newinstance");
        try {
            assertEquals(p.get("property1"), "value1");
        } catch(Exception e) {
            fail();
        }
    }
}
