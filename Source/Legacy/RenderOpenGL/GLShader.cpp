#include "RenderOpenGL/GLShader.h"
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

int32 GLShader::GetUniformLocation(const String &name)
{
    auto valPtr = uniformLocations.TryGet(name);
    if(valPtr)
        return *valPtr;

    int32 location = glGetUniformLocation(id, StringEncode::UTF8::ToChars(name).GetData());
    uniformLocations.Put(name, location);
    return location;
}

void GLShader::SetInt(const String &name, int32 value)
{
    int32 location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void GLShader::SetFloat(const String &name, float value)
{
    int32 location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void GLShader::SetVector2(const String &name, const Vector2 &value)
{
    int32 location = GetUniformLocation(name);
    glUniform2f(location, value.x, value.y);
}

void GLShader::SetVector3(const String &name, const Vector3 &value)
{
    int32 location = GetUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
}

void GLShader::SetVector4(const String &name, const Vector4 &value)
{
    int32 location = GetUniformLocation(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void GLShader::SetMatrix4(const String &name, const Matrix4 &value)
{
    int32 location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value.GetPtr());
}

void GLShader::SetColor(const String &name, const Color &value)
{
    int32 location = GetUniformLocation(name);
    glUniform4f(location, value.r, value.g, value.b, value.a);
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
        CT_EXCEPTION(Render, "Undefined shader type: vertex.");
        return result;
    }
    if (fragPos == INDEX_NONE)
    {
        CT_EXCEPTION(Render, "Undefined shader type: fragment.");
        return result;
    }

    result.vertex = src.Substring(vertexPos + vertexPrefix.Length(), fragPos - vertexPos - vertexPrefix.Length());
    result.fragment = src.Substring(fragPos + fragPrefix.Length());

    constexpr auto ProcessInclude = [](String &src) {
        auto pos = src.IndexOf(CT_TEXT("#include"));
        if (pos == INDEX_NONE)
            return false;

        int32 dqPos0, dqPos1;
        if (!src.Find(CT_TEXT('"'), pos, dqPos0))
        {
            CT_EXCEPTION(Render, "Parse include error!");
            return false;
        }
        if (!src.Find(CT_TEXT('"'), dqPos0 + 1, dqPos1))
        {
            CT_EXCEPTION(Render, "Parse include error!");
            return false;
        }
        String includePath = src.Substring(dqPos0 + 1, dqPos1 - dqPos0 - 1);
        IO::FileHandle includeFile(includePath);
        if (!includeFile.IsFile())
        {
            CT_EXCEPTION(Render, "Cannot find include file.");
            return false;
        }

        String includeSrc = includeFile.ReadString();
        src = src.Replace(pos, dqPos1 - pos + 1, includeSrc);
        return true;
    };

    while (ProcessInclude(result.vertex));
    while (ProcessInclude(result.fragment));

    return result;
}

bool GLShader::Compile(uint32 shaderID, const String &source)
{
    auto u8Str = StringEncode::UTF8::ToChars(source);
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