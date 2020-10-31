/*
  ==============================================================================

    Limiter.cpp
    Created: 28 Mar 2020 11:32:15am
    Author:  Alex

  ==============================================================================
*/

#include "Limiter.h"

Limiter::Limiter() {
    buffer = DelayBuffer(10, 2);
    gain = 1;
    xPeak = 0;
    coeff = 0;
}

float Limiter::calculateSample(float samp, float lt, float at, float rt) {
    float amplitude = abs(samp);
    if (amplitude > xPeak) coeff = at;
    else coeff = rt;

    xPeak = (1 - coeff) * xPeak + coeff * amplitude;
    float filter = std::min(1.0f, lt / xPeak);

    if (filter < gain) coeff = at;
    else coeff = rt;

    gain = (1 - coeff) * gain + coeff * filter;

    float limitedSamp = gain * buffer.getData();
    buffer.setData(samp);
    buffer.nextSample();
    return limitedSamp;
}
