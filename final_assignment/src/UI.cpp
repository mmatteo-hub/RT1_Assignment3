#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "geometry_msgs/Twist.h"

// defining a client
ros::ServiceClient client;

// function to show the menù and give the input
void callBack()
{
	// defining a variable s_srv of type second_assignment::Service
	second_assignment::Service s_srv;
	
	// defining a char to use to store the input
	char inputUsr;
	
	// getting the keyboard input
	std::cin >> inputUsr;

	// put the input on the request of the server
	s_srv.request.input = inputUsr;
	// waut for the existance of the server
	client.waitForExistence();
	// call the server
	client.call(s_srv);
}

// main
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "UI");
	// defining a node handle
	ros::NodeHandle nh;
	// call the service with the client
	client = nh.serviceClient<second_assignment::Service>("/service");
	
	// spin the prorgram in this way since there is no topic to subscribe to to spin with ros::spin()
	while(ros::ok())
	{
		// call the function to show the menù and take the input at each iteration
		callBack();
		ros::spinOnce();
	}
	
	return 0;
}
