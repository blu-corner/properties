/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#include "properties.h"

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <map>

namespace neueda
{

properties::properties () : 
    mProperties (),
    mNamespace (""),
    mClass (""),
    mName ("")
{
    // default constructor
}

properties::properties (const string& ns,
                        const string& clas,
                        const string& instance) : 
    mProperties (),
    mNamespace (ns),
    mClass (clas),
    mName (instance)
{
    // default constructor
}

properties::properties (const properties& p,
                        const string& ns,
                        const string& clas,
                        const string& instance) :
    mProperties (p.mProperties),
    mNamespace (ns),
    mClass (clas),
    mName (instance)
{

}

properties::~properties ()
{
    // destructor
}

bool
properties::loadFromFile (const string& filePath, string& errorMessage)
{
    ifstream readFromFile (filePath.c_str ());
    if (readFromFile.fail ())
    {
        errorMessage.assign (strerror (errno));
        return false;
    }
    string   prop;
    string   value;

    string line;
    while (getline (readFromFile, line))
    {
        utils_trimWhiteSpace (line);
        if (line.size() == 0 || line[0] == '#')
            continue; // ignore

        size_t i = line.find_first_of ('=');
        if (i == string::npos)
            continue; // ignore

        prop.assign (line, 0, i);
        value.assign (line, i + 1, line.size() - i);

        if (prop.find_first_of('#') != string::npos)
            continue; // ignore

        utils_trimWhiteSpace (prop);

        i = value.find_first_of ('#');
        if (i != string::npos)
            value.assign (value, 0, i);

        utils_trimWhiteSpace (value);

        setProperty (prop, value);
    }
    readFromFile.close ();
    return true;
}

bool
properties::get (const string& property,
                 string& value) const
{
    // search for <namespace>.<class>.<name>.<property>
    if (!mNamespace.empty () && !mClass.empty () && !mName.empty ())
    {
        if (getProperty (getFullPropertyKey (property), value))
            return true;
    }

    // search for <namespace>.<class>.<property>
    if (!mNamespace.empty () && !mClass.empty ())
    {
        if (getProperty (getNamespaceClassKey (property), value))
            return true;
    }

    // search for <namespace>.<property>
    if (!mNamespace.empty ())
    {
        if (getProperty (getNamespaceKey (property), value))
            return true;
    }

    // treat property as fully qualified
    if (getProperty (property, value))
        return true;

    return false;
}

bool
properties::get (const string& property,
                 const string& defaultValue,
                 string& value) const
{
    if (get (property, value))
        return true; // found

    value.assign (defaultValue);
    return false; // not found, using default
}

bool
properties::get (const string& property,
                 bool& value,
                 bool& valid) const
{
    string s;

    valid = true;
    if (!get (property, s))
        return false;

    if (!utils_parseBool (s, value))
        valid = false;

    return true;
}

bool
properties::get (const string& property,
                 const bool& defaultValue,
                 bool& value,
                 bool& valid) const
{
    string s;

    valid = true;
    if (!get (property, value, valid))
    {
        value = defaultValue;
        return false;
    }

    return true;
}

bool
properties::getV (string& value,
                  const char* fmt, ...) const
{
    va_list ap;
    va_start (ap, fmt);

    char s[2048];
    vsnprintf (s, sizeof s, fmt, ap);

    va_end (ap);

    string property (s);

    return get (property, value);
}

bool
properties::getV (const string& defaultValue,
                  string& value,
                  const char* fmt, ...) const
{
    va_list ap;
    va_start (ap, fmt);

    char s[2048];
    vsnprintf (s, sizeof s, fmt, ap);

    va_end (ap);

    string property (s);

    return get (property, defaultValue, value);
}

void
properties::setProperty (const string& key,
                         const string& value)
{
    mProperties.insert(pair<string, string> (key, value));
}

map <string, string>::const_iterator
properties::begin () const
{
    return mProperties.begin ();
}

map <string, string>::const_iterator
properties::end () const
{
    return mProperties.end ();
}

vector<string>
properties::getKeys () const
{
    vector<string> keys;

    map <string, string>::const_iterator it;
    for (it = begin (); it != end (); ++it)
        keys.push_back (it->first);

    return keys;
}

bool
properties::getProperty (const string& property, string& value) const
{
    map<string, string>::const_iterator it = mProperties.find (property);
    if (it != mProperties.end ())
    {
        value.assign (it->second);
        return true;
    }
    return false;
}

string
properties::getFullPropertyKey (const string& key) const
{
    return mNamespace + "." + mClass + "." + mName + "." + key;
}

string
properties::getNamespaceNameKey (const string& key) const
{
    return mNamespace + "." + mName + "." + key;
}

string
properties::getNamespaceClassKey (const string& key) const
{
    return mNamespace + "." + mClass + "." + key;
}

string
properties::getNamespaceKey (const string& key) const
{
    return mNamespace + "." + key;
}

} /* neueda */
