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

// size for the array
#define SIZE 144

// define for the distance
#define th 0.5

// variable to determine the manual drive
bool manual = false;

// define publishers:
// pub for the goal 
ros::Publisher pub;
// pub to cancel the goal
ros::Publisher pubCancel;
// pub to publishthe velocity
ros::Publisher pubV;

// define a variable to publish
move_base_msgs::MoveBaseActionGoal pose;

// define a variable to publish
geometry_msgs::PointStamped poseStamped;

// define a string to save the goal id
std::string goalID;

// define a variable for reading fields of the goal to cancel
actionlib_msgs::GoalID goalToCancel;

// define a variable to publish the new velocity
geometry_msgs::Twist n;

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

// function to take the velocity: always available the current vel of the robot
void takeVel(const geometry_msgs::Twist::ConstPtr& m)
{
	if(!manual)
	{
		pubV.publish(m);
	}
	n.linear.x = m -> linear.x;
	n.angular.z = m -> angular.z;
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

// function to calculate the minimum distance among array values
double min_val(double a[])
{
	// setting dist as the maximum for the laser so that there cannot be errors
	double dist = 30;
	
	// test each element of the array
	for(int i = 0; i < SIZE; i++)
	{
		// check if the value is less than the distance
		if(a[i] < dist)
		{
			// update tge distance with the value
			dist = a[i];
		}
	}
	
	// return the distance found as minimum
	return dist;
}

// menu for the drive manual
void menuManual()
{
	std::cout << "\n###################### INFOS ######################\n";
	std::cout << "\nEnter:\nm to manual drive;\nq to quit\n";
	std::cout << "###################################################\n";
	std::cout << "\nType here: ";
}

// manual drive
void drive()
{
	char inputUsr;
	while(inputUsr != 'q')
	{
		switch(inputUsr)
		{
			// manual drive inserted
			case 'm':
				manual = true;
				// print
				std::cout << "Manual drive inserted.\n";
				break;
			
			// exit the program
			case 'q':
				manual = false;
				// stop the manual drive and stop the robot too
				n.linear.x = 0;
				n.angular.z = 0;
				pubV.publish(n);
				
			// invalid input
			default:
				std::cout << "Invalid input.";
				break;
		}
	}
}

// function to assist the robot driving
void driveAssist(const sensor_msgs::LaserScan::ConstPtr &m)
{
	// check if the manual drive is on or not
	if(!manual)
	{
		//return because it is off
		return;
	}
	
	// declare an array of dimension of the ranges (720)
	float r[m->ranges.size()];
	
	// fill the new array to make the data available
	for(int i = 0; i < m->ranges.size(); i++)
	{
		r[i] = m->ranges[i];
	}
	
	// use all the value for the sector so divid them into 144 array (720/5)
	
	// right
	double right[SIZE];
	// front rigth
	double fr_right[SIZE];
	// front
	double front[SIZE];
	// front left
	double fr_left[SIZE];
	// left
	double left[SIZE];
	
	// fill the right
	for(int i = 0; i < SIZE; i++)
	{
		right[i] = r[i];
	}
	// fill the front right
	for(int i = 0; i < SIZE; i++)
	{
		fr_right[i] = r[i+SIZE];
	}
	// fill the front
	for(int i = 0; i < SIZE; i++)
	{
		fr_right[i] = r[i+2*SIZE];
	}
	// fill the front left
	for(int i = 0; i < SIZE; i++)
	{
		fr_right[i] = r[i+3*SIZE];
	}
	// fill the left
	for(int i = 0; i < SIZE; i++)
	{
		fr_right[i] = r[i+4*SIZE];
	}
	
	// check if there is an obstacle in the front
	if(min_val(front) < th)
	{
		// if the robot has to go straight
		if(n.linear.x > 0 && n.angular.z == 0)
		{
			// stop the robot
			n.linear.x = 0;
		}
	}
	
	// check the distance on the right with the front right array
	if(min_val(fr_right) < th)
	{
		// if the robot has to turn on the right
		if(n.linear.x > 0 && n.angular.z < 0)
		{
			// stop the robot
			n.linear.x = 0;
			n.linear.z = 0;
		}
	}
	
	// check the distance on the right
	if(min_val(right) < th)
	{
		// if the robot has to turn on the right
		if(n.linear.x == 0 && n.angular.z < 0)
		{
			// stop the robot
			n.linear.z = 0;
		}
	}
	
	// check the distance on the left with the front left arrat
	if(min_val(fr_left) < th)
	{
		// if the robot has to turn on the right
		if(n.linear.x > 0 && n.angular.z > 0)
		{
			// stop the robot
			n.linear.x = 0;
			n.linear.z = 0;
		}
	}
	
	// check the distance on the left
	if(min_val(left) < th)
	{
		// if the robot has to turn on the right
		if(n.linear.x == 0 && n.angular.z > 0)
		{
			// stop the robot
			n.linear.z = 0;
		}
	}
	
	// publish the new velocity
	pubV.publish(n);
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

			// get the value by the user
			std::cin >> inX >> inY;
			
			// clear the output to print again in a white background
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
	
	// advertise the topic prov_cmd_vel
	pubCancel = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// subscribe to the topic feedback to have the status always available and updated
	ros::Subscriber sub = nh.subscribe("move_base/feedback", 1, takeStatus);
	
	// subscribe to the topic prov_cmd_vel to have the value of the velocity
	ros::Subscriber subL = nh.subscribe("/cmd_vel", 1, takeVel);
	
	// subscribe to the topic scan to have the value of the laser to avoid obstacles
	ros::Subscriber subV = nh.subscribe("/base_scan", 1, driveAssist);
	
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
