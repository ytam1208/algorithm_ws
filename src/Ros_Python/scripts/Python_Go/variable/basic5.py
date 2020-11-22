#!/usr/bin/env python

import rospy

num_str = "720"
num_int = int(num_str)
print("num_int = ", num_int+1, type(num_int))

num_int2 = 100
num_str2 = str(num_int2)
print("num_str2 = ", num_str2, type(num_str2))
