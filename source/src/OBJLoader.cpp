#include "OBJLoader.h"

std::vector<Vertex> OBJLoader::loadOBJ(const char* file_name)
{
  //vertices
  std::vector<glm::fvec3> vertex_positions;
  std::vector<glm::fvec3> vertex_normals;
  std::vector<glm::fvec2> vertex_texcoords;

  //faces
  std::vector<GLuint> vertex_positions_indices;
  std::vector<GLuint> vertex_texcoords_indices;
  std::vector<GLuint> vertex_normals_indices;

  //Vertex
  std::vector<Vertex> vertices;

  std::stringstream ss;
  std::ifstream in_file(file_name);
  std::string line;
  std::string prefix;

  glm::vec3 temp_vec3;
  glm::vec2 temp_vec2;
  GLuint temp_gluint;

  //error check
  if(!in_file.is_open())
  {
    MessageBoxW(NULL, L"ERRER::OBJLOADER::Could not open *.obj file", L"Error", MB_OK | MB_ICONERROR);
    //something else?
  }

  //one line at a time
  while(std::getline(in_file, line))
  {
    ss.str(line);
    ss >> prefix; //first character


    if(prefix == "v") //vertex position
    {
      ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
      vertex_positions.push_back(temp_vec3);
    }
    else if(prefix == "vt")
    {
      ss >> temp_vec2.x >> temp_vec2.y;
      vertex_texcoords.push_back(temp_vec2);
    }
    else if(prefix == "vn")
    {
      ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
      vertex_normals.push_back(temp_vec3);
    }
    else if(prefix == "f")
    {
      
    }
    else
    {
      //everything that is not vertex data
    }


  }

  in_file.close();

  //success
  return vertices;
}
