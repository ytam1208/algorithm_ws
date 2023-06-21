#!/usr/bin/env python

import rospy

if __name__=='__main__':
    try:
        print("hi")
    except rospy.ROSInterruptException:
        pass