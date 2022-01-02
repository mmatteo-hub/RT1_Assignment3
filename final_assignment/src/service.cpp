#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "final_assignment/Service.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "stdio.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "geometry_msgs/PointStamped.h"

// define a publisher 
ros::Publisher pub;

// define a variable to publish
move_base_msgs::MoveBaseActionGoal pose;

// define a variable to publish
geometry_msgs::PointStamped poseStamped;

// switch to choose what to do given a certain input
bool setDriveMod (final_assignment::Service::Request &req, final_assignment::Service::Response &res)
{
	switch(req.input)
	{
		// publish a position (x,y)
		case '1':
			// give some instructions
			std::cout << "Enter a position with the 'x y' format.\nUse a space for distinguishing the coordinates.\nUse a dot . for decimal coordinates.\nType here: ";

			//defining the variables to store the input
			float inX,inY;

			// get the value
			std::cin >> inX >> inY;
			
			system("clear");
			
			// set the value to publish
			pose.goal.target_pose.pose.position.x = inX;
			pose.goal.target_pose.pose.position.y = inY;
			
			pose.goal.target_pose.header.frame_id = "map";
			pose.goal.target_pose.pose.orientation.w = 1;
			
			// publish the target
			pub.publish(pose);
	
			break;
			
		// drive the robot with the teleop_twist_kwyboard
		case '2':
			
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
	
	return true;
}

// main
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "service");
	// defininf a node handle
	ros::NodeHandle nh;
	// advertise the topic
	pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1);
	
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// spin the program
	ros::spin();
	
	return 0;
}
