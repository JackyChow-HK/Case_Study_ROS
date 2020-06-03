#!/usr/bin/python
# https://www.rahner-edu.de/raspberry-pi/der-sensor-hc-sr04/

import pigpio
import time
import rospy
from sensor_msgs.msg import Range

"""
sensor_msgs/Range

uint8 ULTRASOUND=0
uint8 INFRARED=1
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
uint8 radiation_type
float32 field_of_view
float32 min_range
float32 max_range
float32 range

"""
counter = 1

class Sonar():
    def __init__(self, SONAR_CENTER_GPIO_TRIG, SONAR_CENTER_GPIO_ECHO,range_min=10, range_max=400):

        self.SONAR_CENTER_GPIO_ECHO = SONAR_CENTER_GPIO_ECHO
        self.SONAR_CENTER_GPIO_TRIG = SONAR_CENTER_GPIO_TRIG
        self._range_min = range_min
        self._range_max = range_max

        pi.set_mode(SONAR_CENTER_GPIO_TRIG, pigpio.OUTPUT)
        pi.set_mode(SONAR_CENTER_GPIO_ECHO, pigpio.INPUT)

        
        self.distance_publisher = rospy.Publisher("/SONAR/sonar_dist", Range, queue_size=10)
	rospy.loginfo("Publisher set")
        # --- Default message
        message = Range()
        message.radiation_type = 0
        message.min_range = range_min
        message.max_range = range_max
        self._message = message

    def scan(self):
        max_time = 0.04

        pi.write(self.SONAR_CENTER_GPIO_TRIG, 1)
        time.sleep(0.5)
        pi.write(self.SONAR_CENTER_GPIO_TRIG, 0)
        time.sleep(0.00001)
        pi.write(self.SONAR_CENTER_GPIO_TRIG, 1)

        tStart = time.time()
        timeout = tStart + max_time
        while pi.read(self.SONAR_CENTER_GPIO_ECHO) == 0 and tStart < timeout:
            tStart = time.time()

        tStop = time.time()
        timeout = tStop + max_time
        while pi.read(self.SONAR_CENTER_GPIO_ECHO) == 1 and tStop < timeout:
            tStop = time.time()

        DeltaT = tStop - tStart
        distance = DeltaT * 17240

        if distance > self._range_max:
            distance = self._range_max

        if distance < self._range_min:
            distance = self._range_min
	
        return(distance)

    def run(self):
        # --- Set the control rate
        rate = rospy.Rate(5)

        rospy.loginfo("Running...")
        while not rospy.is_shutdown():
            range_cm =  self.scan()
            global counter 

            if counter == 1  :
      	   	 acceptable  =   range_cm
      	   	 counter = -counter
		 i = 0	
		 #rospy.loginfo("set counter")

            larger_cm =   acceptable   + 50
            smaller_cm =  acceptable   - 50

            if  range_cm >= ( larger_cm) or  range_cm <= (smaller_cm) :
		
		if  i < 2:
			range_cm = acceptable
			i = i +1
       	   	 	#rospy.loginfo("3. Replace = %4.1f cm , i =%4.1f "%(range_cm,i)	)
		else:
			
			acceptable  =   range_cm
			i = 0
       	   	 	#rospy.loginfo("4. Acceptable = %4.1f cm , Range =%4.1f , set i =%4.1f"%(range_cm,acceptable ,i)	)
            else :
      	   	 acceptable  =   range_cm
      	   	 i = 0
      	   	 #rospy.loginfo("2 . get acceptable = %4.1f cm , %4.1f cm, %4.1f cm , i = %4.1f " %(acceptable, larger_cm,smaller_cm,i))
	
      	   	 
	    rospy.loginfo("   ----------- Distance = %4.1f cm " % range_cm )
            self._message.range = range_cm * 0.01
            self.distance_publisher.publish(self._message)

            rate.sleep()

        rospy.loginfo("Stopped")


if __name__ == '__main__':
    rospy.init_node('sonar')
    SONAR_CENTER_GPIO_TRIG = 12
    SONAR_CENTER_GPIO_ECHO = 16
    pi = pigpio.pi()
    
    try:
        sonar = Sonar(SONAR_CENTER_GPIO_TRIG, SONAR_CENTER_GPIO_ECHO)
        sonar.run()
    except rospy.ROSInterruptException:
        pass

