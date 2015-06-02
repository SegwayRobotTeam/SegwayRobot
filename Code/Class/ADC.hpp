#ifndef ADC_HPP
#define ADC_HPP

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "./BBBIOlib/BBBio_lib/BBBiolib.h" // extern C already present
using namespace std;

class ADC
{
	friend ostream& operator<<(ostream& os, const ADC& myADC);
	private:
		unsigned clk_div;
		unsigned sample_dly;
		unsigned open_dly;
		vector<bool> channels_enabled;
		vector<unsigned> buffers_size;
		vector<unsigned> averages;
		vector<string> step_modes;
		//
		vector<vector<unsigned> > buffers;

	public:
		ADC();
		~ADC();
		void DefineChannels(vector<unsigned> ch);
		void Set(unsigned clk_div_data, unsigned open_dly_data, unsigned sample_dly_data);
		void SetChannel(unsigned ch, string step_mode, unsigned av, unsigned buf_size);
		void EnableChannel(unsigned ch);
		void Start(unsigned nsamples);
		vector<unsigned> GetBuffer(unsigned ch) { return buffers[ch]; };
		vector<bool> GetChannelsEnabled() { return channels_enabled; };
};

#endif