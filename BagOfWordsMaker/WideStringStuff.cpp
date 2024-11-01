#include "WideStringStuff.h"
#include <stdexcept>
#include <string.h>
//#include <cwctype>
//#include <cstring>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "XMLExtraction.h"

std::wstring string_to_wide_string(const std::string& string)
{
    if (string.empty())
    {
        return L"";
    }

    const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, string.data(), (int)string.size(), nullptr, 0);
    if (size_needed <= 0)
    {
        throw std::runtime_error("MultiByteToWideChar() failed: " + std::to_string(size_needed));
    }

    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, string.data(), (int)string.size(), const_cast<wchar_t*>(result.data()), size_needed);
    return result;
}

void wideStringToLowerInPlace(std::wstring& str) {
    if (!str.empty()) {
        //Uses some super obscure assembly instruction to make a wide character string lower case.
	for (size_t i = 0; i < str.length(); ++i) {
		str[i] = std::towlower(str[i]);
	}

	//Microsoft Visual Studio only... for some reason
        //_wcslwr_s(&wstring[0], str.size() + 1);
    }
}

//Never used
/*std::string wide_string_to_string(const std::wstring& wide_string)
{
    if (wide_string.empty())
    {
        return "";
    }

    const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), (int)wide_string.size(), nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0)
    {
        throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
    }

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), (int)wide_string.size(), result.data(), size_needed, nullptr, nullptr);
    return result;
}*/
