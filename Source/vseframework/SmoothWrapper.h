/*
  ==============================================================================

    SmoothWrapper.h
    Created: 18 Apr 2024 3:17:39am
    Author:  Cornelius

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using SmoothLinear = juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>;
using SmoothMultiplicative = juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative>;

namespace vseframework
{
	class LinearWrapper
	{
	public:
		LinearWrapper(float sampleRate, float initialValue, float ramp = 0.1f, int channels = 1);

		void setValue(float value, int channel, bool instant = false);

		void setValue(float value, bool instant = false);

		float getNext(int channel);

		float value(int channel);

		float value();

		void reset(float sampleRate, float ramp);

	private:
		std::map<int, SmoothLinear> smooth;

		float sampleRate, ramp;
		int channels;
	};

	class MultiplicativeWrapper
	{
	public:
		MultiplicativeWrapper(float sampleRate, float initialValue, float ramp = 0.1f, int channels = 1);

		void setValue(float value, int channel, bool instant = false);

		void setValue(float value, bool instant = false);

		float getNext(int channel);

		float value(int channel);

		float value();

		void reset(float sampleRate, float ramp);

	private:
		std::map<int, SmoothMultiplicative> smooth;

		float sampleRate, ramp;
		int channels;
	};
}