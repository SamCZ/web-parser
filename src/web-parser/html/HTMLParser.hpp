#pragma once

#include <optional>
#include <utility>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>

#include "../util/String.hpp"
#include "../css/CssHierarchy.hpp"

namespace HtmlParser
{
	enum class XmlStatus : uint8_t
	{
		Loaded,
		FileNotFound,
		Unknown
	};

	class XmlDocument;

	class XmlNode
	{
		friend class XmlDocument;
	private:
		XmlNode* m_Parent{};
		std::string m_TagName;
		std::string m_Content;
		std::vector<XmlNode> m_Childrens;
		std::map<std::string, std::string> m_Attributes;
		std::string m_ElementId;
		std::vector<std::string> m_ElementClasses;
	public:
		inline XmlNode() = default;
		inline explicit XmlNode(std::string tagName) : m_TagName(std::move(tagName)) {}
	private:
		inline XmlNode& AddChild(XmlNode& node)
		{
			std::vector<std::string> attributePackList = SmartSplitAttributes(node.m_TagName);
			for (auto & attrPack : attributePackList) {
				auto del = attrPack.find('=');

				if(del == std::string::npos) {
					continue;
				}

				std::string attrName = attrPack.substr(0, del);
				std::string attrValue = attrPack.substr(del + 1);

				if(attrValue.length() == 0) {
					continue;
				}

				node.m_Attributes[attrName] = attrValue;

				// Process attributes

				if(attrName == "id") {
					node.m_ElementId = attrValue;
				}

				if(attrName == "class") {
					node.m_ElementClasses = SplitString(attrValue, ' ');
				}
			}

			node.FixContentEnding();

			node.m_Parent = this;
			m_Childrens.push_back(node);
			node.m_Attributes.clear();
			return m_Childrens.back();
		}

		inline void FixContentEnding()
		{
			if(!m_Content.empty()) {
				int i;
				for (i = static_cast<int>(m_Content.length()) - 1; i >= 0; --i) {
					if(!std::isspace(m_Content[i])) {
						break;
					}
				}
				m_Content.resize(i + 1);
			}
		}
	public:
		typedef std::vector<XmlNode>::iterator iterator;
		typedef std::vector<XmlNode>::const_iterator const_iterator;

		typedef std::vector<XmlNode>::reverse_iterator      reverse_iterator;
		typedef std::vector<XmlNode>::const_reverse_iterator const_reverse_iterator;

        [[nodiscard]] iterator begin() { return m_Childrens.begin(); }
        [[nodiscard]] iterator end() { return m_Childrens.end(); }

		[[nodiscard]] const_iterator begin() const { return m_Childrens.cbegin(); }
		[[nodiscard]] const_iterator end() const { return m_Childrens.cend(); }

        [[nodiscard]] reverse_iterator rbegin() { return m_Childrens.rbegin(); }
        [[nodiscard]] reverse_iterator rend() { return m_Childrens.rend(); }

		[[nodiscard]] const_reverse_iterator rbegin() const { return m_Childrens.crbegin(); }
        [[nodiscard]] const_reverse_iterator rend() const { return m_Childrens.crend(); }
	public:
		[[nodiscard]] inline const std::string& GetTagName() const noexcept { return m_TagName; }
		[[nodiscard]] inline const std::string& GetContent() const noexcept { return m_Content; }

		[[nodiscard]] inline const std::vector<XmlNode>& GetChilds() const noexcept { return m_Childrens; }
		[[nodiscard]] inline bool HasAnyChild() const noexcept { return !m_Childrens.empty(); }
		[[nodiscard]] inline int GetChildCount() const noexcept { return m_Childrens.size(); }
		[[nodiscard]] inline std::optional<XmlNode> GetChild(uint32_t index) const noexcept
        {
		    if(index < m_Childrens.size()) {
		        return m_Childrens[index];
		    }

		    return std::optional<XmlNode>();
        }

		[[nodiscard]] inline bool IsTextNode() const noexcept { return m_TagName.empty(); }
		[[nodiscard]] inline bool HasContent() const noexcept { return !m_Content.empty(); }

		[[nodiscard]] inline const std::map<std::string, std::string>& GetAttributes() const noexcept { return m_Attributes; }
		[[nodiscard]] inline bool HasAttribute(const std::string& name) const noexcept { return m_Attributes.find(name) != m_Attributes.end(); }
		[[nodiscard]] inline bool HasAttributes() const noexcept { return !m_Attributes.empty(); }

		[[nodiscard]] inline bool GetAttributeValue(const std::string& name, std::string& outValue) const noexcept
		{
			auto it = m_Attributes.find(name);

			if(it != m_Attributes.end()) {
				outValue = it->second;
				return true;
			}

			return false;
		}

		[[nodiscard]] inline std::string GetAttributeValue(const std::string& name) const noexcept
		{
			auto it = m_Attributes.find(name);

			if(it != m_Attributes.end()) {
				return it->second;
			}

			return std::string();
		}

		[[nodiscard]] inline int GetAttributeCount() const noexcept { return m_Attributes.size(); }
	public:
		[[nodiscard]] inline std::string GetElementId() { return m_ElementId; }
		[[nodiscard]] inline const std::vector<std::string>& GetElementClasses() { return m_ElementClasses; }
		[[nodiscard]] inline bool HasClass(const std::string& className) { return std::find(m_ElementClasses.begin(), m_ElementClasses.end(), className) != m_ElementClasses.end(); }
	public:
		[[nodiscard]] inline std::optional<XmlNode> FindElementById(const std::string& id)
		{
			return FindElementById(*this, id);
		}

		[[nodiscard]] inline std::vector<XmlNode> FindElementsByClass(const std::string& className)
		{
        	if(className.length() == 0) {
        		return {};
        	}

			std::vector<XmlNode> foundNodes;
        	FindElementsByClass(*this, foundNodes, className);
        	return foundNodes;
		}
	private:
		[[nodiscard]] static inline std::optional<XmlNode> FindElementById(XmlNode& node, const std::string& id)
		{
			auto it = node.m_Attributes.find("id");

			if(it != node.m_Attributes.end()) {
				return node;
			}

			for (XmlNode& childNode : node) {
				std::optional<XmlNode> result = FindElementById(childNode, id);

				if(result.has_value()) {
					return result;
				}
			}

			return std::optional<XmlNode>();
		}

		static inline void FindElementsByClass(XmlNode& node, std::vector<XmlNode>& foundNodes, const std::string& className)
		{
			if (node.HasClass(className)) {
				foundNodes.push_back(node);
			}

			for (XmlNode& childNode : node) {
				FindElementsByClass(childNode, foundNodes, className);
			}
		}
	};

	class XmlDocument : public XmlNode
	{
	public:
		XmlDocument() : XmlNode("RootNode") {}

		void LoadStylesheet(const std::filesystem::path& path);
		void LoadStylesheet(std::istream& stream);
		void LoadStylesheet(std::string& css);
		void LoadStylesheet(const Css::Hierarchy& cssHierarchy);

		void ClearAllStylesheets();
	public:
		static XmlStatus Parse(XmlDocument& document, const std::filesystem::path& path);
		static XmlStatus Parse(XmlDocument& document, std::istream& stream);
	private:
		static void ReadTag(XmlNode& currentNode, const std::string& xmlFileData, uint32_t offset);
	};
}