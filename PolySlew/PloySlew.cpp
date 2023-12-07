#ifndef __PolySlew_H
#include "PolySlew.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) { return new PolySlew(audioMaster); }

PolySlew::PolySlew(audioMasterCallback audioMaster) :
	AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 1.0;
	C = 1.0;
	D = 1.0;
	E = 0.0;

	sR1 = 0;
	sL2 = 0;
	sL3 = 0;
	sR1 = 0;
	sR2 = 0;
	sR3 = 0;

	Algorithm = 0; //Definitely fucks stuff up, not sure what else to say here
	numAlgorithm = 4;

	_canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
	_canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
	_canDo.insert("x2in2out");
	setNumInputs(kNumInputs);
	setNumOutputs(kNumOutputs);
	setUniqueID(kUniqueId);
	canProcessReplacing();     // supports output replacing
	canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
	vst_strncpy(_programName, "Default", kVstMaxProgNameLen); // default program name
}

PolySlew::~PolySlew() {}
VstInt32 PolySlew::getVendorVersion() { return 1000; }
void PolySlew::setProgramName(char* name) { vst_strncpy(_programName, name, kVstMaxProgNameLen); }
void PolySlew::getProgramName(char* name) { vst_strncpy(name, _programName, kVstMaxProgNameLen); }
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 PolySlew::getChunk (void** data, bool isPreset)
{
	float* chunkData = (float*)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;

	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 PolySlew::setChunk(void* data, VstInt32 byteSize, bool isPreset)
{
	float* chunkData = (float*)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);

	/* calculate any other fields you need here - you could copy in
	 code from setParameter() here. */
	return 0;
}

void PolySlew::setParameter(VstInt32 index, float value) {
	switch (index) {
		case kParamA: A = value; break;
		case kParamB: B = value; break;
		case kParamC: C = value; break;
		case kParamD: D = value; break;
		case kParamE: E = value; break;
		default: throw; // unknown parameter, shouldn't happen!
	}
}

float PolySlew::getParameter(VstInt32 index) {
	switch (index) {
		case kParamA: return A; break;
		case kParamB: return B; break;
		case kParamC: return C; break;
		case kParamD: return D; break;
		case kParamE: return E; break;
		default: break; // unknown parameter, shouldn't happen!
	} return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PolySlew::getParameterName(VstInt32 index, char* text) {
	switch (index) {
		case kParamA: vst_strncpy(text, "Constant", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy(text, "Linear", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy(text, "Quadratic", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy(text, "Cubic", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy(text, "Algorithm", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this is our labels for displaying in the VST host
}

void PolySlew::getParameterDisplay(VstInt32 index, char* text) {
	switch (index) {
		case kParamA: float2string(A, text, kVstMaxParamStrLen); break;
		case kParamB: float2string(B, text, kVstMaxParamStrLen); break;
		case kParamC: float2string(C, text, kVstMaxParamStrLen); break;
		case kParamD: float2string(D, text, kVstMaxParamStrLen); break;
		case kParamE: float2string(E, text, kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PolySlew::getParameterLabel(VstInt32 index, char* text) {
	switch (index) {
		case kParamA: vst_strncpy(text, "", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy(text, "", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy(text, "", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy(text, "", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy(text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	}
}

VstInt32 PolySlew::canDo(char* text)
{
	return (_canDo.find(text) == _canDo.end()) ? -1 : 1;
} // 1 = yes, -1 = no, 0 = don't know

bool PolySlew::getEffectName(char* name) {
	vst_strncpy(name, "PolySlew", kVstMaxProductStrLen); return true;
}

VstPlugCategory PolySlew::getPlugCategory() { return kPlugCategEffect; }

bool PolySlew::getProductString(char* text) {
	vst_strncpy(text, "KiosRains PolySlew", kVstMaxProductStrLen); return true;
}

bool PolySlew::getVendorString(char* text) {
	vst_strncpy(text, "KiosRains", kVstMaxVendorStrLen); return true;
}
