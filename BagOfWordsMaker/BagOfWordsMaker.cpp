#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <ostream>
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00

#if _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include <memory>
#include "tinyxml2.h"
#include "WideStringStuff.h"
#include "XMLExtraction.h"

std::vector<std::wstring> tokenizeText(const std::wstring& text) {
    std::wregex word_regex(LR"(\b\w+\b)");
    auto words_begin = std::wsregex_iterator(text.begin(), text.end(), word_regex);
    auto words_end = std::wsregex_iterator();

    std::vector<std::wstring> vec;

    for (std::wsregex_iterator i = words_begin; i != words_end; ++i) {
        std::wstring token = i->str();
        wideStringToLowerInPlace(token);
        vec.push_back(token);
    }

    return vec;
}

std::map<std::wstring, int> generateHistogram(const std::vector<std::wstring>& words) {
    std::map<std::wstring, int> histogram;
    for (const auto& word : words) {
        ++histogram[word];
    }
    return histogram;
}

#ifndef MAX_PATH
#define MAX_PATH 500
#endif

/*void checkCWD() {
    wchar_t buffer[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, buffer) != 0) {
        std::wcerr << L"Current directory: " << buffer << std::endl;
    }
    else {
        std::wcerr << L"Error getting current directory." << std::endl;
    }
}*/

void htmlToBagOfWords(const std::shared_ptr<std::string> filename, std::wostream& out) {
    std::shared_ptr<tinyxml2::XMLDocument> doc = openDoc(*filename);
    if (doc->Error()) {
        return;
    }

    try {
        {
            std::wstring title = extractTitleFromXML(doc);
            out << title << std::endl;
        }

        std::wstring text = extractTextFromXML(doc);
        auto hist = generateHistogram(tokenizeText(text));

        {
            int tokenCount = 0;
            int totalCount = 0;
            for (const auto& pair : hist) {
                tokenCount++;
                totalCount += pair.second;
            }
            out << tokenCount << " " << totalCount << std::endl;  //No real reason to give the total count, because people can just calc that themselves
        }

        for (const auto& pair : hist) {
            out << pair.first << L" " << pair.second << std::endl;
        }
        out << std::endl;
    }
    catch (...) {
        //oopsie woopsies
    }
}

//called like:
//a.exe "./TexasStateLaw/TexasStateConstitution/CN.2.htm"
int main(int argc, char* argv[]) {

    //checkCWD();

    std::vector<std::wostringstream> outputBuffers(argc - 1);
    std::vector<std::thread> threads;
    threads.reserve(argc - 1);

    for (size_t i = 1; i < argc; ++i) {
        std::shared_ptr<std::string> filename = std::make_shared<std::string>(argv[i]);
        threads.emplace_back(htmlToBagOfWords, filename, std::ref(outputBuffers[i - 1]));
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        if (threads[i].joinable()) {
            threads[i].join();
        }   //else, guessing it finished before we could .join it.
        
        //We can only do this because the threads are pushed in order, and joined in order.
        std::wcout << outputBuffers[i].str();
    }

    return 0;
}
