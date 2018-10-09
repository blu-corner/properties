/*
 * Copyright 2014-2018 Neueda Ltd.
 */
import com.neueda.config.*;

public class example
{
    public static void main(String[] args)
    {
        RawProperties props = new RawProperties ();

        StringVector vector = props.getKeys ();

        try {
            String value = props.getProperty ("invalid-key");
            System.out.println(value);
        }
        catch (Exception e)
        {
            System.out.println(e);
        }

        props.setProperty ("test", "1234");

        try {
            String value = props.getProperty ("test");
            System.out.println(value);
        }
        catch (Exception e)
        {
            System.out.println(e);
        }

    }
}
