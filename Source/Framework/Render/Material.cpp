#include "Render/Material.h"

SPtr<Material> Material::Create(const String &name)
{
    return Memory::MakeShared<Material>(name);
}

Material::Material(const String &name) : name(name)
{   
}