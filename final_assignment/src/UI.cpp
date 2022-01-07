#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "final_assignment/Service.h"
#include "move_base_msgs/MoveBaseGoal.h"

// defining a client
ros::ServiceClient client;

// define a publisher 
ros::Publisher pub;

void menu()
{
	std::cout << "\n###################### MENU' ######################";
	std::cout << "\nPress:\n1 to publish a target with (x,y);\n2 to drive the robot with keyboard;\n3 to delete the current goal;\n0 to stop the program execution.\n\nIMPORTANT: The goal will be automatically cancelled after 60 seconds\n";
	std::cout << "###################################################\n";
}

// function to show the menù and give the input
void callBack()
{
	// defining a variable s of type final_assignment::Service
	final_assignment::Service s;
	
	// define a variable to publish
	move_base_msgs::MoveBaseGoal goal;
	
	// defining a char to use to store the input
	char inputUsr;
	
	// show the menù
	menu();
	
	// getting the keyboard input
	std::cin >> inputUsr;

	system("clear");

	// put the input on the request of the server
	s.request.input = inputUsr;
	// waut for the existance of the server
	client.waitForExistence();
	// call the server
	client.call(s);
}

// main
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "UI");
	// defining a node handle
	ros::NodeHandle nh;
	
	// call the service with the client
	client = nh.serviceClient<final_assignment::Service>("/service");
	
	// spin the prorgram
	while(ros::ok())
	{
		callBack();
		ros::spinOnce();
	}
	
	return 0;
}
