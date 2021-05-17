#pragma once

#include <string>

#include "../util/String.hpp"

namespace Css
{
	class ClassPathNode
	{

	};

	class ClassPath
	{
	public:
		explicit ClassPath(const std::string& path)
		{
			auto list = StringUtils::SplitString(path, ' ');

			// TODO: build css path
		}
	};
}