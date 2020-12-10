#!/usr/bin/env python

import rospy

#example 61
price = ['20180728', 100, 130, 140, 150, 160, 170]

print(price)

#example 62
nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
print(nums[::2])

#example 63
nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
print(nums[1::2])

#example 64
nums = [1, 2, 3, 4, 5]
print(nums[::-1])

#example 65
interest = ['samsung', 'LG', 'Naver']
print(interest[0], interest[2])

#example 66
interest.append("Sk")
interest.append("future")

print(interest)

#example 67
print("/".join(interest))

#example 68
print("\n".join(interest))

#example 69
string = "samsung/LG/Naver"

print(string)

#example 70
data = [2, 4, 3, 1, 5, 10, 9]

data.sort()
print(data)

