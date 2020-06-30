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
    int32 index = time < c.lastUpdateTime ? 0 : c.lastKeyframe;
    while (index < c.keyframes.Count() - 1)
    {
        if (c.keyframes[index + 1].time > time)
            break;
        index++;
    }

    int32 nextIndex = index + 1;
    if (nextIndex == c.keyframes.Count())
        nextIndex = 0;

    c.lastUpdateTime = time;
    c.lastKeyframe = index;

    return Interpolate(c.keyframes[index], c.keyframes[nextIndex], time);
}

int32 Animation::AddChannel(int32 matrixID)
{
    channels.Add(Channel(matrixID));
    return channels.Count() - 1;
}

void Animation::AddKeyframe(int32 index, const Keyframe &keyframe)
{
    CT_CHECK(keyframe.time <= duration);

    channels[index].lastKeyframe = 0;
    auto &frames = channels[index].keyframes;

    for (int32 i = 0; i < frames.Count(); ++i)
    {
        if (frames[i].time == keyframe.time)
        {
            frames[i] = keyframe;
            return;
        }

        if (frames[i].time > keyframe.time)
        {
            frames.Insert(i, keyframe);
            return;
        }
    }
    frames.Add(keyframe);
}

const Animation::Keyframe &Animation::GetKeyframe(int32 index, AnimTimeType time) const
{
    for (auto &e : channels[index].keyframes)
    {
        if (e.time == time)
        {
            return e;
        }
    }
    // Just throw an exception
    return channels[index].keyframes[-1];
}

bool Animation::TryGetKeyframe(int32 index, AnimTimeType time, Keyframe &out) const
{
    for (auto &e : channels[index].keyframes)
    {
        if (e.time == time)
        {
            out = e;
            return true;
        }
    }
    return false;
}

void Animation::Animate(AnimTimeType totalTime, Array<Matrix4> &matrices)
{
    auto time = Math::Fmod(totalTime, duration);
    for (auto &c : channels)
    {
        matrices[c.matrixID] = AnimateChannel(c, time);
    }
}