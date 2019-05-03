#include <hls_stream.h>

void vibrato (hls::stream<float>	&in, //source of input samples
	hls::stream<float>	&out, //source of output samples
	float delay,				//Using the delay time of 0.5 seconds
	float depth
	);

