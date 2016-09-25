#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"

ros::Subscriber sub;
ros::Publisher  pub;
ros::Publisher  pub_takeoff;
ros::Publisher  pub_land;
ros::Publisher  pub_reset;

void sendEmptyMsg(ros::Publisher &pPub) {
    std_msgs::Empty msg;
    pPub.publish(msg);
}

void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg) {
    float sl_left_right = msg->axes[0];
    float sl_up_down = msg->axes[1];
    float LT = msg->axes[2];
    float sr_left_right = msg->axes[3];
    float sr_up_down = msg->axes[4];
    float RT = msg->axes[5];
    float button_a = msg->buttons[0];
    float button_back = msg->buttons[6];

    if (!(button_a)) {
        //float crosskey_left_right = msg->axes[6];
        //float crosskey_up_down = msg->axes[7];
        geometry_msgs::Twist cmd_vel;
        cmd_vel.linear.x = round(sl_up_down*10.0)/10.0;      //move foward backward
        cmd_vel.linear.y = round(sl_left_right*10.0)/10.0;   //move left right
        cmd_vel.linear.z = round(sr_up_down*10.0)/10.0;       //move up_down
        cmd_vel.angular.z =round(sr_left_right*10.0)/10.0;   //turn right left
        pub.publish(cmd_vel);

        if (LT == -1.0)
            sendEmptyMsg(pub_takeoff);
        else if (RT == -1.0)
            sendEmptyMsg(pub_land);

        if (button_back == 1)
            sendEmptyMsg(pub_reset);
    }
    else {
        geometry_msgs::Twist cmd_vel;
        cmd_vel.linear.x = 0.0;   //move foward backward
        cmd_vel.linear.y = 0.0;   //move left right
        cmd_vel.linear.z = 0.0;   //move up_down
        cmd_vel.angular.z =0.0;   //turn right left
        pub.publish(cmd_vel);
    }
}


int main(int argc, char **argv) {

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  sub = n.subscribe("joy", 1000, chatterCallback);
  pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  pub_takeoff = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
  pub_land = n.advertise<std_msgs::Empty>("/ardrone/land", 1);
  pub_reset = n.advertise<std_msgs::Empty>("/ardrone/reset", 1);
  ros::spin();

  return 0;
}
