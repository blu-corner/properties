/*
 * Copyright 2014-2018 Neueda Ltd.
 */
%pragma(java) jniclasscode=%{
     // jniclasscode pragma code: Static block so that the JNI class loads the C++ DLL/shared object when the class is loaded
     static {
         try {
             System.loadLibrary("Properties");
         } catch (UnsatisfiedLinkError e) {
             System.err.println("Native code library failed to load.\n" + e);
             System.exit(1);
         }
     }
%}


%include "exception.i"

// simple exception handler
%exception {
    try {
        $action
    } catch (std::exception &e) {
        std::string s("properties-error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    }
}

%javaexception("java.lang.Exception") neueda::properties::loadFromFile {
    try {
        $action
    } catch (std::exception &e) {
        std::string s("properties-error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    }
 }

%javaexception("java.lang.Exception") neueda::properties::get {
    try {
        $action
    } catch (std::exception &e) {
        std::string s("properties-error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    }
 }

%javaexception("java.lang.Exception") neueda::properties::get {
    try {
        $action
    } catch (std::exception &e) {
        std::string s("properties-error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    }
 }

SWIG_JAVABODY_PROXY(public, public, SWIGTYPE)
SWIG_JAVABODY_TYPEWRAPPER(public, public, public, SWIGTYPE)

%include "properties.i"
