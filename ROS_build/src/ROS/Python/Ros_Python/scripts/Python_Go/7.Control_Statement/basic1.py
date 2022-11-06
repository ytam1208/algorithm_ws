#!/usr/bin/env python

import rospy

#example101
flag = True
print(flag)

flag = False
print(flag)

#example 102
print(3 == 5) #FLASE

#example 103
print(3 < 5)

#example 104
x = 4
print(1 < x < 5)

#example 105
print((3 == 3) and (4 != 3))

#example 106
print(3 >= 4)

#example 107
if 4 < 3:
	print("Hello World")

#example 108
if 4 < 3:
	print("Hello World.")
else:
	print("Hi, there,")

if 4 > 3:
	print("Hello World.")
else:
	print("Hi, there,")

#example 109
if True :
	print("109_1")
	print("109_2")
else :
	print("109_3")
print("109_4")

#example 110
if True :
	if False:
		print("110_1")
		print("110_2")
	else:
		print("110_3")
else :
	print("110_4")
print("110_5")

