#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "final_assignment/Service.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "stdio.h"
#include "move_base_msgs/MoveBaseActionGoal.h"

// define a publisher 
ros::Publisher pub;

// define a variable to publish
move_base_msgs::MoveBaseActionGoal pose;

//defining the variables to store the input
float inX,inY;	

// switch to choose what to do given a certain input
bool setDriveMod (final_assignment::Service::Request &req, final_assignment::Service::Response &res)
{
	switch(req.input)
	{
		// publish a position (x,y)
		case '1':
			// give some instructions
			std::cout << "Enter a postiion with the 'x y' format.\nUse a space for distinguishing the coordinates.\nUse a dot . for the decimal (enter i.d even if d=0):";

			// get the value
			std::cin >> inX >> inY;
			
			system("clear");
			
			// set the value to publish
			pose.goal.target_pose.pose.position.x = inX;
			pose.goal.target_pose.pose.position.y = inY;
			
			pose.goal.target_pose.header.frame_id = "map";
			pose.goal.target_pose.pose.orientation.w = 1;
	
			break;
		
		// drive the robot with the move_base topic
		case '2':
			break;
			
		// drive the robot with the teleop_twist_kwyboard
		case '3':
			break;
			
		// kill all nodes
		case '0':
			ros::shutdown();
			break;
			
		// invalid input
		default:
			std::cout << "Invalid input.";
			break;
	}
	
	// publish the target
	pub.publish(pose);
	
	return true;
}

// main
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "service");
	// defininf a node handle
	ros::NodeHandle nh;
	// advertise the topic and call the function
	pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1);
	
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// spin the program
	ros::spin();
	
	return 0;
}
