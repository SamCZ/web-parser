#include <web-parser/html/HTMLLoader.hpp>
#include <iostream>

using namespace HtmlParser;

int main()
{
	XmlDocument doc;
	if(XmlDocument::Load(doc, "page.html") == XmlStatus::Loaded) {
		std::function<void(const XmlNode&, int)> fnc;
		fnc = [&fnc](const XmlNode& node, int depth){
			std::string spaces;
			for (int i = 0; i < depth; ++i) {
				spaces += "-";
			}

			if(!node.HasContent()) {
				std::cout << (spaces + node.GetTagName() + " (" + std::to_string(node.GetAttributeCount()) + ")") << std::endl;
			} else {
				std::cout << spaces + node.GetTagName()+ "(" + std::to_string(node.GetAttributeCount()) + "): " + node.GetContent() << std::endl;
			}

			for (const XmlNode& child : node) {
				fnc(child, depth + 1);
			}
		};

		fnc(doc, 0);

		std::cout << doc.FindElementById("panel")->GetChild(0)->GetContent() << std::endl;

		for(const XmlNode& el : doc.FindElementsByClass("one")) {
			std::cout << "class: " << el.GetTagName() << el.GetChildCount() << el.GetContent() << std::endl;
		}
	}
}