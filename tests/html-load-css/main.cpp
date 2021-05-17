#include <web-parser/html/HTMLParser.hpp>
#include <iostream>

using namespace HtmlParser;

int main()
{
	XmlDocument doc;
	if(XmlDocument::Parse(doc, "page.html") == XmlStatus::Loaded) {
		//doc.LoadStylesheet("style.css");

		// TODO: Test getting some style of some element

		if(auto block = doc.FindElementById("block1")) {
			std::cout << "Style of block: " << block->GetTagName() << std::endl;
		}
	}

	return 0;
}