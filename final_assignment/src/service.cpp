#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "final_assignment/Service.h"

// switch to choose what to do given a certain input
bool setDriveMod (final_assignment::Service::Request &req, final_assignment::Service::Response &res)
{
	switch(req.input)
	{
		// publish a position (x,y)
		case '1':
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
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// spin the program
	ros::spin();
	
	return 0;
}
