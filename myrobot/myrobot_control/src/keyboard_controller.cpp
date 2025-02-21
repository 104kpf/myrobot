#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

class Node{

    public:    
        Node() {
            // Publisher for sending Float64MultiArray messages
            command_pub = nh.advertise<std_msgs::Float64MultiArray>("/real_robot_arm_joint", 10);

            // Initialize data with zero values
            for (int i = 0; i < 5; ++i)
                command_msg.data.push_back(0.0);

            run();
            
        }

    private:
        // Variables
        ros::NodeHandle nh;
        ros::Publisher command_pub;
        std_msgs::Float64MultiArray command_msg;
        float joint1 = 0;
        float joint2 = 0;
        float joint3 = 0;
        float joint4 = 0;
        float EEF_state = 0;

        // Main function to run
        void run() {
            // Rate at which to publish messages
            ros::Rate loop_rate(20);  // Adjust the rate as needed
            while (ros::ok()) {
                // Read keyboard input
                int key = getch();

                // Determine which joint to control based on keyboard input
                bool check = false;
		if (key == 'a') {
		    joint1 += 0.1;
                    check = true;
                }
                else if (key == 'd') {
		    joint1 -= 0.1;
                    check = true;
                }
		else if (key == 'w') {
		    joint2 += 0.1;
                    check = true;
                }
		else if (key == 's') {
		    joint2 -= 0.1;
                    check = true;
                }
		else if (key == 'i') {
		    joint3 += 0.1;
                    check = true;
                }
		else if (key == 'k') {
		    joint3 -= 0.1;
                    check = true;
                }
		else if (key == 'j') {
		    joint4 += 0.1;
                    check = true;
                }
		else if (key == 'l') {
		    joint4 -= 0.1;
                    check = true;
                }
                else if (key == 'z') {
		    joint1 = 0;
                    check = true;
                }
		else if (key == 'x') {
		    joint2 = 0;
                    check = true;
                }
		else if (key == 'c') {
		    joint3 = 0;
                    check = true;
                }
		else if (key == 'v') {
		    joint4 = 0;
                    check = true;
                }
        else if (key == 'm') {
		    EEF_state = 1.0;
                    check = true;
                }
        else if (key == 'n') {
		    EEF_state = 0.0;
                    check = true;
                }
        else if (key == 'e') {
		    
            joint1 = 0;
            joint2 = 0;
            joint3 = 0;
            joint4 = 0;
                    check = true;
                }

                //pos ristrict
                if(joint1>1.2)
                {
                    joint1 =1.2;
                }
                if(joint2>1.4)
                {
                    joint2 =1.4;
                }
                if(joint3>1.7)
                {
                    joint3 =1.7;
                }
                if(joint4>1.6)
                {
                    joint4 =1.6;
                }


                if(-1.2>joint1)
                {
                    joint1 =-1.2;
                }
                if(-1.4>joint2)
                {
                    joint2 =-1.4;
                }
                if(-1.7>joint3)
                {
                    joint3 =-1.7;
                }
                if(-1.6>joint4)
                {
                    joint4 =-1.6;
                }

                // Clear previous data
                command_msg.data.clear();

                // Initialize data with zero values
                for (int i = 0; i < 5; ++i)
                    command_msg.data.push_back(0.0);

                // Fill in message
                command_msg.data[0] = joint1;
                command_msg.data[1] = joint2;
                command_msg.data[2] = joint3;
                command_msg.data[3] = joint4;
                command_msg.data[4] = EEF_state;


                // Publish the message if any joint command is given
                if (check) {
                    command_pub.publish(command_msg);
                    ROS_WARN("Command sent: [%f, %f, %f, %f, %f]", command_msg.data[0], command_msg.data[1], command_msg.data[2], command_msg.data[3], command_msg.data[4]);
                }

                // Spin once to handle callbacks
                ros::spinOnce();

                // Sleep to maintain the specified rate
                loop_rate.sleep();
            }
        }
 
        // Function to read keyboard input without waiting for Enter key
        int getch() {
            static struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            int ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return ch;
        }
};

int main(int argc, char **argv) {
    // Initialize ROS node
    printf("press key to start, press 'e' to reset robot position");
    ros::init(argc, argv, "keyboard_controller");
    Node node;
    ros::spin();
    return 0;
} 
