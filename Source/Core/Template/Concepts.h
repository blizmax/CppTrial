#pragma once

#include "Core/Template/.Package.h"

template <typename T>
concept OneKindOfChars = std::same_as<T, char8> || std::same_as<T, char16> || std::same_as<T, char32> || std::same_as<T, wchar>;
