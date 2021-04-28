#pragma once

#include "glm.hpp"
#include "ext.hpp"

struct Light
{
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
