#pragma once

#include "Render/.Package.h"
#include "Math/Quat.h"

class Animation
{
public:
    using AnimTimeType = float;

    struct Keyframe
    {
        AnimTimeType time = 0.0;
        Vector3 translation;
        Vector3 scaling{1.0f, 1.0f, 1.0f};
        Quat rotation;
    };

    struct Channel
    {
        Channel(int32 matrixID) : matrixID(matrixID)
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
    void AddKeyframe(int32 channelID, const Keyframe &frame);
    const Keyframe &GetKeyframe(int32 channelID, AnimTimeType time) const;
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