#pragma once

#include "Render/.Package.h"

class Animation
{
public:
    using AnimTimeType = double;

    struct Keyframe
    {
        AnimTimeType time = 0;
        Vector3 translation;
        Vector3 scaling{ 1.0f, 1.0f, 1.0f };
        Quat rotation;
    };

    struct Channel
    {
        Channel(int32 matrixID)
            : matrixID(matrixID)
        {
        }

        int32 matrixID;
        Array<Keyframe> keyframes;
        int32 lastKeyframe = 0;
        AnimTimeType lastUpdateTime = 0;
    };

public:
    Animation(AnimTimeType duration);

    int32 AddChannel(int32 matrixID);
    void AddKeyframe(int32 index, const Keyframe &keyframe);
    const Keyframe &GetKeyframe(int32 index, AnimTimeType time) const;
    bool TryGetKeyframe(int32 index, AnimTimeType time, Keyframe &out) const;
    void Animate(AnimTimeType totalTime, Array<Matrix4> &matrices);

    int32 GetChannelCount() const
    {
        return channels.Count();
    }

    void SetName(const String &newName)
    {
        name = newName;
    }

    const String &GetName() const
    {
        return name;
    }

    static SPtr<Animation> Create(AnimTimeType duration);

private:
    Matrix4 AnimateChannel(Channel &c, AnimTimeType time);
    Matrix4 Interpolate(const Keyframe &start, const Keyframe &end, AnimTimeType time) const;

private:
    String name;
    Array<Channel> channels;
    AnimTimeType duration = 0;
};