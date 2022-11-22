#ifndef __FloatCrusher_H
#include "FloatCrusher.h"
#endif

void FloatCrusher::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	float gain = powf(B*2,2)/4;
	float factor = powf(2.0,-128*(A/8.0+0.875));
	float inversefactor = 1.0 / factor;

	while (--sampleFrames >= 0)
	{
		float inputSampleL = *in1;
		float inputSampleR = *in2;

		inputSampleL *= factor;
		inputSampleR *= factor;

		inputSampleL *= inversefactor;
		inputSampleR *= inversefactor;

		*out1 = inputSampleL*gain;
		*out2 = inputSampleR*gain;

		in1++;
		in2++;
		out1++;
		out2++;
	}
}