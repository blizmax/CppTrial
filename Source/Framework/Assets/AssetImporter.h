#pragma once

#include "Assets/AssetPtr.h"

class ImportSettings
{
public:
    virtual ~ImportSettings() = default;
};

template <typename T>
class AssetImporter
{
public:
    virtual ~AssetImporter() = default;
    virtual APtr<T> Import(const String &path, const SPtr<ImportSettings> &settings) = 0;
};