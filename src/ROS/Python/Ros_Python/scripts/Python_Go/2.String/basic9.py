#!/usr/bin/env python
#slicing
import rospy

a = "3"
b = "4"
print(a + b)

a = 3
b = 4
print(a+b)

print("Hi" * 3)
print("-" * 80)

t1 = 'python'
t2 = 'java'
t3 = t1 + ' ' + t2 + ' '
print(t3 * 4)

name1 = "kim"
age1 = 10
name2 = "lee"
age2 = 13
print("name = ", name1, " ", "age = ", age1)
print("name = ", name2, " ", "age = ", age2)

##%s
print("name: %s age: %d " % (name1, age1))
print("name2: %s age2: %d " % (name2, age2))
##format
print("name: {} age: {} ".format(name1, age1))
print("name2: {} age2: {} ".format(name2, age2))
##format() QUIZ
Samsun_Open = "5,969,782,550"
Samsun_replace = Samsun_Open.replace(",","")
Samsun_Open_int = int(Samsun_replace)
print(Samsun_Open_int)

Point = "2020/03(E) (IFRS_Connect)"
print(Point[:7])

data = "    Samsung    "
data_replace = data.replace(" ", "")
print(data_replace)
data_strip = data.strip()
print(data_strip)
