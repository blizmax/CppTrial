#pragma once

#include "Assets/AssetPtr.h"

class ImportSettings
{
public:
    virtual ~ImportSettings() {};
};

class AssetImporter
{
public:
   virtual ~AssetImporter() {};

   virtual bool IsExtensionSupported(const String &ext) const = 0;
   virtual APtr<AssetObject> Import(const String &path, const SPtr<ImportSettings> &settings) = 0;
};