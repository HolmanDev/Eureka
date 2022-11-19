#include "pch.h"
#include "Shader.h"

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            int typeIndex = (int)type;
            if (typeIndex >= 0)
                ss[typeIndex] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); // Creates a shader and stores and ID from which it can be referenced
    const char* src = source.c_str(); // converts std::string to a c-style string (char*, a pointer to the first character in a string)
    glShaderSource(id, 1, &src, nullptr); // Fills the shader with the source code
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// Compile shaders and combine them. Takes them in as strings and converts them to a useable format
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); // Creates a program and returns a non-zero value by which it can be referenced.
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs)); //! Is this where we can add more shaders?
    GLCall(glLinkProgram(program)); // Links a program and creates executables
    GLCall(glValidateProgram(program)); // Checks if the executable can execute in the current OpenGL state

    // Delete intermediates because the shaders are stored elsewhere now
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint params1;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &params1);
    if (params1 != GL_TRUE) {
        std::cout << "ruh roh" << std::endl;
    }

    GLint params2;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params2);
    if (params2 != GL_TRUE) {
        std::cout << "ruh roh" << std::endl;
    }

    GLint params3;
    glGetProgramiv(program, GL_LINK_STATUS, &params3);
    if (params3 != GL_TRUE) {
        std::cout << "ruh roh" << std::endl;
    }

    return program;
}

// Uses the shader
void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    //if (location == -1)
    //    std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}
