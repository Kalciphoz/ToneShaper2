/*
  ==============================================================================

    SmoothWrapper.cpp
    Created: 18 Apr 2024 3:17:39am
    Author:  Cornelius

  ==============================================================================
*/

#include "SmoothWrapper.h"

namespace vseframework
{
	LinearWrapper::LinearWrapper(float samplerate, float initialValue, float r, int c)
	{
		sampleRate = samplerate;
		ramp = r;
		channels = c;

		for (int i = 0; i < channels; i += 1)
		{
			smooth[i] = SmoothLinear(initialValue);
			smooth[i].reset(sampleRate, r);
		}
	}

	void LinearWrapper::setValue(float value, int channel, bool instant)
	{
		if (instant)
			smooth[channel].setCurrentAndTargetValue(value);
		else
			smooth[channel].setTargetValue(value);
	}

	void LinearWrapper::setValue(float value, bool instant) { setValue(value, 0, instant); }

	float LinearWrapper::getNext(int channel)
	{
		if (channels == 1)
		{
			if (channel == 0)
				return smooth[0].getNextValue();
			else
				return smooth[0].getCurrentValue();
		}
		else
			return smooth[channel].getNextValue();
	}

	void LinearWrapper::reset(float sr, float rl)
	{
		sampleRate = sr;
		ramp = rl;
		for (int i = 0; i < channels; i += 1)
			smooth[i].reset(sampleRate, ramp);
	}

	float LinearWrapper::value(int channel) { return smooth[channel].getCurrentValue(); }

	float LinearWrapper::value() { return value(0); }



	MultiplicativeWrapper::MultiplicativeWrapper(float samplerate, float initialValue, float r, int c)
	{
		sampleRate = samplerate;
		ramp = r;
		channels = c;

		for (int i = 0; i < channels; i += 1)
		{
			smooth[i] = SmoothMultiplicative(initialValue);
			smooth[i].reset(sampleRate, r);
		}
	}

	void MultiplicativeWrapper::setValue(float value, int channel, bool instant)
	{
		if (instant)
			smooth[channel].setCurrentAndTargetValue(value);
		else
			smooth[channel].setTargetValue(value);
	}

	void MultiplicativeWrapper::setValue(float value, bool instant) { setValue(value, 0, instant); }

	float MultiplicativeWrapper::getNext(int channel)
	{
		if (channels == 1)
		{
			if (channel == 0)
				return smooth[0].getNextValue();
			else
				return smooth[0].getCurrentValue();
		}
		else
			return smooth[channel].getNextValue();
	}

	void MultiplicativeWrapper::reset(float sr, float rl)
	{
		sampleRate = sr;
		ramp = rl;
		for (int i = 0; i < channels; i += 1)
			smooth[i].reset(sampleRate, ramp);
	}

	float MultiplicativeWrapper::value(int channel) { return smooth[channel].getCurrentValue(); }

	float MultiplicativeWrapper::value() { return value(0); }
}