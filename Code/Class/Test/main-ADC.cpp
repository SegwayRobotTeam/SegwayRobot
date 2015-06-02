#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "ADC.hpp"
//#include "./BBBIOlib/BBBio_lib/BBBiolib.h"
using namespace std;

int main()
{
	//
	// Define parameters
	//
	// Oss: max sample frequency = 200kHz
	//      If (average != 1), the product:
	//      (average*sample_freq) < 200kHz
	//      
	const int clk_div = 160;     // fc = 10k
	const int open_dly = 0;
	const int sample_dly = 1;
	unsigned average = 16;       // average = 16
	//unsigned SAMPLE_SIZE = 20;
	unsigned num_acquisitions = 3;
	unsigned BUFFER_SIZE = 10;
	unsigned sample;

	//
	// Configure ADC
	//
	ADC myADC;
	myADC.Set(clk_div, open_dly, sample_dly);
	//
	unsigned chvec[] = {0,1};
	vector<unsigned> channels(chvec, chvec + sizeof(chvec) / sizeof(unsigned) );
	string step_mode("SW_CONTINUOUS");
	//string step_mode("SW_ONE_SHOOT"); // Doesn't work

	//
	// Configure channels
	//
	myADC.DefineChannels(channels);
	myADC.SetChannel(0, step_mode, average, BUFFER_SIZE);
	myADC.SetChannel(1, step_mode, average, BUFFER_SIZE);

	//
	// For all the acquisitions
	//
	for (unsigned i=0; i < num_acquisitions; i++)
	{
		cout << "Start sampling, fetch " << BUFFER_SIZE << " samples" << endl;
		//
		// For all enabled channels
		//
		for (unsigned j=0; j < channels.size(); j++)
			myADC.EnableChannel(j);
		//
		myADC.Start(BUFFER_SIZE);
		//
		for (unsigned j=0; j < channels.size(); j++)
		{
			//printf("AIN 0 :\n");
			cout << "AIN " << j << ":" << endl;
			//
			// Iterate inside the buffer
			//
			for(unsigned k = 0 ; k < myADC.GetBuffer(j).size() ; k++)
			{
				sample = myADC.GetBuffer(j)[k];
				//printf("\t[sample : %d , %f v]\n", sample, ((float)sample / 4095.0f) * 1.8f);
				cout << "\t[sample: " << sample << ", " << ((float)sample/4095.0f)*1.8f << "V]" << endl;
			}
			cout << endl;
			this_thread::sleep_for(chrono::microseconds(1000000));
		}
	}
}