/*
 * Copyright 2014-2018 Neueda Ltd.
 */
package com.neueda.properties.example;

import com.neueda.properties.Properties;

public class App
{
    public static void main( String[] args )
    {
        Properties p = new Properties("oms", "gw", "lse");
        try {
            p.loadFromFile("../sample.properties");
        }
        catch(Exception e) {
            System.out.println("failed to load properties file: " + e.toString());
        }

        try {
            String property1 = p.get("property1");
            System.out.println("property1=" + property1);

            boolean property2 = p.getBool("property2");
            System.out.println("property2=" + property2);

            long property3 = p.getInteger("property3");
            System.out.println("property3=" + property3);

            double property4 = p.getDouble("property4");
            System.out.println("property4=" + property4);
        } catch(Exception e) {
            System.out.println("failed to get: " + e.toString());
        }
    }
}
