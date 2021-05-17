#include "CssParser.hpp"
#include "../util/String.hpp"

namespace Css::CssParser
{
	static ESizeMode ParseSizeMode(const std::string& mode)
	{
		if(mode == "auto") {
			return ESizeMode::Auto;
		}

		if(mode == "fit-content") {
			return ESizeMode::FitContent;
		}

		if(mode == "inherit") {
			return ESizeMode::Inherit;
		}

		if(mode == "initial") {
			return ESizeMode::Initial;
		}

		if(mode == "max-content") {
			return ESizeMode::MaxContent;
		}

		if(mode == "min-content") {
			return ESizeMode::MinContent;
		}

		if(mode == "revert") {
			return ESizeMode::Revert;
		}

		return ESizeMode::Unset;
	}

	static std::optional<FUnitValue> ParseUnitValue(const std::string& value)
	{
		int number = 0;

		if(value.empty()) {
			return {};
		}

		if('0' <= value[0] && value[0] <= '9') {
			int i;
			for (i = 0; i < value.length(); ++i) {
				const char& c = value[i];

				if('0' <= c && c <= '9') {
					number = number * 10 + (c - '0');
				} else {
					break;
				}
			}

			if(i == value.length()) {
				if(number == 0) {
					return FUnitValue(0, ESizeUnit::Pixels);
				} else {
					throw std::runtime_error("Unknown unit value !");
				}
			}

			if(value[i] == '%') {
				return FUnitValue(number, ESizeUnit::Percent);
			}

			std::string_view view(&value[i], value.length() - i + 1);

			if(view.compare("px")) {
				return FUnitValue(number, ESizeUnit::Pixels);
			} else if(view.compare("em")) {
				return FUnitValue(number, ESizeUnit::RelativeToFont);
			} else {
				throw std::runtime_error("Unknown unit value !");
			}
		} else {
			return FUnitValue(ParseSizeMode(value));
		}
	}

	static void ParseFourSize(std::optional<FFourSize>& fourSize, std::string& key, std::string& value)
	{
		if(!fourSize.has_value()) {
			FFourSize data = {};
			fourSize = data;
		}

		std::vector<std::string> split = StringUtils::SplitString(value, ' ');

		switch (split.size()) {
			case 1: {
				std::optional<FUnitValue> unitValue = ParseUnitValue(value);

				fourSize->Bottom = unitValue;
				fourSize->Left = unitValue;
				fourSize->Right = unitValue;
				fourSize->Top = unitValue;
				break;
			}
			case 2: {
				std::optional<FUnitValue> unitValue0 = ParseUnitValue(split[0]);
				std::optional<FUnitValue> unitValue1 = ParseUnitValue(split[1]);

				fourSize->Top = unitValue0;
				fourSize->Bottom = unitValue0;

				fourSize->Right = unitValue1;
				fourSize->Left = unitValue1;
				break;
			}
			case 4: {
				std::optional<FUnitValue> unitValue0 = ParseUnitValue(split[0]);
				std::optional<FUnitValue> unitValue1 = ParseUnitValue(split[1]);
				std::optional<FUnitValue> unitValue2 = ParseUnitValue(split[2]);
				std::optional<FUnitValue> unitValue3 = ParseUnitValue(split[3]);

				fourSize->Top = unitValue0;
				fourSize->Right = unitValue1;
				fourSize->Bottom = unitValue2;
				fourSize->Left = unitValue3;
				break;
			}

			default:
				throw std::runtime_error("Unsupported value for parsing !");
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	static void ParserColor(Node& node, std::string& key, std::string& value)
	{
		// TODO: Parse color
	}

	static void ParserMargin(Node& node, std::string& key, std::string& value)
	{
		ParseFourSize(node.Style.Margin, key, value);
	}

	static void ParserPadding(Node& node, std::string& key, std::string& value)
	{
		ParseFourSize(node.Style.Padding, key, value);
	}

	static void ParserBackground(Node& node, std::string& key, std::string& value)
	{
		// TODO: Parse background
	}

	static void ParserBorder(Node& node, std::string& key, std::string& value)
	{
		// TODO: Parse border
	}

	static void ParserDisplay(Node& node, std::string& key, std::string& value)
	{
		// TODO: Parse display
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	std::map<std::string, std::function<void(Node&, std::string&, std::string&)>> TokenParsers;

	void InitTokenParsers()
	{
		TokenParsers = {
				{"color", &ParserColor},
				{"margin", &ParserMargin},
				{"padding", &ParserPadding},
				{"background", &ParserBackground},
				{"border", &ParserBorder},
				{"display", &ParserDisplay}
		};
	}

}