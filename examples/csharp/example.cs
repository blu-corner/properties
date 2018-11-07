/*
 * Copyright 2014-2018 Neueda Ltd.
 */
using System;
using Neueda.Properties;

class example
{
    static void Main(string[] args)
    {
        String key1 = "test.key1";
        String key2 = "test.key2";
        Properties p = new Properties ();

        p.setProperty (key1, "test");
        p.setProperty (key2, "1");

        Console.WriteLine (p.get (key1));

        try
        {
            //Does not fail, because value can be parset as int
            Console.WriteLine (p.getInteger (key2));
            //Does fail because value is a string
            Console.WriteLine (p.getInteger (key1));
        }
        catch (Exception e)
        {
            Console.WriteLine (e);
        }

    }
}
