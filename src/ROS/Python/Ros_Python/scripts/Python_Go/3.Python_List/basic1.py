#!/usr/bin/env python

import rospy

#example 51
movie_rank = ["Dark", "Split", "luckcy"]
print(movie_rank)

#example 52
movie_rank.append("bat")
print(movie_rank)

#example 53 
movie_rank.insert(1, "super")
print(movie_rank)

#example 54
del movie_rank[3]
print(movie_rank)

#example 55
del movie_rank[2]
del movie_rank[2]
print(movie_rank)

#example 56
lang1 = ["C", "C++", "JAVA"]
lang2 = ["Python", "GO", "C#"]

lang3 = lang1 + lang2
print(lang3)

#example 57
nums = [1, 2, 3, 4, 5, 6, 7]

print("max: ", max(nums))
print("min: ", min(nums))

#example 58
nums = [1, 2, 3, 4, 5]

print(sum(nums))

#example 59
cook = ["pizza", "kimbab", "mandu", "chicken", "pizza2", "noodle"]

print(len(cook))

#example 60
nums = [1, 2, 3, 4, 5]
average = sum(nums) / len(nums)

print(average)
