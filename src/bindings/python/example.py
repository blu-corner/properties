#
# Copyright 2014-2018 Neueda Ltd.
#
import Config


props = Config.properties ()
print props.getKeys ()

try:
    print props['invalid-key']
except Exception as e:
    print e

props['test'] = '1234'
print props['test']

