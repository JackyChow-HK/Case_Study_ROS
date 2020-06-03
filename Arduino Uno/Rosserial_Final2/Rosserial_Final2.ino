

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif
#include <stdlib.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <Servo.h>
#include <std_msgs/String.h>
#include <NewPing.h>


ros::NodeHandle nh;

geometry_msgs::Twist msg;


////////////Define

#define SONAR_NUM 2     // Number of sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 100


////////////Variables

char show_result[8]; // Buffer big enough for 7-character float
char sonar_id[8]; // Buffer big enough for 7-character float  
char log_msg[16];


unsigned long pingTimer[SONAR_NUM]; 
unsigned int detect_dist[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;   

float move1;

/////////////Library
Servo steering;
Servo bd_motor;

//NewPing sonar(2, 3, MAX_DISTANCE);

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
 NewPing(2, 3, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
 NewPing(8, 9, MAX_DISTANCE)
};


void callback(const geometry_msgs::Twist& cmd_vel)
{
  move1 = cmd_vel.linear.x;
  //move2 = cmd_vel.angular.z;
 
  
  if (move1 > 0  ) // forward
  {
    bd_motor.write(117);// forward   
     nh.loginfo("Foward");
    }
  else  if (detect_dist[0] < 25 && detect_dist[1] < 25 && move1 == 0  )  //right  sensor detected steering distance
    {   
       bd_motor.write(70);// Backward
       nh.loginfo("Backward");
         } 
         
    
  else  //stop
  {
    bd_motor.write(100);
    nh.loginfo("stop");
   
    }

}
ros::Subscriber <geometry_msgs::Twist> sub ("/RCC/control/cmd_vel", callback);

//ros::Subscriber <geometry_msgs::Twist> sub ("/cmd_vel", callback);


void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  nh.initNode();
  nh.subscribe(sub);
   
  bd_motor.attach(10); 
  pingTimer[0] = millis() + 100; // 
  pingTimer[1] = pingTimer[0] + PING_INTERVAL +290;          
 }

  
void loop() { 

     for (uint8_t i = 0; i < SONAR_NUM; i++) 

   { // Loop through all the sensors.
    
    if (millis() >= pingTimer[i]) 
    { 
      pingTimer[i] += PING_INTERVAL * SONAR_NUM +580 ; // PING_INTERVAL  B2 [0]& [1] = 600 /2  = 300ms
      sonar[currentSensor].timer_stop();    
      delay(20);
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle();  
      delay(125); 
      steering.detach();
      delay(20);  
      nh.spinOnce();
      delay(125);    
      currentSensor = i;                          // Sensor being accessed.
      detect_dist[currentSensor]  = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
      
    }
 
  }
  

  
}

void echoCheck() { // If ping received, set the sensor distance to array.
  
  if (sonar[currentSensor].check_timer()){
    
     detect_dist[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
     
     dtostrf(detect_dist[currentSensor], 6, 2, show_result); // Leave room for too large numbers
     
     dtostrf(currentSensor, 2, 0, sonar_id); // Leave room for too large numbers!
     
     sprintf(log_msg,"sonar[%s] = %s", sonar_id, show_result);
    
     nh.loginfo(log_msg);

 
  }
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  // The following code would be replaced with your code that does something with the ping results.

       if (detect_dist[0] != NULL && detect_dist[1] != NULL) {
        if (detect_dist[0] < 100 && detect_dist[0] < detect_dist[1])  //left sensor detected steering distance
         {   
            steering.attach(11);
            steering.write(40);
              
            nh.loginfo("Turn Right");
         }
        
        else  if (detect_dist[1] < 100 && detect_dist[1] < detect_dist[0])  //right  sensor detected steering distance
         {
               steering.attach(11);
            steering.write(120);
      
           nh.loginfo("Turn Left");
         } 
   
        else {
           steering.attach(11);
          steering.write(75);
          
            nh.loginfo("Neutral");
         }
           
       }
  
   
}
