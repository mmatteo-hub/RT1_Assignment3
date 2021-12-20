#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "second_assignment/Service.h"

// defining a variable to 
std_srvs::Empty reset;

// variable to pass the velocity change
float change_term = 0;

// switch to choose what to do given a certain input
bool setVelocityFnc (second_assignment::Service::Request &req, second_assignment::Service::Response &res)
{
	switch(req.input)
	{
		// publish a position (x,y)
		case 1:
			break;
		
		// drive the robot with the move_base topic
		case 2:
			break;
			
		// drive the robot with the teleop_twist_kwyboard
		case 3:
			break;
			
		// kill all nodes
		case 0:
			ros::shutdown();
			break;
			
		// invalid input
		default:
			std::cout << "Invalid input.";
			break;
	}
	
	return true;
}

// mian
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "server");
	// defininf a node handle
	ros::NodeHandle nh;
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setVelocityFnc);
	
	// spin the program
	ros::spin();
	
	return 0;
}
