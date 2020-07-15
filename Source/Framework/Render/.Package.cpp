#include "Render/.Package.h"

MaterialData::MaterialData() : 
    base{1.0f, 1.0f, 1.0f, 1.0f},
    specular{0.0f, 0.0f, 0.0f, 0.0f},
    emissive{0.0f, 0.0f, 0.0f, 1.0f},
    alphaThreshold(0.5f),
    IoR(1.5f),
    specularTransmission(0.0f),
    flags(0)
{
}

LightData::LightData() : 
    posW{0.0f, 0.0f, 0.0f},
    type(CT_LIGHT_TYPE_POINT),
    dirW{0.0f, -1.0f, 0.0f},
    openingAngle(Math::PI),
    intensity{1.0f, 1.0f, 1.0f},
    cosOpeningAngle(-1.0f),
    penumbraAngle(0.0f)
{
}

CameraData::CameraData() :
    focalLength(21.0f),
    up{0.0f, 1.0f, 0.0f},
    aspectRatio(1.7777f), // 16:9
    target{0.0f, 0.0f, -1.0f},
    nearZ(0.1f),
    cameraU{0.0f, 0.0f, 1.0f},
    farZ(1000.0f),
    cameraV{0.0f, 1.0f, 0.0f},
    jitterX(0.0f),
    cameraW{1.0f, 0.0f, 0.0f},
    jitterY(0.0f),
    frameHeight(24.0f),
    focalDistance(10000.0f),
    apertureRadius(0.0f),
    shutterSpeed(0.004f)
{
}

