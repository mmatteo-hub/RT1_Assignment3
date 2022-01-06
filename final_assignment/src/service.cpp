#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "final_assignment/Service.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "stdio.h"
#include "string.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "geometry_msgs/PointStamped.h"
#include "move_base_msgs/MoveBaseActionFeedback.h"

// define publishers 
ros::Publisher pub;
ros::Publisher pubCancel;

// define a variable to publish
move_base_msgs::MoveBaseActionGoal pose;

// define a variable to publish
geometry_msgs::PointStamped poseStamped;

// define a string to save the goal id
std::string goalID;

// define a variable for reading fields of the goal to cancel
actionlib_msgs::GoalID goalToCancel;

void setPoseParams(float inX, float inY)
{
	// set the value (x y) to publish
	pose.goal.target_pose.pose.position.x = inX;
	pose.goal.target_pose.pose.position.y = inY;
			
	// set the frame_id
	pose.goal.target_pose.header.frame_id = "map";
	
	// set the quaternion module equal to 1
	pose.goal.target_pose.pose.orientation.w = 1;
}

void cancelGoal()
{
	goalToCancel.id = goalID;
}

void takeStatus(const move_base_msgs::MoveBaseActionFeedback::ConstPtr& msg)
{
	goalID = msg -> status.goal_id.id;
}

void menu()
{
	std::cout << "\n###################### INFOS ######################\n";
	std::cout << "Enter a position with the 'x y' format.\nUse a space for distinguishing the coordinates.\nUse a dot . for decimal coordinates.\n";
	std::cout << "\nCartesian axes are set positive as follow:\n";
	std::cout << "\t^\n\t|\n\t|\n\t|\n\t|\n\t°---------->\nO = (0 0)\n";
	std::cout << "###################################################\n";
	std::cout << "\nType here: ";
}

// switch to choose what to do given a certain input
bool setDriveMod (final_assignment::Service::Request &req, final_assignment::Service::Response &res)
{
	switch(req.input)
	{
		// publish a position (x y)
		case '1':
			// give some instructions
			menu();

			//defining the variables to store the input
			float inX,inY;

			// get the value
			std::cin >> inX >> inY;
			
			system("clear");
			
			// function to set the params
			setPoseParams(inX,inY);
			
			// publish the target chosen
			pub.publish(pose);
	
			break;
			
		// drive the robot with the teleop_twist_kwyboard
		case '2':
			break;
			
		// delete the current goal
		case '3':
			// call the function to cancel the goal
			cancelGoal();
			// publish to cancel
			pubCancel.publish(goalToCancel);
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
	
	// advertise the topic move_base/cancel
	pubCancel = nh.advertise<actionlib_msgs::GoalID>("move_base/cancel", 1);
	
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// subscribe to the topic feedback to have the status always available and updated
	ros::Subscriber sub = nh.subscribe("move_base/feedback", 1, takeStatus);
	
	// spin the program
	ros::spin();
	
	return 0;
}
