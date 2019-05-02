#include "vibrato.h"
#include <hls_stream.h>
#include <iostream>
#include <cmath>

using namespace std;
float tmp;

//Testing for a sample size of 96k
#define dataLength 3000


int main ()
{
	hls::stream<float> in ("input");
	hls::stream<float> out ("input");
	float delay;
	float depth;

	float phase = 0.0f;
	delay = 0.004;
	 depth = 0.32;

	for (int j = 0; j < dataLength; j++) {
		phase += 0.02f;

		in << sinf(phase);

		//cout << sinf(phase) <<" ";

		vibrato (in, out, delay, depth);


		out >> tmp;
		cout << tmp << " ";
	}



}

