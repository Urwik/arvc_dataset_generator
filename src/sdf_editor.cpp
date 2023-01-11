#include <filesystem>
#include <iostream>
#include <Eigen/Eigen>
#include <gazebo/gazebo.hh>
#include <sdf/sdf.hh>

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{

  fs::path sdfPath;
  fs::path current_path = fs::current_path();
  sdfPath = current_path / argv[1];
  std::cout << "Path to file: " << sdfPath << std::endl;

  sdf::SDFPtr sdf_File (new sdf::SDF());
  sdf::init(sdf_File);
  sdf::readFile(sdfPath, sdf_File);

  sdf::ElementPtr modelElement = sdf_File->Root()->GetElement("model");
  sdf::ElementPtr linkElement = modelElement->GetElement("link");

  while (linkElement)
  {
    std::string linkName = linkElement->GetAttribute("name")->GetAsString();
    std::cout << "Modificando el link: " << linkName << std::endl;
    sdf::ElementPtr visualElement = linkElement->GetElement("visual");

    sdf::ElementPtr retroElement;

    if(!visualElement->GetElement("laser_retro")){
      std::cout << "No existe el elemento laser retro" << std::endl;
      retroElement = visualElement->AddElement("laser_retro");
    }
    else{
      std::cout << "Existe el elemento laser retro" << std::endl;
      retroElement = visualElement->GetElement("laser_retro");
      std::cout << "Parent element: " << retroElement->GetParent()->GetName() << std::endl;
      }

    retroElement->Set<double>(1);
    linkElement = linkElement->GetNextElement("link");
  }
  

  return 0;
}

