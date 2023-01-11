// C++
#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <algorithm>
#include <vector>

// PCL
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/file_io.h>
#include <pcl/io/io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

// ROS
#include <ros/ros.h>

namespace fs = std::filesystem;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"  
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

//****************************************************************************//
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////

typedef pcl::PointXYZI PointT;
typedef pcl::PointCloud<PointT> PointCloud;

// GLOBAL VARIABLES
pcl::PCDReader pcd_reader;
pcl::PLYReader ply_reader;
std::vector<int> total_n_points;
int expected_n_points;
std::vector<std::string> error_cloud_names;


int check_cloud(fs::path input_file)
{
  PointCloud::Ptr pc (new PointCloud);
  std::string file_ext = input_file.extension();
  int correct = 0;

  if (file_ext == ".pcd")
    correct = pcd_reader.read(input_file.string(), *pc);
  else if (file_ext == ".ply")
   correct =  ply_reader.read(input_file.string(), *pc);
  else
    return -1;


  if (correct == 0)
  {
    if (pc->points.size() != expected_n_points)
      error_cloud_names.push_back(input_file.stem().string());

    total_n_points.push_back(pc->points.size());
  }
  else
    std::cout << "Error in Cloud: " << input_file.stem().string() << std::endl;

  return 0;
}



int main(int argc, char **argv)
{

  ros::init(argc, argv, "check_dataset");
  ros::NodeHandle nh;

  if(argc == 3)
    expected_n_points = std::stoi(argv[2]);
  else
    expected_n_points = 25000;
  

  // Parses arguments
  if(argc >= 2){
    fs::path input_dir = argv[1];
    
    // Iterates throught the directory passed as an argument
    if(fs::is_directory(input_dir))
    {
      for(const auto &entry : fs::directory_iterator(input_dir))
        check_cloud(entry.path());
    }
    // Checks one cloud passed as an argument
    else if (fs::is_regular_file(input_dir))
    {
      check_cloud(input_dir);
    }
    else
    {
      ROS_ERROR("Wrong argument. It should be a abs path to a directory or abs " 
      "path to a file. If no argument is passed node will get data from current "
      "directory.");
    }
  }

  // Iterates throught current directory
  else
  {
    fs::path current_path = fs::current_path();
    
    for(const auto &entry : fs::directory_iterator(current_path))
      check_cloud(entry.path());
  }

  int total = 0;

  for(int point : total_n_points)
    total += point;

  double mean = total / total_n_points.size();
  auto max = *std::max_element(total_n_points.begin(), total_n_points.end());
  auto min = *std::min_element(total_n_points.begin(), total_n_points.end());


  std::cout << "Max: " << max << std::endl;
  std::cout << "Min: " << min << std::endl;
  std::cout << "Mean: " << mean << std::endl;


  if(max != expected_n_points || min != expected_n_points || mean != expected_n_points)
  {
    std::cout << RED << "Dataset contains errors" << RESET << std::endl;
    std::cout << "Clouds with not enough points" << std::endl;
    for(auto name : error_cloud_names)
      std::cout << name << std::endl;
  }
  else
    std::cout << GREEN << "Dataset is correct!" << RESET << std::endl;

  ros::spinOnce();

  return 0;
}
