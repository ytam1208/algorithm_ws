#!/usr/bin/python3

print("Hello, World!")

family = ['mother', 'father', 'gentleman', 'sexy lady']

print(len(family))

family.remove('father')

print(family)

family.remove('sexy lady')

print(family)

print("Input num = ")
# num = input()
num = int(input())

while num <= 100:
    print(num)
    num += 1

number = 358
rem = rev = 0
while number >= 1:
    rem = number % 10
    rev = rev * 10 + rem
    number = number // 10

print(rev)