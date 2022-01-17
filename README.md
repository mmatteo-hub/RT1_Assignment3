# Research Track: Assignment #3
## Robot simulator using ROS (Robotics Operative System) and C++.

### Professor [Carmine Recchiuto](https://github.com/CarmineD8), Student: [Matteo Maragliano](https://github.com/mmatteo-hub)

## Running the final code
To run the code there are few steps to do:
* install the `xterm` by typing `sudo apt instal xterm`: this is a useful tool which allows us to have multiple console windows opened to run our nodes.
* type the command to run the final code by typing `roslaunch final_assignment final_assignment.launch` and it can be seen that Gazebo, Rviz and three other windows will be opened (we will explain later their meaning).

## Goal of the assignmnet
The task for this assignment is to make a robot move around a circuit using ROS (Robotics Operative System) and C++ or Python: we decided to use the C++ language. The scope of the work is to drive a robot in different ways inside a circuit, not known by the robot at the beginning. The robot has to discover it by moving and can in this way build a map of the environmnet.

The program should allow the user to choose among different possibilities through a menù, structured as follows:
* 1 - publish a goal the robot has to reach;
* 2 - drive the robot manually with the possibility of having a driving assistence to avoid obstacles
* 3 - cancel a goal already set
* 4 - quit the program by turning off all nodes

### General description of the steps followed for the realization
* At the beginning we focalized on being able to move the robot in different ways, trying different modalitites of driving too
* Once each component worked properly we combined together all of them and built the final program.

## Elements in the project
#### Environment
The environment in which the robot has to move is a sort of house as it can be seen in the following pictures:
<img src="https://user-images.githubusercontent.com/62358773/149616513-6e7db4f1-c535-422c-abcd-b51214507905.jpg" width=45%, height=45%>

What the robot sees at the beginning of the execution is:

<img src="https://user-images.githubusercontent.com/62358773/149616590-1febce2a-1430-4153-904a-af2a20ae96a1.jpg" width=30%, height=30%>

The robot is provided of laser scan (the red lines in the picture) able to detect obstacles; it has the possibility of building the map it is moving into by the `gmapping` package which runs together with the simulation: as we can see the *white* part of the map is the one that has already been discovered by the robot.

In the figure it can be already seen the origin of the map, used in a Cartesian plane reference, in which the robot can hold its reference to locate itself in the environment.

#### Robot
The robot is

<img src="https://user-images.githubusercontent.com/62358773/149632556-e0e598cd-1287-404e-88b2-71a23a20fb0f.jpg" width=25%, height=25%>

which has 720 sensors distributed into the 180° frontal view.
The sensor are useful to detect obstacles and were divided into 5 different groups with the same size (144 in each one):
Starting from the 0-th numbered to the 719-th we have:
* right
* front right
* front
* front left
* left

They are used while the robot has to avoid obstacles, in particular we used them inside the manual drive function.

The robot has the possibility to drive into every direction, in particular
* go straight
* turn on its position (z axe) both on the right or on the left
* turn while going straight

## Flowchart of the program
While everything is running in the program, we can build a graph of all nodes connected with eachother to better understand the behaviour; this can be done automatically by the `rosrun rqt_graph rqt_graph` command. The output is:
<img src="https://user-images.githubusercontent.com/62358773/149840202-a3fd86f7-4573-4f52-9ea7-c1bc809b88ca.jpg" width=100%, height=100%>

## Structure of the code
The code has two main nodes:
* UI node
* service node

Here we summarise the main steps computed during the execution:

* The first node has the aim to take the user choice on the modality of driving;
* then it passes the input to the second node by a service;
* once the service node takes the input there is a switch to determine the correct modality for the robot drive.

In orde to avoid problems in terms of synchronization between the callback functioncs we decided to manage all the parts of the code requiring a user choice inside the *UI_node*; the only function that was left into the *service_node* is the one to insert the position of the goal to be reached.

This was done to prevent other threads to stop their execution while waiting for the user to insert a specific character to end that callback.

### Output of the code
##### Windows
When the program runs there are several windows opened, here we explain their meaning:
* Gazebo, a robot simulation environment in which we can already seen all the map for our robot
* Rviz, the window which allows the user see what the robot sees; in this one we cannot see the all map but only the part already discovered by the robot.
* 3 other windows, like terminals:
	* UI node, to print the menù and take the user choice;
	* service node, to print informations about the robot and take the user input for the goal to be rached;
	* teleop twist keyboard, a node running to allow the user drive the robot through the keyboard. This node is always active but can publish only when the manual drive modality in on, in all other cases it does not interfer with the program execution.

##### Map
The final map built by the robot is the following, as we can expect is

<img src="https://user-images.githubusercontent.com/62358773/149633437-857b4271-3597-4696-bff6-a2423921181e.jpg" width=25%, height=25%>

Once the robot receives the input to go to a specific goal it computes a previous path but it cannot know a-priori the presence of obstacles; only after having the all map it can compute an accurate path also avoiding obstacles.

As an example we put the comparison between the path computed, more or less, starting from the same position, in two cases: the first with the map to be discovered and the second with the all map already discovered:

<img src="https://user-images.githubusercontent.com/62358773/149633440-5efdcfaa-26ed-4fe7-94a8-a829c2bb5558.jpg" width=40%, height=40%> <img src="https://user-images.githubusercontent.com/62358773/149633439-68fe7334-809e-4bce-ba1a-cbf0c28af29a.jpg" width=40%, height=40%>

It can be clearly seen the difference between the two cases. Of course when the robot does not know exactly the map it re-computes the path every time a danger obstacle on its road is detected.

