#include <iostream>
#include <eigen3/Eigen/Dense>


#include <filesystem>
#include <ros/package.h>
#include <yaml-cpp/yaml.h>

using namespace std;

namespace arvc{
    class model_base
    {
    public:
        string name;
        string path;
        int num_models;
        string rand_mode;
        Eigen::Vector3f min_scale;
        Eigen::Vector3f max_scale;
        Eigen::Vector3f negative_offset;
        Eigen::Vector3f positive_offset;
        Eigen::Vector3f positive_dist;
        Eigen::Vector3f negative_dist;
        Eigen::Vector3f rotation_range;
        
    };



    class environment
    {
    environment(int arg)
    {
        num_models = arg;
    }
    private:
        int num_models;

    public:    
        string world_name;
        bool insert_models;
        bool dynamic_models;
        model_base models[num_models];   
    };


    class labeled
    {
    labeled(int arg)
    {
        num_models = arg;
    }
    private:
        int num_models;

    public:    
        bool insert_models;
        bool dynamic_models;
        model_base models[num_models];   
    };

    class simulation
    {
    public:
        bool paused;
        bool debug_msgs;
        bool log_msgs;
    };

    class sensor
    {
    public:
        string name;
        string topic;
        string path;
    };

    class camera
    {
    public:
        string name;
        string topic;
        string path;
    };

    class output_data
    {
    public:
        string out_dir;
        int quantity;
        bool pc_binary;
    };


    class config
    {
    public:
        environment env;
        labeled lab;

        simulation sim;
        sensor sensors;
        camera cameras;
        output_data out_data;
    };

  //////////////////////////////////////////////////////////////////////////////
  YAML::Node get_config()
  {
    namespace fs = std::filesystem;
    
    fs::path package_path(ros::package::getPath("arvc_dataset_generator"));
    fs::path config_path = package_path / "config/dataset_generator_config.yaml";

    std::cout << YELLOW << "YAML CONFIG PATH: " << RESET << "\n " << config_path.string().c_str() << std::endl;

    this->config = YAML::LoadFile(config_path.string());
  }



}