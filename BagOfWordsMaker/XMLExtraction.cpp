#include <iostream>
#include "tinyxml2.h"
#include "WideStringStuff.h"
#include "XMLExtraction.h"

#include <sstream>

using namespace tinyxml2;

std::shared_ptr<XMLDocument> openDoc(const std::string& filename) {
    std::shared_ptr<XMLDocument> doc(new XMLDocument);
    if (doc->LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Failed to load file: " << filename << std::endl;
        return doc;
    }
    return doc;
}

void extractTextFromElement(XMLElement* element, std::wostringstream& oss) {
    if (!element) return;

    const char* tag = element->Name();
    if (tag && (strcmp(tag, "style") == 0 || strcmp(tag, "script") == 0)) {
        return; // Skip style and script elements
    }

    if (element->GetText()) {
        std::wstring wide_text = string_to_wide_string(element->GetText());
        oss << wide_text << L" ";
    }

    for (XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        extractTextFromElement(child, oss);
    }
}

std::wstring extractTitleFromElement(XMLElement* element) {
    if (!element) return L"";

    const char* tag = element->Name();
    if (tag && strcmp(tag, "title") == 0 && element->GetText()) {
        return string_to_wide_string(element->GetText());
    }

    for (XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        std::wstring title = extractTitleFromElement(child);
        if (!title.empty()) {
            return title;
        }
    }

    return L"";
}

std::wstring extractTitleFromXML(std::shared_ptr<XMLDocument>& doc) {
    XMLElement* root = doc->RootElement();
    if (root) {
        return extractTitleFromElement(root);
    }
    return L"";
}

std::wstring extractTextFromXML(std::shared_ptr<XMLDocument>& doc) {
    std::wostringstream oss;
    XMLElement* root = doc->RootElement();
    extractTextFromElement(root, oss);

    return oss.str();
}
