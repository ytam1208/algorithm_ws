#!/usr/bin/env python
#slicing
import rospy

string = 'abcdef2a444532'
string_replace = string.replace('a', 'A')
print(string_replace)

string = 'abcd'

string.replace('b', 'B')
print('Not_return replace = ', string)

sting_replace = string.replace('b', 'B')
print(string_replace)
