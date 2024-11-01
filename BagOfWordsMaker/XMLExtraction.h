#pragma once

#include <iostream>
#include <memory>
#include "tinyxml2.h"

std::shared_ptr<tinyxml2::XMLDocument> openDoc(const std::string& filename);
void extractTextFromElement(tinyxml2::XMLElement* element, std::wostringstream& oss);
std::wstring extractTitleFromXML(std::shared_ptr<tinyxml2::XMLDocument>& doc);
std::wstring extractTextFromXML(std::shared_ptr<tinyxml2::XMLDocument>& doc);
