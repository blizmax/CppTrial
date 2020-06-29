#include "Render/Animation.h"

SPtr<Animation> Animation::Create(AnimTimeType duration)
{
    return Memory::MakeShared<Animation>(duration);
}

Animation::Animation(AnimTimeType duration) : duration(duration)
{
}

Matrix4 Animation::Interpolate(const Keyframe &start, const Keyframe &end, AnimTimeType time) const
{
    Matrix4 m;
    //TODO
    return m;
}

Matrix4 Animation::AnimateChannel(Channel &c, AnimTimeType time)
{
    Matrix4 m;
    //TODO
    return m;
}

void Animation::Animate(AnimTimeType totalTime, Array<Matrix4> &matrices)
{
    auto time = Math::Fmod(totalTime, duration);
    for (auto& c : channels)
    {
        matrices[c.matrixID] = AnimateChannel(c, time);
    }
}

int32 Animation::AddChannel(int32 matrixID)
{
    channels.Add(Channel(matrixID));
    return channels.Count() - 1;
}