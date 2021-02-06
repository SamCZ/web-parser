#include "HTMLLoader.hpp"

namespace HtmlParser
{
	void XmlDocument::ReadTag(XmlNode& currentNode, const std::string& xmlFileData, uint32_t offset)
	{
		XmlNode childNode;

		bool readingTag = false;
		bool isEndTag = false;

		for (int i = offset; i < xmlFileData.length(); ++i) {
			char currentChar = xmlFileData[i];

			switch (currentChar) {
				case '<':
					readingTag = true;
					isEndTag = xmlFileData[i + 1] == '/';

					if(childNode.m_Content.length() > 0 && !currentNode.m_Childrens.empty()) {
						XmlNode textNode;
						textNode.m_Content = childNode.m_Content;
						currentNode.AddChild(textNode);
					}

					break;
				case '>':
					readingTag = false;
					if(isEndTag) {
						currentNode.m_Content = childNode.m_Content;
						ReadTag(*currentNode.m_Parent, xmlFileData, i + 1);
						return;
					} else if(xmlFileData[i - 1] == '/') {
						childNode.m_TagName.resize(childNode.m_TagName.length() - 1);
						childNode.m_Content = "";
						currentNode.AddChild(childNode);
						childNode.m_TagName = "";
						childNode.m_Childrens.clear();
					} else {
						ReadTag(currentNode.AddChild(childNode), xmlFileData, i + 1);
						return;
					}

					childNode.m_Content = "";
					childNode.m_TagName = "";
					break;
				default: {
					if(!readingTag) {
						if(std::isspace(currentChar) && std::isspace(childNode.m_Content.back())) {

						} else if(childNode.m_Content.empty() && std::isspace(currentChar)) {

						} else {
							childNode.m_Content += currentChar;
						}
					} else {
						if(std::isspace(currentChar) && std::isspace(childNode.m_TagName.back())) {

						} else {
							childNode.m_TagName += currentChar;
						}
					}

					break;
				}
			}
		}
	}
	XmlStatus XmlDocument::Load(XmlDocument &document, const std::filesystem::path& path)
	{
		std::fstream stream;
		stream.open(path);

		if(!stream.is_open()) {
			return XmlStatus::FileNotFound;
		}

		return Load(document, stream);
	}

	XmlStatus XmlDocument::Load(XmlDocument &document, std::istream& stream)
	{
		std::string xmlFileData(std::istreambuf_iterator<char>(stream), {});

		ReadTag(document, xmlFileData, 0);

		return XmlStatus::Loaded;
	}
}