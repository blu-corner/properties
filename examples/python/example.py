import sys
from properties import Properties

if __name__ == '__main__':
    key = "test"
    value = "true"

    p = Properties("oms", "gw", "lse")
    try:
        p.loadFromFile("../sample.properties")
    except Exception as e:
        print(e)
        sys.exit(1)

    property1 = p['property1']
    print("property1={0}".format(property1))

    try:
        property2 = p.getBool("property2")
        print("property2={0}".format(property2))

        property3 = p.getInteger("property3")
        print("property3={0}".format(property3))

        property4 = p.getDouble("property4")
        print("property4={0}".format(property4))
    except Exception as e:
        print(e)
        sys.exit(1)
