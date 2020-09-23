#include "Shader.h"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  char infoLog[512];
  string vertexCode;
  string fragmentCode;
  ifstream vShaderFile;
  ifstream fShaderFile;
  try
  {
    //otwieranie pliku z Shaderem
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    stringstream vShaderStream, fShaderStream;
    // zapis zawartoœci bufora w stream
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // zamkniêcie pliku Shadera
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(ifstream::failure e)
  {
    cout << "Error Shader File not succesfully read" << endl;
  }
  int success;
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
          glGetShaderInfoLog(vertex, 512, NULL, infoLog);
          MessageBoxW(NULL, L"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", L"ERROR", MB_OK | MB_ICONERROR);
        }
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
          glGetShaderInfoLog(fragment, 512, NULL, infoLog);
          MessageBoxW(NULL, L"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", L"ERROR", MB_OK | MB_ICONERROR);
        }
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
          glGetProgramInfoLog(ID, 512, NULL, infoLog);
          MessageBoxW(NULL, L"ERROR::SHADER::PROGRAM::LINKING_FAILED\n", L"ERROR", MB_OK | MB_ICONERROR);
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
}
