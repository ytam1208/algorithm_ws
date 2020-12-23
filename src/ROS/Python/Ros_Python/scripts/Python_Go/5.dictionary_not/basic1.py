#!/usr/bin/env python

import rospy
from functools import reduce

#dictionary example dumy
lux = [490, 334, 550, 18.72]
lux = {'health': 490, 'mana':334, 'melee':550, 'armor':18.72}
print(lux)

tuple1 = (1, 2, 3, 4)
tuple2 = (5, 6, 7, 8)

dic = {tuple2, tuple1}
print(dic)

#list1 = ["1", "2", "3", "4"]
#list2 = ["5", "6", "7", "8"]

#dic2 = {list1, list2}

#print(dic2)


#example 81
scores = [8.8, 8.9, 8.7, 9.2, 9.3, 9.7, 9.9, 9.5, 7.8, 9.4]

*valid_score = scores

print(valid_score)

