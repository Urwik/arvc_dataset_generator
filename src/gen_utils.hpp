#include <iostream>
#include <eigen3/Eigen/Dense>


#include <filesystem>
#include <ros/package.h>
#include <yaml-cpp/yaml.h>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"  
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"


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
        model_base model[arg];
    }

    public:    
        string world_name;
        bool insert_models;
        bool dynamic_models;
        model_base model[5];   
    };


    class labeled
    {
    labeled(int arg)
    {
        model_base model[num_models];   
    }
    private:
        int num_models;

    public:    
        bool insert_models;
        bool dynamic_models;
        model_base model[5];   
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
        bool enable;
        string name;
        string topic;
        string path;
    };

    class camera
    {
    public:
        bool enable;
        string name;
        string topic;
        string path;
    };

    class output_data
    {
    public:
        bool enable;
        string out_dir;
        int quantity;
        bool pc_binary;
    };


    class config
    {
        config()
        {
            parse_config();
        }
          
    public:
        simulation sim;
        sensor sensors;
        camera cameras;
        output_data out_data;

        environment env;
        labeled lab;



        void parse_config()
        {
            namespace fs = std::filesystem;
            
            fs::path package_path(ros::package::getPath("arvc_dataset_generator"));
            fs::path config_path = package_path / "config/dataset_generator_config.yaml";

            std::cout << YELLOW << "YAML CONFIG PATH: " << RESET << "\n " << config_path.string().c_str() << std::endl;

            YAML::Node config = YAML::LoadFile(config_path.string());

            // SIMULATION
            this->sim.paused = config["simulation"]["paused"].as<bool>();
            this->sim.debug_msgs = config["simulation"]["debug_msgs"].as<bool>();
            this->sim.log_msgs = config["simulation"]["log_msgs"].as<bool>();

            // SENSORS
            this->sensors.enable = config["sensors"]["enable"].as<bool>();
            this->sensors.name = config["sensors"]["name"].as<string>();
            this->sensors.topic = config["sensors"]["topic"].as<string>();
            this->sensors.path = config["sensors"]["path"].as<string>();

            // CAMERAS
            this->cameras.enable = config["cameras"]["enable"].as<bool>();
            this->cameras.name = config["cameras"]["name"].as<string>();
            this->cameras.topic = config["cameras"]["topic"].as<string>();
            this->cameras.path = config["cameras"]["path"].as<string>();

            // OUTPUT DATA
            this->out_data.enable = config["output_data"]["enable"].as<bool>();
            this->out_data.out_dir = config["output_data"]["out_dir"].as<string>();
            this->out_data.quantity = config["output_data"]["quantity"].as<int>();
            this->out_data.pc_binary = config["output_data"]["pc_binary"].as<bool>();

            // ENVIRONMENT
            this->env.world_name = config["environment"]["world_name"].as<string>();
            this->env.insert_models = config["environment"]["insert_models"].as<bool>();
            this->env.dynamic_models = config["environment"]["dynamic_models"].as<bool>();

            for (int i = 0 ; i <  sizeof(this->env.model) / sizeof(arvc::environment::model) ; i++)
            {
                this->env.model[i].name = config["environment"]["model"][i]["name"].as<string>();
                this->env.model[i].path = config["environment"]["model"][i]["path"].as<string>();
                this->env.model[i].num_models = config["environment"]["model"][i]["num_models"].as<int>();
                this->env.model[i].rand_mode = config["environment"]["model"][i]["rand_mode"].as<string>();
                this->env.model[i].min_scale = config["environment"]["model"][i]["min_scale"].as<Eigen::Vector3f>();
                this->env.model[i].max_scale = config["environment"]["model"][i]["max_scale"].as<Eigen::Vector3f>();
                this->env.model[i].negative_offset = config["environment"]["model"][i]["negative_offset"].as<Eigen::Vector3f>();
                this->env.model[i].positive_offset = config["environment"]["model"][i]["positive_offset"].as<Eigen::Vector3f>();
                this->env.model[i].positive_dist = config["environment"]["model"][i]["positive_dist"].as<Eigen::Vector3f>();
                this->env.model[i].negative_dist = config["environment"]["model"][i]["negative_dist"].as<Eigen::Vector3f>();
                this->env.model[i].rotation_range = config["environment"]["model"][i]["rotation_range"].as<Eigen::Vector3f>();
            }
                



        }

    };

  //////////////////////////////////////////////////////////////////////////////


}