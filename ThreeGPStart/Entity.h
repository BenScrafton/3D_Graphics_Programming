#pragma once
#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Model.h"
#include <string>

struct EntityData
{
	std::vector<Model> models;
	std::vector<std::vector<std::string>> textures;
	std::vector<glm::vec3> translations;
};

class Entity
{
private:
	std::vector<Model> models;
public:
	Entity(std::vector<Model> models, std::vector<std::vector<std::string>> textures)
	{
		//Model model 
		//models.push_back("Data\\Models\\AquaPig\\gun.obj");
		//models.push_back("Data\\Models\\AquaPig\\gun_base.obj");
		//models.push_back("Data\\Models\\AquaPig\\hull.obj");
		//models.push_back("Data\\Models\\AquaPig\\propeller.obj");
		//models.push_back("Data\\Models\\AquaPig\\wing_left.obj");
		//models.push_back("Data\\Models\\AquaPig\\wing_right.obj");

		//models.push_back
	}
};

