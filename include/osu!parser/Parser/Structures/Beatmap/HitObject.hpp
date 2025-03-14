#pragma once
#include <osu!parser/Parser/Utilities.hpp>
#include <optional>
#include <variant>

namespace Parser
{
	enum class TypeBitmap : std::int32_t
	{
		HIT_CIRCLE = 1 << 0,
		SLIDER = 1 << 1,
		SPINNER = 1 << 3,
	};
	enum class HitsoundBitmap : std::uint8_t
	{
		// https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#sliders:~:text=mania%20hold%20note.-,Hitsounds,-The%20hitSound%20bit

		NORMAL = 1 << 0,
		// below is additional
		WHISTLE = 1 << 1,
		FINISH = 1 << 2,
		CLAP = 1 << 3
	};
	enum class SampleSetType : std::int32_t
	{
		NO_CUSTOM = 0,
		NORMAL = 1,
		SOFT = 2,
		DRUM = 3
	};

	struct Hitsound
	{
		bool whistle = false;
		bool finish = false;
		bool clap = false;

		void import(const std::int32_t hitsound)
		{
			whistle = (hitsound & std::int32_t(HitsoundBitmap::WHISTLE)) != 0;
			finish = (hitsound & std::int32_t(HitsoundBitmap::FINISH)) != 0;
			clap = (hitsound & std::int32_t(HitsoundBitmap::CLAP)) != 0;
		}
		std::int32_t to_int() const
		{
			std::int32_t hitsound = std::int32_t(HitsoundBitmap::NORMAL);
			if (whistle) hitsound |= std::int32_t(HitsoundBitmap::WHISTLE);
			if (finish) hitsound |= std::int32_t(HitsoundBitmap::FINISH);
			if (clap) hitsound |= std::int32_t(HitsoundBitmap::CLAP);
			return hitsound;
		}

		Hitsound() = default;
		Hitsound(const std::int32_t hitsound_int) { import(hitsound_int); }
	};

	struct HitObjectType
	{
		
	};

	struct HitObject
	{
		// https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#sliders:~:text=additional%20objectParams.-,Sliders,-Slider%20syntax%3A
		struct SliderParams
		{
			struct SliderCurve
			{
				enum class CurveType : char
				{
					BEZIER = 'B', // bézier
					CATMULL = 'C', // centripetal catmull-rom
					LINEAR = 'L',
					PERFECT = 'P'
				};

				CurveType curveType;
				std::vector<std::pair<std::int32_t, std::int32_t>> curvePoints = {};

				void import(const std::string& Curve_str)
				{
					std::vector<std::string> Curve = Utilities::Split(Curve_str, '|');

					// curveType
					curveType = CurveType(Curve.front().front());
					Curve.erase(Curve.begin());

					// curvePoints (now Curve is became curvePoints - aka curvePoint list)
					for (const std::string& CurvePoint : Curve)
					{
						const std::vector<std::string> SplitPoint = Utilities::Split(CurvePoint, ':');
						curvePoints.emplace_back(std::stoi(SplitPoint[0]), std::stoi(SplitPoint[1]));
					}
				}

				SliderCurve() = default;
				SliderCurve(const std::string& Curve_str) { import(Curve_str); }
			};
			struct EdgeHitsound
			{
				struct SampleSet
				{
					SampleSetType normalSet = SampleSetType::NO_CUSTOM;
					SampleSetType additionSet = SampleSetType::NO_CUSTOM;

					void import(const std::string& edgeSet_str)
					{
						if (edgeSet_str.empty()) return; //not written
						auto list = Utilities::Split(edgeSet_str, ':');
						normalSet = SampleSetType(std::stoi(list[0]));
						additionSet = SampleSetType(std::stoi(list[1]));
					}

					SampleSet() = default;
					SampleSet(const std::string& edgeSet_str) { import(edgeSet_str); }
				};

