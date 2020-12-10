#!/usr/bin/env python
#slicing
import rospy

url = "http://sharebook.kr"
print(url[17::])

url_split = url.split('.')
print(url_split[-1])

word1 = "1._If_you_do_not_walk_today,_you_will_have_to_run_tomorrow?"
word_split = word1.split(',')
print(word_split[-1])

word1 = "1._If_you_do_not_walk_today,_you_will_have_to_run_tomorrow,hho?"
word_split = word1.split(',')
print(word_split[-1])
