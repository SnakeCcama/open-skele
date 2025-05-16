#pragma once
#include "gl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
    public:

    //Shader program ID
    unsigned int ID;

    //string objects to recieve shader source code
    std::string vsCode, fsCode;
    
    /*Constructor; 
    Immediately class is initialized, I want to obtain code from  shader files*/
    Shader(const char* vsPath, const char* fsPath)
    {
        //ifstream objects to read from shader source files
        std::ifstream vsFile, fsFile;

        //stringstreams to get source code
        std::stringstream vsStream, fsStream;

        //set exceptions
        vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


        try
        {
            //open source code
            vsFile.open(vsPath);
            fsFile.open(fsPath);

            //write to string stream
            vsStream << vsFile.rdbuf();
            fsStream << fsFile.rdbuf();
            vsFile.close();
            fsFile.close(); 

            //assign to string objects
            vsCode = vsStream.str();
            fsCode = fsStream.str();


        } catch (...)
        {
            std::cout <<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        //add source code as c-style string
        const char* vShCode = vsCode.c_str();
        const char* fShCode = fsCode.c_str();


        //Now openGl shader stuff
        unsigned int vert, frag;
        int success;
        char infoLog[512];

        /*Vertex Shader*/
        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vShCode, NULL);
        glCompileShader(vert);
        
        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vert, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        /*Fragment Shader*/
        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fShCode, NULL);
        glCompileShader(frag);
        
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(frag, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }        

        /*Shader program*/
        ID= glCreateProgram();
        glAttachShader(ID, vert);
        glAttachShader(ID, frag);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vert);
        glDeleteShader(frag);

    }

    void Use()
    {
        glUseProgram(ID);
    }


    /*uniform setter functions (depends on value type you want to set)
    these member functions are const-qualified (barred from modifying variables)*/

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value );
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec3(const std::string &name, const glm::vec3 &vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setthisMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void cleanUp()
    {
        glDeleteProgram(ID);
    }

};