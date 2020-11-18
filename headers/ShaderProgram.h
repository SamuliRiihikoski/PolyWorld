#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"
#include <map>


using std::string;

class ShaderProgram 
{
public:

    ShaderProgram();
    ~ShaderProgram();

    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    bool loadShader(const char* vsFilename, const char* fsFilename);
    void use();

    void setUniform(const GLchar* name, const glm::vec2& v);        
    void setUniform(const GLchar* name, const glm::vec3& v);
    void setUniform(const GLchar* name, const glm::vec4& v);
    void setUniform(const GLchar* name, const glm::mat4& m);

    GLuint getProgram()const;


private:

    string fileToString(const string& filename);
    void checkCombileErrors(GLuint shader, ShaderType type);
    GLint getUniformLocation(const GLchar* name);

    GLuint mHandle;
    std::map<string, GLint> mUniformLocations;

};

inline ShaderProgram::ShaderProgram() : mHandle(0)
{

}

inline ShaderProgram::~ShaderProgram() 
{
    glDeleteProgram(mHandle);
}

inline bool ShaderProgram::loadShader(const char* vsFilename, const char* fsFilename)
{
    string vsString = fileToString(vsFilename);
    string fsString = fileToString(fsFilename);
    
    const GLchar* vsSourcePtr = vsString.c_str();
    const GLchar* fsSourcePtr = fsString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSourcePtr, NULL);
    glShaderSource(fs, 1, &fsSourcePtr, NULL);

    glCompileShader(vs);
    checkCombileErrors(vs, VERTEX);
    glCompileShader(fs);
    checkCombileErrors(fs, FRAGMENT);

    mHandle = glCreateProgram();
    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);
    glLinkProgram(mHandle);

    checkCombileErrors(mHandle, PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

inline void ShaderProgram::use()
{
    if (mHandle > 0) 
        glUseProgram(mHandle);
} 

inline string ShaderProgram::fileToString(const string& filename) 
{
    std::stringstream ss;
    std::ifstream file;

    try
    {
        file.open(filename, std::ios::in);

        if (!file.fail())
            ss << file.rdbuf();
    }
    catch (std::exception ex)
    {
        std::cerr << "Error reading shader filename!" << std::endl;
    }

    return ss.str();
}

inline void ShaderProgram::checkCombileErrors(GLuint shader, ShaderType type)
{
    int status;

    if (type == PROGRAM)
    {
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
            std::cerr << "Error! Program failed to link. " << errorLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
        }
    }
    
}

inline GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
    std::map<string, GLint>::iterator it = mUniformLocations.find(name);

    if (it == mUniformLocations.end())
        mUniformLocations[name] = glGetUniformLocation(mHandle, name);

    return mUniformLocations[name];
}

inline void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) 
{
    GLuint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}        

inline void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) 
{
    GLuint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}      
inline void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) 
{
    GLuint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}      
inline void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m) 
{
    GLuint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}      

#endif
