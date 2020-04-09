#pragma once

#include "Assets/AssetPtr.h"

class AssetImportSettings
{
public:
    virtual ~AssetImportSettings() {};
};

class AssetImporter
{
public:
   virtual ~AssetImporter() {};

   virtual bool IsExtensionSupported(const String &ext) const = 0;
   virtual APtr<AssetObject> Import(const String &path, const SPtr<AssetImportSettings> &settings) = 0;
};