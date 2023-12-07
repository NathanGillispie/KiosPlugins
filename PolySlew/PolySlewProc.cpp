#ifndef __PolySlew_H
#include "PolySlew.h"
#endif

void PolySlew::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	float delta = 1e-20;

	float limit = powf(A,2) * 2.0;
	float slew = powf(B,8) * 2.0;
	float quad = powf(C,8) * 2.0;
	float cube = powf(D,8) * 4.0/3.0;
	int Algorithm = floor((E * (1 - delta) + delta * .5) * numAlgorithm);
	
	while (--sampleFrames >= 0)
	{
		//Shift samples
		sL0 = sL1;
		sL1 = sL2;
		sL2 = sL3;
		sL3 = *in1;
		sR0 = sR1;
		sR1 = sR2;
		sR2 = sR3;
		sR3 = *in2;

		float aL3 = 1.0 / 6.0 * (-sL0 + 3 * sL1 - 3 * sL2 + sL3);
		float aR3 = 1.0 / 6.0 * (-sR0 + 3 * sR1 - 3 * sR2 + sR3);

		float aL2 = .5 * (sL1 - (2 * sL2) + sL3);
		float aR2 = .5 * (sR1 - (2 * sR2) + sR3);

		switch (Algorithm) {
		case 0: //Normal PolySlew
			//Cubic limiting
			if (aL3 > cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 + 6 * cube;
			else if (aL3 < -cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 - 6 * cube;

			if (aR3 > cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 + 6 * cube;
			else if (aR3 < -cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 - 6 * cube;

			//Quadratic limiting
			if (aL2 > quad) sL3 = -sL1 + 2 * sL2 + 2 * quad;
			else if (aL2 < -quad) sL3 = -sL1 + 2 * sL2 + 2 * (-quad);

			if (aR2 > quad) sR3 = -sR1 + 2 * sR2 + 2 * quad;
			else if (aR2 < -quad) sR3 = -sR1 + 2 * sR2 + 2 * (-quad);

			//Slewing inputs
			if (sL3 - sL2 > slew) sL3 = sL2 + slew;
			else if (sL3 - sL2 < -slew) sL3 = sL2 - slew;

			if (sR3 - sR2 > slew) sR3 = sR2 + slew;
			else if (sR3 - sR2 < -slew) sR3 = sR2 - slew;

			//Conventional limiting
			if (sL3 > limit) sL3 = limit;
			else if (sL3 < -limit) sL3 = -limit;

			if (sR3 > limit) sR3 = limit;
			else if (sR3 < -limit) sR3 = -limit;
			break;

		case 1: //Inverted Polyslew
			//Cubic limiting
			if (aL3 > cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 - 6 * cube;
			else if (aL3 < -cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 + 6 * cube;

			if (aR3 > cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 - 6 * cube;
			else if (aR3 < -cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 + 6 * cube;

			//Quadratic limiting
			if (aL2 > quad) sL3 = -sL1 + 2 * sL2 - 2 * quad;
			else if (aL2 < -quad) sL3 = -sL1 + 2 * sL2 - 2 * (-quad);

			if (aR2 > quad) sR3 = -sR1 + 2 * sR2 - 2 * quad;
			else if (aR2 < -quad) sR3 = -sR1 + 2 * sR2 - 2 * (-quad);

			//Slewing inputs
			if (sL3 - sL2 > slew) sL3 = sL2 - slew;
			else if (sL3 - sL2 < -slew) sL3 = sL2 + slew;

			if (sR3 - sR2 > slew) sR3 = sR2 - slew;
			else if (sR3 - sR2 < -slew) sR3 = sR2 + slew;

			//non-Conventional limiting
			if (sL3 > limit) sL3 = 1.0;
			else if (sL3 < -limit) sL3 = -1.0;

			if (sR3 > limit) sR3 = 1.0;
			else if (sR3 < -limit) sR3 = -1.0;
			break;

		case 2: //Reverse PolySlew
			//Conventional limiting
			if (sL3 > limit) sL3 = limit;
			else if (sL3 < -limit) sL3 = -limit;

			if (sR3 > limit) sR3 = limit;
			else if (sR3 < -limit) sR3 = -limit;

			//Slewing inputs
			if (sL3 - sL2 > slew) sL3 = sL2 + slew;
			else if (sL3 - sL2 < -slew) sL3 = sL2 - slew;

			if (sR3 - sR2 > slew) sR3 = sR2 + slew;
			else if (sR3 - sR2 < -slew) sR3 = sR2 - slew;

			//Quadratic limiting
			if (aL2 > quad) sL3 = -sL1 + 2 * sL2 + 2 * quad;
			else if (aL2 < -quad) sL3 = -sL1 + 2 * sL2 - 2 * quad;

			if (aR2 > quad) sR3 = -sR1 + 2 * sR2 + 2 * quad;
			else if (aR2 < -quad) sR3 = -sR1 + 2 * sR2 - 2 * quad;

			//Cubic limiting
			if (aL3 > cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 + 6 * cube;
			else if (aL3 < -cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 - 6 * cube;

			if (aR3 > cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 + 6 * cube;
			else if (aR3 < -cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 - 6 * cube;
			break;

		default: //100% dry
			break;
		}

		*out1 = sL3;
		*out2 = sR3;

		in1++;
		in2++;
		out1++;
		out2++;
	}
}

void PolySlew::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	double delta = 1e-20;

	double limit = pow(A,2) * 2.0;
	double slew = pow(B,8) * 2.0;
	double quad = pow(C,8) * 2.0;
	double cube = pow(D,8) * 4.0/3.0;
	int Algorithm = floor((E * (1 - delta) + delta * .5) * numAlgorithm);

	while (--sampleFrames >= 0)
	{
		//Shift samples
		sL0 = sL1;
		sL1 = sL2;
		sL2 = sL3;
		sL3 = *in1;
		sR0 = sR1;
		sR1 = sR2;
		sR2 = sR3;
		sR3 = *in2;

		double aL3 = 1.0 / 6.0 * (-sL0 + 3 * sL1 - 3 * sL2 + sL3);
		double aR3 = 1.0 / 6.0 * (-sR0 + 3 * sR1 - 3 * sR2 + sR3);

		double aL2 = .5 * (sL1 - (2 * sL2) + sL3);
		double aR2 = .5 * (sR1 - (2 * sR2) + sR3);


		switch (Algorithm) {
		case 0: //Normal PolySlew
			//Cubic limiting
			if (aL3 > cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 + 6 * cube;
			else if (aL3 < -cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 - 6 * cube;

			if (aR3 > cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 + 6 * cube;
			else if (aR3 < -cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 - 6 * cube;

			//Quadratic limiting
			if (aL2 > quad) sL3 = -sL1 + 2 * sL2 + 2 * quad;
			else if (aL2 < -quad) sL3 = -sL1 + 2 * sL2 + 2 * (-quad);

			if (aR2 > quad) sR3 = -sR1 + 2 * sR2 + 2 * quad;
			else if (aR2 < -quad) sR3 = -sR1 + 2 * sR2 + 2 * (-quad);

			//Slewing inputs
			if (sL3 - sL2 > slew) sL3 = sL2 + slew;
			else if (sL3 - sL2 < -slew) sL3 = sL2 - slew;

			if (sR3 - sR2 > slew) sR3 = sR2 + slew;
			else if (sR3 - sR2 < -slew) sR3 = sR2 - slew;

			//Conventional limiting
			if (sL3 > limit) sL3 = limit;
			else if (sL3 < -limit) sL3 = -limit;

			if (sR3 > limit) sR3 = limit;
			else if (sR3 < -limit) sR3 = -limit;
			break;

		case 1:
			//Cubic limiting
			if (aL3 > cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 - 6 * cube;
			else if (aL3 < -cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 + 6 * cube;

			if (aR3 > cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 - 6 * cube;
			else if (aR3 < -cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 + 6 * cube;

			//Quadratic limiting
			if (aL2 > quad) sL3 = -sL1 + 2 * sL2 - 2 * quad;
			else if (aL2 < -quad) sL3 = -sL1 + 2 * sL2 - 2 * (-quad);

			if (aR2 > quad) sR3 = -sR1 + 2 * sR2 - 2 * quad;
			else if (aR2 < -quad) sR3 = -sR1 + 2 * sR2 - 2 * (-quad);

			//Slewing inputs
			if (sL3 - sL2 > slew) sL3 = sL2 - slew;
			else if (sL3 - sL2 < -slew) sL3 = sL2 + slew;

			if (sR3 - sR2 > slew) sR3 = sR2 - slew;
			else if (sR3 - sR2 < -slew) sR3 = sR2 + slew;

			//non-Conventional limiting
			if (sL3 > limit) sL3 = 1.0;
			else if (sL3 < -limit) sL3 = -1.0;

			if (sR3 > limit) sR3 = 1.0;
			else if (sR3 < -limit) sR3 = -1.0;
			break;

		case 2: //Reverse PolySlew
			//Conventional limiting
			if (sL3 > limit) sL3 = limit;
			else if (sL3 < -limit) sL3 = -limit;

			if (sR3 > limit) sR3 = limit;
			else if (sR3 < -limit) sR3 = -limit;

			//Slewing inputs
			if (sL3 - sL2 > slew) sL3 = sL2 + slew;
			else if (sL3 - sL2 < -slew) sL3 = sL2 - slew;

			if (sR3 - sR2 > slew) sR3 = sR2 + slew;
			else if (sR3 - sR2 < -slew) sR3 = sR2 - slew;

			//Quadratic limiting
			if (aL2 > quad) sL3 = -sL1 + 2 * sL2 + 2 * quad;
			else if (aL2 < -quad) sL3 = -sL1 + 2 * sL2 - 2 * quad;

			if (aR2 > quad) sR3 = -sR1 + 2 * sR2 + 2 * quad;
			else if (aR2 < -quad) sR3 = -sR1 + 2 * sR2 - 2 * quad;

			//Cubic limiting
			if (aL3 > cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 + 6 * cube;
			else if (aL3 < -cube) sL3 = sL0 - 3 * sL1 + 3 * sL2 - 6 * cube;

			if (aR3 > cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 + 6 * cube;
			else if (aR3 < -cube) sR3 = sR0 - 3 * sR1 + 3 * sR2 - 6 * cube;
			break;

		default: //100% dry
			break;
		}

		*out1 = sL3;
		*out2 = sR3;

		in1++;
		in2++;
		out1++;
		out2++;
	}	
}