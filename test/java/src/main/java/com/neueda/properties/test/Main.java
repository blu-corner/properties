/*
 * Copyright 2018 Neueda
 * 
 */

package com.neueda.properties.test;

import org.junit.internal.TextListener;
import org.junit.runner.JUnitCore;

import com.neueda.properties.test.TestProperties;
import com.neueda.properties.test.TestNamespaceProperties;

public class Main {
    public static void main(String[] args) {
        

    	System.out.println("Running tests!");
    	System.out.println("java.library.path: " + System.getProperty("java.library.path"));
    	
        JUnitCore engine = new JUnitCore();
        engine.addListener(new TextListener(System.out)); // required to print reports
        engine.run(TestProperties.class);
        engine.run(TestNamespaceProperties.class);
    }
}
