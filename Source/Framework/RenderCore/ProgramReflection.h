#pragma once

#include "RenderCore/DescriptorSet.h"

namespace RenderCore
{

struct ProgramReflectionDesc
{

};

class ProgramReflection
{
public:

    static SPtr<ProgramReflection> Create(const ProgramReflectionDesc &desc);

};


}