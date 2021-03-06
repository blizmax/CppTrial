#pragma once

#include "Assets/AssetPtr.h"

class ImportSettings
{
public:
    virtual ~ImportSettings() = default;

    template <typename T>
    static SPtr<T> As(const SPtr<ImportSettings> &settings)
    {
        auto result = std::dynamic_pointer_cast<T>(settings);
        if (!result)
            result = T::Create();
        return result;
    }
};

class IAssetImporter
{
public:
    virtual ~IAssetImporter() = default;
};

template <typename T>
class AssetImporter : public IAssetImporter
{
public:
    virtual ~AssetImporter() = default;
    virtual APtr<T> Import(const String &path, const SPtr<ImportSettings> &settings) = 0;
};