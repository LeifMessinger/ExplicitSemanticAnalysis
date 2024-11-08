#include "WideStringStuff.h"
#include <stdexcept>
#include <string.h>
//#include <cwctype>
//#include <cstring>
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "XMLExtraction.h"
#include <string>
#include <cstdlib>
#include <cwchar>
#include <clocale>

std::wstring string_to_wide_string(const std::string& string)
{
    if (string.empty())
    {
        return L"";
    }

#ifndef _WIN32
    std::setlocale(LC_ALL, "en_US.UTF-8"); // Set the locale to UTF-8

    std::mbstate_t state = std::mbstate_t();
    const char* src = string.data();
    size_t len = string.size();
    std::wstring result(len, L'\0');
    wchar_t* dest = result.data();

    size_t converted = 0;
    while (len > 0)
    {
        int ret = std::mbtowc(dest, src, len);
        if (ret < 0)
        {
            throw std::runtime_error("mbtowc() failed");
        }
        else if (ret == 0)
        {
            break; // Null character encountered
        }

        src += ret;
        len -= ret;
        dest++;
        converted++;
    }

    result.resize(converted);
    return result;
#else
    const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, string.data(), (int)string.size(), nullptr, 0);
    if (size_needed <= 0)
    {
        throw std::runtime_error("MultiByteToWideChar() failed: " + std::to_string(size_needed));
    }

    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, string.data(), (int)string.size(), const_cast<wchar_t*>(result.data()), size_needed);
    return result;
#endif
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
