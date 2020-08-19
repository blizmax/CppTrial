#pragma once

#include "Assets/AssetPtr.h"

class ExportSettings
{
public:
    virtual ~ExportSettings() = default;

    template <typename T>
    static SPtr<T> As(const SPtr<ExportSettings> &settings)
    {
        auto result = std::dynamic_pointer_cast<T>(settings);
        if (!result)
            result = T::Create();
        return result;
    }
};


class IAssetExporter
{
public:
    virtual ~IAssetExporter() = default;
};

template <typename T>
class AssetExporter : public IAssetExporter
{
public:
    virtual ~AssetExporter() = default;
    virtual void Export(const SPtr<T> &asset, const String &path, const SPtr<ExportSettings> &settings) = 0;
};