/*
 * Copyright 2014-2018 Neueda Ltd.
 */
%module(directors="1", thread="1") propertiesBinding

%{
#include "properties.h"

#include <stdexcept>
#include <sstream>
#include <string>
%}

%include "std_string.i"
%include "stdint.i"
%include "std_vector.i"
%include "cdata.i"
%include "typemaps.i"

// macros
%define __attribute__(x)
%enddef

%rename(Properties) neueda::properties;

// templates
%template(StringVector) std::vector<std::string>;

// ignore these
%ignore neueda::properties::begin;
%ignore neueda::properties::end;

%extend neueda::properties {
    std::string get (const std::string& key) throw (std::runtime_error)
    {
        std::string value;
        bool ok = self->get (key, value);
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }

        return value;
    }

    std::string get (const std::string& key, const std::string& defaultValue) throw (std::runtime_error)
    {
        std::string value;
        self->get (key, defaultValue, value);
        return value;
    }
    
    void loadFromFile (const std::string& filePath) throw (std::runtime_error)
    {
        std::string errorMessage;
        bool ok = self->loadFromFile (filePath, errorMessage);
        
        if (!ok)
        {
            std::ostringstream oss;
            oss << "failed to load properties file: "
                << errorMessage;
            throw std::runtime_error (oss.str ());
        }
    }

     bool getBool (const std::string& key) throw (std::runtime_error)
     {
         bool val;
         bool valid = true;
         if (!self->get (key, val, valid))
         {
             std::ostringstream oss;
             oss << "failed to find key: "
                 << key;
             throw std::runtime_error (oss.str ());
         }

         if (!valid)
         {
             std::ostringstream oss;
             oss << "failed to parse value as bool "
                 << key;
             throw std::runtime_error (oss.str ());
         }

         return val;
     }

    int getInteger (const std::string& key) throw (std::runtime_error)
    {
        int val;
        bool valid = true;
        if (!self->get (key, val, valid))
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }

        if (!valid)
        {
            std::ostringstream oss;
            oss << "failed to parse value as integer "
            << key;
            throw std::runtime_error (oss.str ());
        }

        return val;
    }

    double getDouble (const std::string& key) throw (std::runtime_error)
    {
        double val;
        bool valid = true;
        if (!self->get (key, val, valid))
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }

        if (!valid)
        {
            std::ostringstream oss;
            oss << "failed to parse value as integer "
                << key;
            throw std::runtime_error (oss.str ());
        }

        return val;
    }
}

%include "properties.h"
