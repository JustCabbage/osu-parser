#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include "Structures/Beatmap/TimingPoint.hpp"
#include "Structures/Beatmap/HitObject.hpp"
#include "Structures/Beatmap/Sections/GeneralSection.hpp"
#include "Structures/Beatmap/Sections/MetadataSection.hpp"
#include "Structures/Beatmap/Sections/DifficultySection.hpp"
#include "Structures/Beatmap/Sections/EditorSection.hpp"

namespace Parser
{
	static constexpr int MINIMUM_LINE_CHARACTERS = 3;

	class Beatmap
	{
	public:
		Beatmap(const std::string& BeatmapPath) : m_CurrentStream(BeatmapPath)
		{
			this->Reset();
			if (!m_CurrentStream.good())
			{
				return;
			}

			std::string CurrentLine;
			std::getline(m_CurrentStream, CurrentLine);
			while (std::getline(m_CurrentStream, CurrentLine))
			{
				CurrentLine = Utilities::Trim(CurrentLine);
				static std::string CurrentSection;
				if (!CurrentLine.empty() && CurrentLine.front() == '[' && CurrentLine.back() == ']')
				{
					CurrentSection = Utilities::Split(Utilities::Split(CurrentLine, '[')[1], ']')[0];
					continue;
				}
				// In >=C++11, std::string can save UTF-8 string (non-ascii char will be saved as 2 bytes, but in that one byte it looks weird)
				if (CurrentLine.size() < MINIMUM_LINE_CHARACTERS) continue;

				this->m_Sections[CurrentSection].push_back(CurrentLine);
			}

			this->General.Parse(this->m_Sections["General"]);
			this->Metadata.Parse(this->m_Sections["Metadata"]);
			this->Editor.Parse(this->m_Sections["Editor"]);
			this->Difficulty.Parse(this->m_Sections["Difficulty"]);

			for (const std::string& PointString : this->m_Sections["TimingPoints"])
			{
				TimingPoint Point;
				const std::vector<std::string> SplitPoint = Utilities::Split(PointString, ',');
				Point.Time = std::stoi(SplitPoint[0]);
				Point.BeatLength = std::stof(SplitPoint[1]);
				Point.Meter = std::stoi(SplitPoint[2]);
				Point.SampleSet = std::stoi(SplitPoint[3]);
				Point.SampleIndex = std::stoi(SplitPoint[4]);
				Point.Volume = std::stoi(SplitPoint[5]);
				Point.Uninherited = (std::stoi(SplitPoint[6]) == 1);
				Point.Effects = std::stoi(SplitPoint[7]);
				
				if (Point.Uninherited) this->UninheritedTimingPoints.push_back(Point);
				else this->InheritedTimingPoints.push_back(Point);
			}
			std::sort(this->UninheritedTimingPoints.begin(), this->UninheritedTimingPoints.end());
			std::sort(this->InheritedTimingPoints.begin(), this->InheritedTimingPoints.end());

			for (const std::string& ObjectString : this->m_Sections["HitObjects"])
			{
				HitObject Object;
				const std::vector<std::string> SplitObject = Utilities::Split(ObjectString, ',');
				Object.x = std::stoi(SplitObject[0]);
				Object.y = std::stoi(SplitObject[1]);
				Object.time = std::stoi(SplitObject[2]);
				Object.type = HitObjectType(std::stoi(SplitObject[3]), (this->HitObjects.empty()),
					(this->HitObjects.empty()) ? 1 : (this->HitObjects.back().type.comboColor));
				Object.hitSound = Hitsound(std::stoi(SplitObject[4]));

				// Parsing objectParams
				/// as Slider
				if (Object.type.Slider) {
					Object.sliderParams->curve.import(SplitObject[5]);
					Object.sliderParams->slides = std::stoi(SplitObject[6]);
					Object.sliderParams->length = std::stod(SplitObject[7]);
					if (SplitObject.size() >= 10) Object.sliderParams->edgeHitsounds.import(SplitObject[8], SplitObject[9]);
				}
				else Object.sliderParams = std::nullopt;
				/// as Spinner
				if (Object.type.Spinner) {
					Object.endTime = std::stoi(SplitObject[5]);
				}
				// while HoldNote is special case
				if (Object.type.HoldNote) {
					auto list = Utilities::Split(SplitObject[5], ':', true);
					Object.endTime = std::stoi(list.front());
					Object.hitSample = HitObject::HitSample(list.back());
				}

				// Parsing hitSample for other
				if (!Object.type.HoldNote) {
					if (Object.type.Slider && SplitObject.size() >= 11) {
						Object.hitSample = HitObject::HitSample(SplitObject[10]);
					}
					else if (Object.type.Spinner && SplitObject.size() >= 7) {
						Object.hitSample = HitObject::HitSample(SplitObject[6]);
					}
					else if (Object.type.HitCircle && SplitObject.size() >= 6) {
						Object.hitSample = HitObject::HitSample(SplitObject[5]);
					}
				}

				// Calculate endTime
				if (!Object.type.HoldNote)
					if (Object.type.HitCircle) { Object.endTime = Object.time; }
					else if (Object.type.Slider) {
						if (!this->Difficulty.SliderMultiplier.empty() && !this->UninheritedTimingPoints.empty())
						{
							TimingPoint currentTimeAsTimepoint;
							currentTimeAsTimepoint.Time = Object.time;

							// Get SliderMultiplier
							double_t SliderMultiplier = std::stod(this->Difficulty.SliderMultiplier);

							// Get BeatLength
							auto CurrentUninheritedTimingPoint =
								std::lower_bound(this->UninheritedTimingPoints.begin(), this->UninheritedTimingPoints.end(), currentTimeAsTimepoint);
							if (CurrentUninheritedTimingPoint == this->UninheritedTimingPoints.end())
								CurrentUninheritedTimingPoint = this->UninheritedTimingPoints.begin();
							double_t BeatLength = CurrentUninheritedTimingPoint->BeatLength;

							// Get SV
							double_t SV;
							auto CurrentInheritedTimingPoint =
								std::lower_bound(this->InheritedTimingPoints.begin(), this->InheritedTimingPoints.end(), currentTimeAsTimepoint);
							if (CurrentInheritedTimingPoint == this->InheritedTimingPoints.end()) SV = 1;
							else SV = SliderMultiplier * (100.0 / std::abs(CurrentInheritedTimingPoint->BeatLength));

							Object.endTime = 
								Object.time + Object.sliderParams->length / (SliderMultiplier * 100 * SV) * BeatLength * Object.sliderParams->slides;
						}
						else Object.endTime = Object.time;
					}

				this->HitObjects.push_back(Object);
			}
			std::sort(this->HitObjects.begin(), this->HitObjects.end(),
				[](const HitObject& A, const HitObject& B) { return A.time < B.time; });
		}
	private:
		void Reset()
		{
			this->Version = 14;
			this->UninheritedTimingPoints.clear();
			this->InheritedTimingPoints.clear();
			this->HitObjects.clear();
		}
	public:
		GeneralSection General;
		MetadataSection Metadata;
		EditorSection Editor;
		DifficultySection Difficulty;
		std::int32_t Version = 14;
		std::vector<TimingPoint> UninheritedTimingPoints = {};
		std::vector<TimingPoint> InheritedTimingPoints = {};
		std::vector<HitObject> HitObjects = {};
	private:
		std::ifstream m_CurrentStream;
		std::map<std::string, std::vector<std::string>> m_Sections = {};
	};
}