#!/usr/bin/env python
#slicing
import rospy

phone_number = "010-1111-2222"
print(phone_number)

phone_number1 = phone_number.replace("-", " ")
print(phone_number1)

phone_number2 = phone_number.replace("-", "")
print(phone_number2)
