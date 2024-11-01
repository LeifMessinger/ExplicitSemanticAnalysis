#pragma once
#include <string>

std::wstring string_to_wide_string(const std::string& string);

void wideStringToLowerInPlace(std::wstring& wstring);

//std::string wide_string_to_string(const std::wstring& wide_string);