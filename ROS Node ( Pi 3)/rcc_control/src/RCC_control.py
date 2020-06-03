#!/usr/bin/python
"""
Sonar array composed of 3 sonars (left, center, right)

Subscribes to the three topics 0 (left) 1(center) 2 (right)

calculates a correction to the cmd_vel that is:
    multiplicative for x
    additive for angle

"""
import math, time
import rospy
from sensor_msgs.msg import Range
from geometry_msgs.msg import Twist


DIST_BREAK          = 1.4


def saturate(value, min, max):
    if value <= min: return(min)
    elif value >= max: return(max)
    else: return(value)

class ObstAvoid():
    def __init__(self):
        
        self.range_center   = 3

        
        self.sub_center = rospy.Subscriber("/SONAR/sonar_dist", Range, self.update_range)

        rospy.loginfo("Subscribers set")
        
        self.pub_twist = rospy.Publisher("/RCC/control/cmd_vel", Twist, queue_size=5)
        rospy.loginfo("Publisher set")
        
        self._message = Twist()
        
        #self._time_steer        = 0
        #self._steer_sign_prev   = 0
        
    def update_range(self, message):
		self.range_center = message.range

    def get_control_action(self):
        
        break_action   = 0.0
        steer_action   = 0.0
        
        #--- Get the minimum distance
        #range   = min([self.range_center, self.range_left, self.range_right])
        
        if self.range_center > DIST_BREAK:
    		break_action   = 1
                rospy.loginfo("Forward %.1f"%break_action)

                     
        if steer_action == 0.0 and  break_action   == 0.0:
                rospy.loginfo("Break %.1f ,%.1f"%(break_action, steer_action))
				
            
        return (break_action, steer_action)
        
    
        
    def run(self):
        
        #--- Set the control rate
        rate = rospy.Rate(5)

        while not rospy.is_shutdown():
            #-- Get the control action
            break_action, steer_action = self.get_control_action()
            
            #-- update the message
            self._message.linear.x  = break_action
            self._message.angular.z = steer_action
            
            #-- publish it
            self.pub_twist.publish(self._message)

            rate.sleep()        
            
if __name__ == "__main__":

    rospy.init_node('obstacle_avoid')
    
    obst_avoid     = ObstAvoid()
    obst_avoid.run()            
