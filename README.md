# PROJECT MOVED TO [Version1 Org/properties](https://github.com/Version1/properties). THIS REPOSITORY WILL NO LONGER BE MAINTAINED.

# properties [![Build Status](https://travis-ci.com/blu-corner/properties.svg?branch=master)](https://travis-ci.com/blu-corner/properties) [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=blu-corner_properties&metric=alert_status)](https://sonarcloud.io/dashboard?id=blu-corner_properties)

A C++ properties file parser. There are SWIG bindings allowing this module to be
used from Java and Python.

Properties can be in any of the following formats:

```bash
<namespace>.<class>.<name>.<property>=<value>
<namespace>.<class>.<property>=<value>
<namespace>.<property>=<value>
<property>=<value>
```

The hierarchy for property loading also matches the above order. Given the following
example config file:

```bash
<namespace>.<class>.<name>.KEY=VALUE1
KEY=VALUE2
```

Retrieving KEY from the properties will return VALUE1.

## Getting Started

To compile the installation:

```bash
$ git submodule update --init --recursive
$ mkdir build
$ cd build
$ cmake -DTESTS=ON ../
$ make install
```

Language bindings can be enabled by passing -DJAVA=on, -DCSHARP=on, -DPYTHON=on
to CMake. It is possible to build all bindings in the same build.

### Dependencies

The only external dependency is SWIG, and is only required when building the Java,
C# or Python bindings. For information on installing SWIG please visit the [SWIG website](http://www.swig.org). All other dependencies are managed through git submodules.

### Example Usage

Given a configuration file:

```bash
oms.gw.lse.ip=192.168.0.1
oms.gw.lse.port=7222
```

The following shows how to load the file and access properties within it.

```cpp
#include "properties.h"
#include <stdint.h>
#include <string>
#include <iostream>

using namespace neueda;

int main ()
{
    properties p ("oms", "gw", "lse");

    std::string err;
    if (!p.loadFromFile ("config.file", err))
    {
        std::cout << err << std::endl;
        return 1;
    }

    std::string ip;
    std::string port;

    if (!p.get ("ip", ip))
    {
        std::cout << "failed to retrieve ip" << std::endl;
        return 1;
    }

    if (!p.get ("port", port))
    {
        std::cout << "failed to retrieve port" << std::endl;
        return 1;
    }

    std::cout << "IP: " << ip << " Port: " << port << std::endl;
}

```

Examples have been provided in each language within the [examples folder](./examples/).

* [C++](examples/cpp/README.md)
* [Java](examples/java/README.md)
* [Python](examples/python/README.md)

## Running the Tests

To compile the unittests specify -DTESTS=on when running CMake. The lib folder 
must be on the LD_LIBRARY_PATH in order to find libproperties.so.

```bash
mkdir build && cd build
cmake -DTESTS=on ..
make install
make test
```