				Hitsound sound; // edgeSounds 
				SampleSet sample; // edgeSets
			};
			struct EdgeHitsounds: std::vector<EdgeHitsound>
			{
				void import(const std::string edgeSounds_str, const std::string edgeSets_str)
				{
					auto edgeSounds = Utilities::Split(edgeSounds_str, '|');
					auto edgeSets = Utilities::Split(edgeSets_str, '|');

					for (std::int32_t i=0; i < edgeSounds.size(); i++)
					{
						EdgeHitsound edgeHitsound;
						edgeHitsound.sound.import(std::stoi(edgeSounds[i]));
						edgeHitsound.sample.import(edgeSets[i]);

						this->push_back(edgeHitsound);
					}
				}

				EdgeHitsounds() = default;
				EdgeHitsounds(const std::string edgeSounds_str, const std::string edgeSets_str) { import(edgeSounds_str, edgeSets_str); }
			};

			std::int32_t slides = 1; // aka Repeats
			std::double_t length = 0; // aka PixelLength
			SliderCurve curve;
			EdgeHitsounds edgeHitsounds; // <edgeSounds + edgeSets> list
		};
		struct SpinnerParams
		{
			std::int32_t endTime = 0;
		};
		struct HitSample
		{
			// https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#hitsounds:~:text=enabled%20by%20default.-,Custom%20hit%20samples,-Usage%20of%20hitSample

			SampleSetType normalSet = SampleSetType::NO_CUSTOM;
			SampleSetType additionSet = SampleSetType::NO_CUSTOM;
			std::int32_t index = 0;
			std::int32_t volume = 0;
			std::string filename;

			HitSample() = default;
			HitSample(const std::string hitsample_str)
			{
				if (hitsample_str.empty()) return; //not written
				auto list = Utilities::Split(hitsample_str, ':');
				normalSet = SampleSetType(std::stoi(list[0]));
				additionSet = SampleSetType(std::stoi(list[1]));
				index = std::stoi(list[2]);
				volume = std::stoi(list[3]);
				if (list.size() > 4) { filename = list[4]; }
			}

			std::string getHitsoundTypeFilename(const HitsoundBitmap hitsoundType)
			{
				if (!filename.empty())
				{
					// Only provide Normal hitsound
					if (hitsoundType == HitsoundBitmap::NORMAL) return filename;
					return ""; 
				}

				//sampleSet
				SampleSetType sampleSet = (hitsoundType == HitsoundBitmap::NORMAL) ? normalSet : additionSet;
				std::string sampleSetStr;
				switch (sampleSet)
				{
				case SampleSetType::NORMAL: sampleSetStr = "normal"; break;
				case SampleSetType::SOFT: sampleSetStr = "soft"; break;
				case SampleSetType::DRUM: sampleSetStr = "drum"; break;
				default: return ""; // No custom sampleSet, use skin hitsound instand
				}

				// hitSound
				std::string hitsoundTypeStr;
				switch (hitsoundType)
				{
				case HitsoundBitmap::WHISTLE: hitsoundTypeStr = "whistle"; break;
				case HitsoundBitmap::FINISH: hitsoundTypeStr = "finish"; break;
				case HitsoundBitmap::CLAP: hitsoundTypeStr = "clap"; break;
				default: hitsoundTypeStr = "normal"; break;
				}

				std::string filename;
				filename.append(sampleSetStr);
				filename.append("-hit");
				filename.append(hitsoundTypeStr);
				if (index != 0 && index != 1) filename.append(std::to_string(index));
				filename.append(".wav");
				return filename;
			}
		};

		std::int32_t x = 0, y = 0;
		std::int32_t time = 0;
		TypeBitmap type = TypeBitmap::HIT_CIRCLE;
		Hitsound hitSound;
		std::optional<SliderParams> sliderParams = SliderParams();
		std::optional<SpinnerParams> spinnerParams = SpinnerParams();
		HitSample hitSample;
	};
}