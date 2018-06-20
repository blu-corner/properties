#include "properties.h"
#include "utils.h"
#include <stdint.h>
#include <iostream>

using namespace std;
using namespace neueda;

int main (int argc, char** argv)
{
    string key = "test";
    properties p ("oms", "gw", "lse");

    string err;
    if (!p.loadFromFile ("../sample.properties", err))
    {
        cout << "loadFromFile failed: " << err << endl;
        return 1;
    }

    string property1;
    if (!p.get ("property1", property1))
    {
        cout << "failed to get property1" << endl;
    }
    else
        cout << "property1=" << property1 << endl;

    bool valid;
    bool property2;
    p.get ("property2", property2, valid);
    if (!valid)
    {
        cout << "failed to parse boolean for property2" << endl;
    }
    else
        cout << "property2=" << property2 << endl;

    int property3;
    p.get ("property3", property3, valid);
    if (!valid)
    {
        cout << "failed to parse number for property3" << endl;
    }
    else
        cout << "property3=" << property3 << endl;

    double property4;
    p.get ("property4", property4, valid);
    if (!valid)
    {
        cout << "failed to parse number for property4" << endl;
    }
    else
        cout << "property4=" << property4 << endl;

    return 0;
}
