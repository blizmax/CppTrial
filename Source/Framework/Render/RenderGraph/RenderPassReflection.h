#pragma once

#include "Render/RenderGraph/.Package.h"

namespace RenderPassReflectionInternal
{
enum class FieldType
{
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    RawBuffer,
};

enum class FieldVisibility
{
    Undefined,
    Internal,
    Input,
    Output,
    InputOutput,
};

class Field
{
public:
    Field() = default;

    Field(const String &name, const String &desc, FieldVisibility visibility)
        : name(name), desc(desc), visibility(visibility)
    {
    }

    bool IsValid() const;
    Field &ResourceType(FieldType type, int32 width, int32 height, int32 depth, int32 sampleCount, int32 mipLevels, int32 arrayLayers);

    bool operator==(const Field &other) const = default;
    bool operator!=(const Field &other) const = default;

    Field &RawBuffer(int32 size)
    {
        return ResourceType(FieldType::RawBuffer, size, 0, 0, 0, 0, 0);
    }

    Field &Texture1D(int32 width, int32 mipLevels = 1, int32 arrayLayers = 1)
    {
        return ResourceType(FieldType::Texture1D, width, 0, 0, 0, mipLevels, arrayLayers);
    }

    Field &Texture2D(int32 width, int32 height, int32 sampleCount = 1, int32 mipLevels = 1, int32 arrayLayers = 1)
    {
        return ResourceType(FieldType::Texture2D, width, height, 0, sampleCount, mipLevels, arrayLayers);
    }

    Field &Texture3D(int32 width, int32 height, int32 depth, int32 arrayLayers = 1)
    {
        return ResourceType(FieldType::Texture3D, width, height, depth, 1, 1, arrayLayers);
    }

    Field &TextureCube(int32 width, int32 height, int32 mipLevels = 1, int32 arrayLayers = 1)
    {
        return ResourceType(FieldType::TextureCube, width, height, 1, 1, mipLevels, arrayLayers);
    }

    Field &SetName(const String &inName)
    {
        name = inName;
        return *this;
    }

    Field &SetDesc(const String &inDesc)
    {
        desc = inDesc;
        return *this;
    }

    Field &SetResourceFormat(ResourceFormat inFormat)
    {
        format = inFormat;
        return *this;
    }

    Field &SetResourceBindFlags(ResourceBindFlags flags)
    {
        bindFlags = flags;
        return *this;
    }

    Field &SetVisibility(FieldVisibility inVisibility)
    {
        visibility = inVisibility;
        return *this;
    }

    const String &GetName() const
    {
        return name;
    }

    const String &GetDesc() const
    {
        return desc;
    }

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

    int32 GetDepth() const
    {
        return depth;
    }

    int32 GetSampleCount() const
    {
        return sampleCount;
    }

    int32 GetArrayLayers() const
    {
        return arrayLayers;
    }

    int32 GetMipLevels() const
    {
        return mipLevels;
    }

    ResourceFormat GetResourceFormat() const
    {
        return format;
    }

    ResourceBindFlags GetResourceBindFlags() const
    {
        return bindFlags;
    }

    FieldType GetFieldType() const
    {
        return fieldType;
    }

    FieldVisibility GetVisibility() const
    {
        return visibility;
    }

private:
    FieldType fieldType = FieldType::Texture2D;
    int32 width = 0;
    int32 height = 0;
    int32 depth = 0;
    int32 sampleCount = 1;
    int32 mipLevels = 1;
    int32 arrayLayers = 1;
    ResourceFormat format = ResourceFormat::Unknown;
    ResourceBindFlags bindFlags = ResourceBind::None;
    FieldVisibility visibility = FieldVisibility::Undefined;
    String name;
    String desc;
};

}

class RenderPassReflection
{
public:
    using FieldType = RenderPassReflectionInternal::FieldType;
    using FieldVisibility = RenderPassReflectionInternal::FieldVisibility;
    using Field = RenderPassReflectionInternal::Field;

    Field &AddInput(const String &name, const String &desc);
    Field &AddOutput(const String &name, const String &desc);
    Field &AddInputOutput(const String &name, const String &desc);
    Field &AddInternal(const String &name, const String &desc);
    Field &AddField(Field field);
    int32 GetFieldIndex(const String &name) const;

    bool operator==(const RenderPassReflection &other) const;
    bool operator!=(const RenderPassReflection &other) const
    {
        return !(*this == other);
    }

    bool HasField(const String &name) const
    {
        return GetFieldIndex(name) != -1;
    }

    const Field &GetField(const String &name) const
    {
        return fields[GetFieldIndex(name)];
    }

    bool HasField(int32 index) const
    {
        return index >= 0 && index <= fields.Count();
    }

    const Field &GetField(int32 index) const
    {
        return fields[index];
    }

    int32 GetFieldCount() const
    {
        return fields.Count();
    }

private:
    Field &AddField(const String &name, const String &desc, FieldVisibility visibility);

private:
    Array<Field> fields;
};