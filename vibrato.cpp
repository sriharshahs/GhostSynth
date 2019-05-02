#include <hls_stream.h>
#include <math.h>
#include <iostream>
#include <stdio.h>

#define PI 3.141592653589793
//Modulation frequency of the sinusoidal LFO for the Vibrato effect
#define modFreq 6.0
#define SAMPLERATE 48000
//Delay in terms of no of samples
//For achieving the 0.5 second delay considering the sample rate of 48khz
static float buffer[SAMPLERATE/2];
static int delayBufferVal = SAMPLERATE/2;
//static int depthVal;
static float offset;
static int readoffset;
static int readPos, delta, nextOut;
static int writePos;
static float phaseStep;
static bool Initialized;

float vibrato_init() {
	float phase;
	phase= 2 * PI * modFreq / SAMPLERATE;
	return phase;
}

void vibrato(
	hls::stream<float>	&in, //source of input samples
	hls::stream<float>	&out, //source of output samples
	float delay,				//Using the delay time of 0.5 seconds
	float depth				//Using the depth value as 50%
	){
#pragma HLS INTERFACE axis register both port=out
#pragma HLS INTERFACE axis register both port=in
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE s_axilite port=delay bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=depth bundle=CTRL_BUS


	//depthVal= depth * SAMPLERATE;


	if (!Initialized)
	{
		phaseStep = vibrato_init();
	}

	Initialized = true;

	float sinval = sin(phaseStep);
	sinval = (sinval + 1) * 0.5;

	offset = delay * (1 + sinval * 0.32) * SAMPLERATE;


	//printf("Offset %f, delay %d, sinval %f, depth %f, SAMPLERATAE %d\n",offset,
		//	delay, sinval, depth, SAMPLERATE);

	if (offset > delayBufferVal)
	{
		offset = (float) delayBufferVal;
	}

	readoffset = writePos - offset;

	if (readoffset > delayBufferVal)
		readoffset = readoffset - delayBufferVal;
	else if (readoffset <= 0)
		readoffset = readoffset + delayBufferVal;


	//readoffset = (readoffset >= 0 ? (readoffset < delayBufferVal ? readoffset :
		//							readoffset - delayBufferVal) : readoffset + delayBufferVal);

	//Interpolating samples to find the output stream

	readPos = (int)readoffset;
	delta = readoffset - readPos;
	if (readPos != delayBufferVal-1)
		nextOut = buffer[readPos+1];
	else
		nextOut = buffer[0];

	//nextOut = (readPos != delayBufferVal-1 ? buffer[readPos+1]: buffer[0]);

	out << buffer[readPos] + delta * (nextOut - buffer[readPos]);

	in >> buffer[writePos];

	//printf("BUFFER IN %f\n", buffer[writePos]);

	//Incrementing write position pointers
	if(writePos == (delayBufferVal-1))
		writePos = 0;
	else
		writePos = writePos + 1;

	//Incrementing the phase offset by 0.2
	phaseStep = phaseStep + 0.2;


    }
