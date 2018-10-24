/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#include "properties.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#define TEST_FILE_NAME "gtest_neueda_config.properties"

using namespace neueda;

class PropertiesTestHarness : public ::testing::Test
{
protected:
    virtual void SetUp ()
    {
        std::ofstream myfile;
        myfile.open (TEST_FILE_NAME);
        myfile << "namespace.class.name.key1=123\n"
               << "namespace.class.name.key2=456\n"
               << "namespace.class.key4=456\n"
               << "namespace.key5=456\n"
               << "namespace.class.key6=true\n";
        myfile.close();        
        
        std::string errorMessage;
        mProps = new properties("namespace", "class", "name");
        bool ok = mProps->loadFromFile (TEST_FILE_NAME, errorMessage);
        if (!ok)
        {
            std::cerr << "failed to load file: "
                      << errorMessage
                      << std::endl;
        }
        ASSERT_TRUE(ok);
    }

    virtual void TearDown ()
    {
        ::remove (TEST_FILE_NAME);
        delete mProps;
    }

    properties* mProps;
};


TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GET_KEY1)
{
    std::string value;
    bool ok = mProps->get ("key1", value);

    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "123");
}

TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GETV_KEY1)
{
    std::string key ("key1");
    std::string value;
    bool ok = mProps->getV (value, "%s", key.c_str ());

    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "123");
}

TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GET_KEY2)
{
    std::string value;
    bool ok = mProps->get ("key2", value);

    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "456");
}

TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GET_NUMBER_KEY2)
{
    int value;
    bool valid;
    bool ok = mProps->get ("key2", value, valid);

    ASSERT_TRUE(ok);
    ASSERT_TRUE(valid);
    ASSERT_EQ(value, 456);
}

TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GET_KEY4)
{
    std::string value;
    bool ok = mProps->get ("key4", value);

    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "456");
}

TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GET_KEY5)
{
    std::string value;
    bool ok = mProps->get ("key5", value);

    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "456");
}

TEST_F(PropertiesTestHarness, TEST_THAT_PROPERTIES_CAN_GET_BOOL_KEY6)
{
    bool value;
    bool valid;
    bool ok = mProps->get ("key6", value, valid);

    ASSERT_TRUE(ok);
    ASSERT_TRUE(valid);
    ASSERT_TRUE(value);
}

TEST_F(PropertiesTestHarness, TEST_SET_PROPERTY)
{
    mProps->setProperty ("new", "100");
    
    std::string value;
    bool ok = mProps->get ("new", value);

    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "100");
}
