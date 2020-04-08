#include "Application/Application.h"

class LearnVK : public Logic
{
public:
    void OnLoad() override
    {

    }

    void OnUnload() override
    {

    }

    void OnUpdate() override
    {

    } 
};

LearnVK learnVK;
Logic *gLogic = &learnVK;