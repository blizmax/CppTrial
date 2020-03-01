#include "Render/OpenGL/GLShader.h"
#include "IO/FileHandle.h"

SPtr<Shader> Shader::Create(const String &path)
{
    return Memory::MakeShared<GLShader>(path);
}

GLShader::GLShader(const String &path)
{
    auto source = Parse(path);

    uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (Compile(vertexShader, source.vertex) && Compile(fragmentShader, source.fragment))
    {
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);

        if (!Link())
        {
            glDeleteProgram(id);
            id = 0;
        }
        else
        {
            CT_LOG(Debug, CT_TEXT("Init Shader succeed! Path :{0}"), path);
        }
        
        glDetachShader(id, vertexShader);
        glDetachShader(id, fragmentShader);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLShader::~GLShader()
{
    glDeleteProgram(id);
}

void GLShader::Bind() const
{
    glUseProgram(id);
}

void GLShader::Unbind() const
{
    glUseProgram(0);
}

GLShader::ShaderSource GLShader::Parse(const String &path)
{
    ShaderSource result;

    IO::FileHandle file(path);
    if (!file.IsFile())
    {
        CT_EXCEPTION(Render, "Cannot find source file.");
        return result;
    }

    String src = file.ReadString();
    String vertexPrefix = CT_TEXT("#type vertex");
    String fragPrefix = CT_TEXT("#type fragment");
    auto vertexPos = src.IndexOf(vertexPrefix);
    auto fragPos = src.IndexOf(fragPrefix);

    if (vertexPos == INDEX_NONE)
    {
        CT_EXCEPTION(Render, "Parse vertex source error!");
        return result;
    }
    if (fragPos == INDEX_NONE)
    {
        CT_EXCEPTION(Render, "Parse fragment source error!");
        return result;
    }

    result.vertex = src.Substring(vertexPos + vertexPrefix.Length(), fragPos - vertexPos - vertexPrefix.Length());
    result.fragment = src.Substring(fragPos + fragPrefix.Length());
    return result;
}

bool GLShader::Compile(uint32 shaderID, const String &source)
{
    auto u8Str = StringEncode::ToUTF8(source);
    const char *cstr = u8Str.GetData();

    glShaderSource(shaderID, 1, &cstr, NULL);
    glCompileShader(shaderID);

    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        Array<GLchar> infoStr;
        infoStr.AppendUninitialized(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoStr[0]);

        CT_LOG(Error, "Compile shader error! {0}", String(infoStr.GetData()));
        return false;
    }
    return true;
}

bool GLShader::Link()
{
    glLinkProgram(id);

    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
        Array<GLchar> infoStr;
        infoStr.AppendUninitialized(maxLength);
        glGetProgramInfoLog(id, maxLength, &maxLength, &infoStr[0]);

        CT_LOG(Error, "Link shader error! {0}", String(infoStr.GetData()));
        return false;
    }
    return true;
}