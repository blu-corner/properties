/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#pragma once

#include <cstdarg>
#include <string>
#include <map>
#include <vector>
#include "utils.h"

using namespace std;

namespace neueda
{

class properties
{
public:
    /* properties hierarchy, is

       <namespace>.<class>.<name>.<property>,
       <namespace>.<class>.<property>,
       <namespace>.<property>,
       <property>
    */

    properties ();

    properties (const string& ns,
                const string& clas,
                const string& instance);

    properties (const properties& p,
                const string& ns,
                const string& clas,
                const string& instance);

    ~properties ();

    /**
     * Parses the given file for properties, returns a bool to indicate success
     * or failure. In the event of failure errorMessage will contain further
     * information.
     *
     * @param filePath The full path to the properties file
     * @param errorMessage Further info should an error occur
     * @return Whether the load is successful
     */
    bool loadFromFile (const string& filePath, string& errorMessage);

    /**
     * Retrieves a value for the given property as a string. Returns bool
     * indicating whether the property was found or not.
     *
     * @param property const string property to search for
     * @param value The value found for that property
     * @return Whether the property was found
     */
    bool get (const string& property, string& value) const;

    /**
     * Retrieves a value for the given property as a string. Returns bool
     * indicating whether the property was found or not. If the property was not
     * found default is assigned.
     *
     * @param property const string property to search for
     * @param defaultValue const string value to assign if not found
     * @param value The value found for that property
     * @return Whether the property was found
     */
    bool get (const string& property,
              const string& defaultValue,
              string& value) const;

    /**
     * Retrieves the value for the given property as a boolean. Returns bool
     * indicating whether the property was found or not, and sets valid to
     * true/false if it was a valid bool.
     *
     * @param property const string property to search for
     * @param value bool value found for that property
     * @param valid whether the value found could be parsed to a bool
     * @return Whether the property was found
     */
    bool get (const string& property,
              bool& value,
              bool& valid) const;

    /**
     * Retrieves the value for the given property as a boolean. Returns bool
     * indicating whether the property was found or not, and sets valid to
     * true/false if it was a valid bool. If the property wasn't found
     * defaultValue is assigned to value.
     *
     * @param property const string property to search for
     * @param defaultValue const bool value assigned if property not found
     * @param value bool value found for that property
     * @param valid whether the value found could be parsed to a bool
     * @return Whether the property was found
     */
    bool get (const string& property,
              const bool& defaultValue,
              bool& value,
              bool& valid) const;

    /**
     * Retrieves the value for the given property as an integer/double. Returns
     * bool if the property was found and sets valid to true/false if it was a
     * valid number.
     *
     * @param property const string property to search for
     * @param value number value found for that property
     * @param valid whether the value found could be parsed to a number
     * @return Whether the property was found
     */
    template <typename T>
    bool get (const string& property, T& val, bool& valid) const
    {
        string s;

        valid = true;
        if (!get (property, s))
            return false;

        if (!utils_parseNumber (s, val))
            valid = false;

        return true;
    }

    bool getV (string& value,
               const char* fmt, ...) const;

    bool getV (const string& defaultValue,
               string& value,
               const char* fmt, ...) const;

    /**
     * Assign the given value to the property
     *
     * @param property const string property name
     * @param value const string value to assign to the given property
     */
    void setProperty (const string& property, const string& value);
    
    map <string, string>::const_iterator begin () const;

    map <string, string>::const_iterator end () const;

    vector<string> getKeys () const;

private:
    bool getProperty (const string& property, string& value) const;

    string getFullPropertyKey (const string& key) const;

    string getNamespaceNameKey (const string& key) const;

    string getNamespaceClassKey (const string& key) const;

    string getNamespaceKey (const string& key) const;

    map <string, string>    mProperties;
    string                  mNamespace;
    string                  mClass;
    string                  mName;
};

};
