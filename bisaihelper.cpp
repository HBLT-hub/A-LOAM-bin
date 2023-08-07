#include <math.h>
#include <vector>
#include <aloam_velodyne/common.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>
#include <eigen3/Eigen/Dense>
#include <ceres/ceres.h>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
using namespace pcl;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "bisaihelper");
    ros::NodeHandle nh;
    ros::Publisher pubLaserCloudFullmy = nh.advertise<sensor_msgs::PointCloud2>("/velodyne_cloud_my", 100);
    ros::Rate rate(5);
    ifstream infile("/home/zcf/桌面/slambisai/202210131557/name.txt",ios::in);
    string str; // 定义字符数组用来接受读取一行的数据

       while (ros::ok()) 
     {
        ros::spinOnce();
        if(!infile){  // 判断文件是否存在
      cerr<<"open error."<<endl;
      exit(1); // 退出程序
    }
 
    // while(infile)
    // {
        if(infile)
        getline(infile,str);  // getline函数可以读取整行并保存在str数组里
        // cout<<str<<endl;
    // }
       string bin_file_path = "/home/zcf/桌面/slambisai/202210131557/PointClouds/"+str.substr(0,20);
       cout<<bin_file_path<<endl;

       	fstream input(bin_file_path.c_str(), ios::in | ios::binary);
        	if(!input.good()){
		         cerr << "Could not read file: " << bin_file_path << endl;
		        exit(EXIT_FAILURE);
	       }
	        input.seekg(0, ios::beg);

        pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
        int i;
	for (i=0; input.good() && !input.eof(); i++) {
		pcl::PointXYZI point;
		input.read((char *) &point.x, 3*sizeof(float));
		input.read((char *) &point.intensity, sizeof(float));
		cloud->push_back(point);
	}
	input.close();
      pcl::io::savePCDFileBinary("/home/zcf/A-LOAM_ws/1.pcd", *cloud);
      
       double secs=stod(str.substr(0,16))/1e6;
      sensor_msgs::PointCloud2 laserCloudFullmy;
      pcl::toROSMsg(*cloud, laserCloudFullmy);
      laserCloudFullmy.header.stamp = ros::Time(secs);
      cout<<secs<<endl;
      laserCloudFullmy.header.frame_id = "/camera";
      pubLaserCloudFullmy.publish(laserCloudFullmy);

        cout<<"111"<<endl;
        rate.sleep();
     }


    return 0;
}
