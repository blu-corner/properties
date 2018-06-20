/*
 * Copyright 2014-2018 Neueda Ltd.
 */
%feature("autodoc");

// needs exceptions
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

%include "properties.i"

%extend neueda::properties {

    std::string __getitem__(const std::string& key)
    {
        std::string value;
        if (!self->get (key, value)) {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }

        return value;
    }

    void __setitem__(const std::string key, std::string value)
    {
        self->setProperty (key, value);
    }
}
