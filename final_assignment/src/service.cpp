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

// define publishers:
// pub for the goal 
ros::Publisher pub;
// pub to cancel the goal
ros::Publisher pubCancel;

// define a variable to publish
move_base_msgs::MoveBaseActionGoal pose;

// define a variable to publish
geometry_msgs::PointStamped poseStamped;

// define a string to save the goal id
std::string goalID;

// define a variable for reading fields of the goal to cancel
actionlib_msgs::GoalID goalToCancel;

// flag to enable the target
int flag = 0;

// function to set the parameters to the right field of the variable to publish
void setPoseParams(float inX, float inY)
{
	// set the value (x y) to the x and y field of the variable pose
	pose.goal.target_pose.pose.position.x = inX;
	pose.goal.target_pose.pose.position.y = inY;
			
	// set the frame_id
	pose.goal.target_pose.header.frame_id = "map";
	
	// set the quaternion module equal to 1
	pose.goal.target_pose.pose.orientation.w = 1;
	
	// publish the target chosen
	pub.publish(pose);
}

// function to cancel the goal by the user input
void cancelGoal()
{
	// set the goal id to cancel equat to the actual goalID
	goalToCancel.id = goalID;
	// publish to cancel
	pubCancel.publish(goalToCancel);
}

// function to cancel the goal by the timer
void cancelGoalTimer(const ros::TimerEvent)
{
	// set the goal id to cancel equat to the actual goalID
	goalToCancel.id = goalID;
	// publish to cancel
	pubCancel.publish(goalToCancel);
}

// function to take the status: in particular the actual goal id
void takeStatus(const move_base_msgs::MoveBaseActionFeedback::ConstPtr& msg)
{
	// set the goalID variable with the value of the actual goal id
	goalID = msg -> status.goal_id.id;
}

// menu to display inside the switch
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
			// set the flag value to 1 to enable the timer creation
			flag = 1;
			
			// give some instructions
			menu();

			//defining the variables to store the input
			float inX,inY;

			// get the value
			std::cin >> inX >> inY;
			
			system("clear");
			
			// function to set the params
			setPoseParams(inX,inY);
	
			break;
			
		// drive the robot with the teleop_twist_kwyboard
		case '2':
			
			break;
			
		// delete the current goal
		case '3':
			// set the flag value to 1 to enable the timer creation
			flag = 1;
			
			// call the function to cancel the goal
			cancelGoal();
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
	// defining a node handle
	ros::NodeHandle nh;
	
	// advertise the topic
	pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1);
	
	// advertise the topic move_base/cancel
	pubCancel = nh.advertise<actionlib_msgs::GoalID>("move_base/cancel", 1);
	
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// subscribe to the topic feedback to have the status always available and updated
	ros::Subscriber sub = nh.subscribe("move_base/feedback", 1, takeStatus);
	
	// timer is created inside the main only if the program enters the case 1 and 3 of the setDriveMod function
	if(flag)
	{
		// create a timer and iniziatilse it with 1 minute counter
		ros::Timer timer = nh.createTimer(ros::Duration(60.0),cancelGoalTimer);
	}
	
	// set the value to 0 again to avoid the creation of another timer before the necessary
	// in this way every time the program enters the case 1 or 3 the timer is re-created starting again the 1 minute counter
	flag = 0;
	
	// spin the program
	ros::spin();
	
	return 0;
}