## Pseudocode
#### Distance from obstacle
This is the function used by the robot to determine the presence and the distance of an obstacle inside the map when it is discovered.
```cpp
dist = 30;
	
for(every element in the array)
	if(distance i-th less than my actual min dist)
		update the distance with this value
```

#### Driving assistance
This function allows the user have some prints to better understand what the robot sees inside the map and what can do or cannot. Using this function the robot is also stopped automatically when it is dangerously near an obstacle.

Sicne there is the possibility to disable it, those functionalities are not available when it is not used, so the robot may crash because of the non-presence of au automatic control.
```cpp
if(assistance driving enabled)
	fill the array

	if(distance in front less than th)
		if(robot has to go straight)
			stop the robot

	if(distance in front right less than th)
		if(robot has to go on the right while straight)
			stop the robot

	if(distance in right less than th)
		if(robot has to turn on the right)
			stop the robot

	if(distance in front left less than th)
		if(robot has to go on the left while straight)
			stop the robot

	if(distance in left less than th)
		if(robot has to turn on the left)
			stop the robot
```

If we check the main function we can see almost all the function used:
#### main: UI_node
```cpp
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "UI");
	// defining a node handle
	ros::NodeHandle nh;
	
	// call the service with the client
	client = nh.serviceClient<final_assignment::Service>("/service");
	
	// spin the prorgram
	// ros::spin() not used since there is not a topic to subscribe to which enables the spin mode
	
	while(ros::ok())
	{
		// call the function to manage the choice of the user for the behaviour of the robot
		callBack();
		ros::spinOnce();
	}
	
	return 0;
}
```
Inside the *callBack()* it is managed the user input choice and then processed by a *switch case*. It is not used the *ros::spin()* command since there is not a periodic topic to subscribe to.

Inside the *callBack()* function it is also called the function to activate the manual drive:
```cpp
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
	
	// check if the input is the manual drive one
	if(inputUsr == '2')
	{
		system("clear");
		// call the function to manage the choice for the manual drive
		manuallyDrive();
	}

	// clear the output to print again in a white background
	system("clear");

	// put the input on the request of the server
	s.request.input = inputUsr;
	// waut for the existance of the server
	client.waitForExistence();
	// call the server
	client.call(s);
}
```
The *manuallyDrive()* function is a switch in which it is called the client again and it is passed the option to have the *driving assistance* or not, thus avoiding the problems we talked about before.

#### main: service_node
```cpp
// main
int main(int argc, char ** argv)
{
	// initialising the node
	ros::init(argc, argv, "service");
	// defining a node handle
	ros::NodeHandle nh;
	
	// advertise the service
	// advertise the service and call the function
	ros::ServiceServer service = nh.advertiseService("/service", setDriveMod);
	
	// advertise topics
	// advertise the topic move_base/goal for setting the goal
	pub = nh.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal", 1);
	
	// advertise the topic move_base/cancel for cancelling the goal
	pubCancel = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 1);
	
	// advertise the topic cmd_vel
	pubV = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	
	// subscribe to the topic feedback to have the status always available and updated
	ros::Subscriber sub = nh.subscribe("/move_base/feedback", 1, takeStatus);
	
	// subscribe to the topic goal to have the current status always available and updated
	ros::Subscriber subG = nh.subscribe("/move_base/goal", 1, currGoal);
	
	// subscribe to the topic prov_cmd_vel to have the value of the velocity
	ros::Subscriber subV = nh.subscribe("/my_cmd_vel", 1, takeVel);
	
	// subscribe to the topic scan to have the value of the laser to avoid obstacles
	ros::Subscriber subL = nh.subscribe("/scan", 1, driveAssist);
	
	// spin the program
	ros::spin();
	
	return 0;
}
```
Here we have lots of different functions and topics we subscribed to, in particulare:
* setDriveMod: a function in which it is used a *switch case* to determine the choice (inserted and passed throug the client).

We put only the case in which we choose the goal:
```cpp
// switch to choose what to do given a certain input
bool setDriveMod (final_assignment::Service::Request &req, final_assignment::Service::Response &res)
{
	switch(req.input)
	{	
		// publish a position (x y)
		case '1':
			system("clear");
			
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
			
		// ... other cases not inserted here ...
```
... and here we put the function to set the parameters goal. It is important to set also the *frame_id* and and the *orientation* whose module has to be 1 *(x=y=z=0, w=1)*.
```cpp
// function to set the parameters to the right field of the variable to publish
void setPoseParams(float inX, float inY)
{
	move_base_msgs::MoveBaseActionGoal pose;

	// set the value (x y) to the x and y field of the variable pose
	pose.goal.target_pose.pose.position.x = inX;
	pose.goal.target_pose.pose.position.y = inY;
			
	// set the frame_id
	pose.goal.target_pose.header.frame_id = "map";
	
	// set the quaternion module equal to 1
	pose.goal.target_pose.pose.orientation.w = 1;
	
	// publish the target chosen
	pub.publish(pose);
	
	// set the goal flag
	G = true;
}
```
We do not put other code here but we explain the meaninf of each function
* takeStatus: used to haved always the position of the robot updated
* currGoal: used to have stored the input for the goal; it also implements the control for the robot to check if the goal has been reached.
* takeVel: used to have always the velocity parameters updated;
* driveAssist: used to have the assistance while driving the robot by keyboard enabled;
* cancelGoal: not present inside the main but very useful; it is called when the user wants to cancel a goal.

### Service structure
To have a general but complete vision of the code we put also the structure of the simple service implemented:
```cpp
// request
char input
```
It is composed only by a request in which it is passed the user choice.

## Future improvements
It is a simple structure for the code even if there are many function to manage. A possible improvement can be the possibility of storing the map already seen and make it available in a future simulation to optimize the time to find a path for a goal.
