#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <Windows.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "vertex.h"

class OBJLoader
{
public:
  static std::vector<Vertex> loadOBJ(const char* file_name);

};

