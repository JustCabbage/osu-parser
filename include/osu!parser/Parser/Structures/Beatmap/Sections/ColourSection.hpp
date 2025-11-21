#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include "Section.hpp"
#include "osu!parser/Parser/Utilities.hpp"

namespace OsuParser::Beatmap::Sections::Colour
{
	struct Colour
	{
		int32_t r;
		int32_t g;
		int32_t b;

		Colour(const std::string& value)
		{
			const auto vtr = Utilities::Split(Utilities::Trim(value), ',');
			this->r = std::stoi(vtr[0]);
			this->g = std::stoi(vtr[1]);
			this->b = std::stoi(vtr[2]);
		}
	};

	class ColourSection final : public Section
	{
		std::optional<Colour> GetColour(const std::string& name)
		{
			const auto value = this->GetAttribute(name);
			if (value.empty()) return std::nullopt;
			return Colour(value);
		}

	public:
		ColourSection() = default;
		void Parse(std::vector<std::string>& Lines) override
		{
			this->LoadAttributes(Lines);

			this->Combo1 = GetColour("Combo1");
			this->Combo2 = GetColour("Combo2");
			this->Combo3 = GetColour("Combo3");
			this->Combo4 = GetColour("Combo4");
			this->Combo5 = GetColour("Combo5");
			this->Combo6 = GetColour("Combo6");
			this->Combo7 = GetColour("Combo7");
			this->Combo8 = GetColour("Combo8");
			this->SliderTrackOverride = GetColour("SliderTrackOverride");
			this->SliderBorder = GetColour("SliderBorder");
		}

		std::optional<Colour> Combo1 = std::nullopt;
		std::optional<Colour> Combo2 = std::nullopt;
		std::optional<Colour> Combo3 = std::nullopt;
		std::optional<Colour> Combo4 = std::nullopt;
		std::optional<Colour> Combo5 = std::nullopt;
		std::optional<Colour> Combo6 = std::nullopt;
		std::optional<Colour> Combo7 = std::nullopt;
		std::optional<Colour> Combo8 = std::nullopt;
		std::optional<Colour> SliderTrackOverride = std::nullopt;
		std::optional<Colour> SliderBorder = std::nullopt;
	};
}
