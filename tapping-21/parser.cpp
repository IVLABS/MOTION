//date:12/9/16
//written by-sakshi
//receives data from csv file and published on a node with custom msg int64[10]
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "readCSV.cpp"
#include "motion/angles_in.h"
#include "motion/angles_out.h"

int main(int argc, char **argv)
{
   readCSV file;//object of class readCSV
   std::vector<std::string> data;// vector,data to store returned vector by storeCSV2array
   int row=119;//rows that we want to read from csv file
   int col=20;//col that we want to read from csv file
 
   data=file.storeCSV2array("/home/sakshi/.ros/roll_8_change_swing.csv",row,col);//by default,the path is taken from .ros
   ros::init(argc, argv, "parser");//node name=angle_parser
   ros::NodeHandle n;
   ros::Publisher pub_ang = n.advertise<motion::angles_out>("angle_to_motor", 10);//topic name=angle_to_mo
   motion::angles_in arr;
   motion::angles_out arr1;
   int no_of_cycles=10;
   int k=0;
   ros::Time begin = ros::Time::now();
   while (ros::ok())
   {
  // if (k<no_of_cycles)
  //{
      ros::Rate loop_rate(100);
      for(int i=0;i<row;i++)
        { 
          std::cout<<i<<std::endl;
          std::vector<float> vect;
          std::stringstream ss(data.at(i));
          float m;
          while (ss >> m)
           {
             vect.push_back(m);
             if (ss.peek() == ',')
                ss.ignore();
           }
          for (int h=0; h<10; h++)
           { 
            vect.at(h) = vect.at(h)+vect.at(h+10);
            arr.angle_to_motor_in[h]=vect.at(h); 
           // ROS_INFO("%f",arr.angle_to_motor_in[0]);
           }
          for (int h=0; h<10; h++)
           {
            arr.angle_to_motor_in[h]=vect.at(h) ; 

            float a = (arr.angle_to_motor_in[h]+150);

            arr1.angle_to_motor_out[h]=round(21+(a/300)*981);

           }
         // if (i==25)
         pub_ang.publish(arr1);


         loop_rate.sleep();


        }
     ros::spinOnce();
     k++;
//double secs_current =ros::Time::now().toSec();
 //ROS_INFO("%f",secs_current);  
    //}   
   }

return 0;
}
