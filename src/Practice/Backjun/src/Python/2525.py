#!/usr/bin/env python3

hour, min = map(int, input().split())
end_min = int(input())

time = hour + end_min//60 
time_mi = min + end_min%60

while time_mi > 59:
    time_mi -= 60
    hour += 1

while hour > 23:
    hour -= 24

print(int(hour), int(time_mi))
    