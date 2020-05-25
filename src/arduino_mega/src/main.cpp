#include <ros/ros.h>
#include "arduino_mega/arduino_mega.hpp"

int main(int argc, char **argv)
{
    ros::init(argc, argv,"arduino_mega");
    ros::NodeHandle n;

    ROS_INFO("%s", ros::this_node::getName().c_str());

    Arduino_mega arduino_mega(n);

    arduino_mega.runLoop();
    
    return 0;
}