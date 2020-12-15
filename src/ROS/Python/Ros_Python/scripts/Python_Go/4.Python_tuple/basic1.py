#!/usr/bin/env python
import rospy


#example 71
my_varialbe = ()
print(type(my_varialbe))

#example 72
movie_rank = ("dark", "split", "lucky")
print(movie_rank)

#example 73
tupp = (1,)
print(type(tupp))

#example 74
t = (1, 2, 3)
#t[0] = 'a'

#example 75
t = 1, 2, 3, 4
print(type(t))

#example 76
t = ('a', 'b', 'c')
print(t)
t = ('A', 'B', 'C')
print(t)

#example 77, 78
interest = ('samsung', 'LG', 'SK') ##tuple
print(type(interest))
data = list(interest)	##tuple->list change
print(type(data))

interest2 = ['samsung', 'LG', 'SK'] ##List
print(type(interest2))
data = tuple(interest2) ##List->tuple change
print(type(data))

#example 79
temp = ('apple', 'banana', 'cake')
a, b, c = temp
print(a, b, c) ## apple, banana, cake

#example 80
data = tuple(range(2, 100, 2)) ##range(start_num, end_num, upcount)
print(data)

data2 = tuple(range(100, 2, -2))
print(data2)


