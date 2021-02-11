#include "HTMLParser.hpp"
#include "../css/CssParser.hpp"
#include "../css/CssClassPath.hpp"

namespace HtmlParser
{
	static std::vector<std::string> _AutoClosingTags = {
			"!DOCTYPE",
			"meta",
			"link",
			"img",
			"br"
	};

	static bool IsAutoClosingTag(std::string tag)
	{
		auto i = tag.find(' ');

		if(i != std::string::npos) {
			tag = tag.substr(0, i);
		}

		return std::find(_AutoClosingTags.begin(), _AutoClosingTags.end(), tag) != _AutoClosingTags.end();
	}

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
						currentNode.FixContentEnding();
						ReadTag(*currentNode.m_Parent, xmlFileData, i + 1);
						return;
					} else if(IsAutoClosingTag(childNode.m_TagName) || xmlFileData[i - 1] == '/') {
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

	XmlStatus XmlDocument::Parse(XmlDocument &document, const std::filesystem::path& path)
	{
		std::fstream stream;
		stream.open(path);

		if(!stream.is_open()) {
			return XmlStatus::FileNotFound;
		}

		return Parse(document, stream);
	}

	XmlStatus XmlDocument::Parse(XmlDocument &document, std::istream& stream)
	{
		std::string xmlFileData(std::istreambuf_iterator<char>(stream), {});

		ReadTag(document, xmlFileData, 0);

		return XmlStatus::Loaded;
	}

	void XmlDocument::LoadStylesheet(const std::filesystem::path &path)
	{
		auto css = Css::Hierarchy::Parse(path);

		if(css.has_value()) {
			LoadStylesheet(css.value());
		} else {
			throw std::runtime_error("Cannot load " + path.string());
		}
	}

	void XmlDocument::LoadStylesheet(std::istream &stream)
	{
		auto css = Css::Hierarchy::Parse(stream);

		if(css.has_value()) {
			LoadStylesheet(css.value());
		} else {
			throw std::runtime_error("Cannot load css from stream !");
		}
	}

	void XmlDocument::LoadStylesheet(std::string &css)
	{
		auto cssh = Css::Hierarchy::Parse(css);

		if(cssh.has_value()) {
			LoadStylesheet(cssh.value());
		} else {
			throw std::runtime_error("Cannot parse css from string !");
		}
	}

	void XmlDocument::LoadStylesheet(const Css::Hierarchy &cssHierarchy)
	{
		// TODO: apply styles
	}

	void XmlDocument::ClearAllStylesheets()
	{
		// TODO: Clear all styles
	}
}