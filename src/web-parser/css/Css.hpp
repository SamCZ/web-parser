#pragma once

#include <optional>
#include <cstdint>
#include <assert.h>

namespace Css
{
	enum class ESide : uint8_t
	{
		Bottom = 0,
		Left,
		Right,
		Top
	};

	enum class EPosition : uint8_t
	{
		Unset = 0,
		Absolute,
		Fixed,
		Inherit,
		Initial,
		Relative,
		Revert,
		Static,
		Sticky
	};

	enum class EDisplay : uint8_t
	{
		Unset = 0,
		Block,
		Flex,
		Grid,
		Inherit,
		Inline,
		InlineBlock,
		None
	};

	enum class ESizeMode : uint8_t
	{
		Unset = 0,
		Auto,
		FitContent,
		Inherit,
		Initial,
		MaxContent,
		MinContent,
		Revert
	};

	enum class ESizeUnit : uint8_t
	{
		Pixels = 0,
		Percent,
		RelativeToFont
	};

	struct FBorder
	{
		enum class EStyle : uint8_t
		{
			Solid = 0,
			None
		};

		EStyle Style;
		float Width[4];
		float Radius[4];
		float Color[4];

		inline FBorder() : Style(EStyle::None), Width{0, 0, 0, 0}, Radius{0, 0, 0, 0}, Color{1, 1, 1, 1}
		{

		}

		inline void SetColor(float r, float g, float b, float a) noexcept
		{
			Color[0] = r;
			Color[1] = g;
			Color[2] = b;
			Color[3] = a;
		}

		inline void SetWidth(uint8_t side, float width) noexcept
		{
			assert(side <= 4);
			Width[side] = width;
		}

		inline void SetRadius(uint8_t side, float radius) noexcept
		{
			assert(side <= 4);
			Radius[side] = radius;
		}
	};

	struct FBackground
	{

	};

	struct FBackgroundImage
	{

	};

	template<typename T1>
	struct CssValue
	{
		bool IsNumericValue;
		union
		{
			T1 Value;
			struct
			{
				int NumValue;
				ESizeUnit Unit;
			};
		};

		[[maybe_unused]] explicit CssValue(T1 value) : IsNumericValue(false), Value(value)
		{

		}

		[[maybe_unused]] CssValue(int numValue, ESizeUnit unit) : IsNumericValue(true), NumValue(numValue), Unit(unit)
		{

		}
	};

	typedef CssValue<ESizeMode> FUnitValue;

	struct FFourSize
	{
		std::optional<FUnitValue> Bottom;
		std::optional<FUnitValue> Left;
		std::optional<FUnitValue> Right;
		std::optional<FUnitValue> Top;
	};

	struct Style
	{
		std::optional<FUnitValue> Width;
		std::optional<FUnitValue> Height;

		std::optional<FUnitValue> Bottom;
		std::optional<FUnitValue> Left;
		std::optional<FUnitValue> Right;
		std::optional<FUnitValue> Top;

		std::optional<EPosition> Position;
		std::optional<EDisplay> Display;

		std::optional<FBorder> Border;
		std::optional<FBackground> Background;
		std::optional<FBackgroundImage> BackgroundImage;

		std::optional<FFourSize> Margin;
		std::optional<FFourSize> Padding;
	};
}