#include "ADC.hpp"

ADC::ADC() {
	iolib_init();
	channels_enabled = vector<bool>(7,false);
	buffers_size = vector<unsigned>(7,0);
	averages = vector<unsigned>(7,0);
	step_modes = vector<string>(7);
	buffers = vector<vector<unsigned> >(7);
}

ADC::~ADC() {
	iolib_free();
}

void ADC::Set(unsigned clk_div_data, unsigned open_dly_data, unsigned sample_dly_data) {
	clk_div = clk_div_data;
	sample_dly = sample_dly_data;       // Same value for all channels
	open_dly = open_dly_data;           // Same value for all channels
	BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div); // Set to One Shot mode
	                                                                  // Continuous mode not supported by this class
}

void ADC::SetChannel(unsigned ch, string step_mode, unsigned av, unsigned buf_size) {
	assert(channels_enabled[ch]);
	assert(av == 1 || av == 2 || av == 4 || av == 8 || av == 16);
	assert(step_mode.compare("SW_ONE_SHOOT") == 0 || \
	       step_mode.compare("SW_CONTINUOUS") == 0 || \
	       step_mode.compare("HW_ONE_SHOOT") == 0 || \
	       step_mode.compare("HW_CONTINUOUS") == 0);
	//
	averages[ch] = av;
	step_modes[ch] = step_mode;
	buffers_size[ch] = buf_size;   // TODO getBuffersSize(int ch);
	//
	buffers[ch] = vector<unsigned>(buf_size,0);

	unsigned av_conf;
	switch( av ) {
		case 1:
			av_conf = BBBIO_ADC_STEP_AVG_1;
			break;
		case 2:
			av_conf = BBBIO_ADC_STEP_AVG_2;
			break;
		case 4:
			av_conf = BBBIO_ADC_STEP_AVG_4;
			break;
		case 8:
			av_conf = BBBIO_ADC_STEP_AVG_8;
			break;
		case 16:
			av_conf = BBBIO_ADC_STEP_AVG_16;
			break;
		default:
			assert(0);
	}
	
	step_modes[ch] = step_mode;			// Store the describing string into step_modes vector (Useful to debug and <<)
	unsigned mode_conf;
	if ( step_mode.compare("SW_ONE_SHOOT") == 0)
	{
		mode_conf = BBBIO_ADC_STEP_MODE_SW_ONE_SHOOT;
	}
	else if ( step_mode.compare("SW_CONTINUOUS") == 0)
	{
		mode_conf = BBBIO_ADC_STEP_MODE_SW_CONTINUOUS;
	}
	else if ( step_mode.compare("HW_ONE_SHOOT") == 0)
	{
		mode_conf = BBBIO_ADC_STEP_MODE_HW_ONE_SHOOT;
	}
	else if ( step_mode.compare("HW_CONTINUOUS") == 0)
	{
		mode_conf = BBBIO_ADC_STEP_MODE_HW_CONTINUOUS;
	}
	else
		assert(0);
	//
	BBBIO_ADCTSC_channel_ctrl(ch, mode_conf, open_dly, sample_dly, av_conf, &buffers[ch][0], buffers[ch].size());
}

void ADC::EnableChannel(unsigned ch) {
	assert(channels_enabled[ch]);
	BBBIO_ADCTSC_channel_enable(ch);
}

void ADC::Start(unsigned nsamples) {
	BBBIO_ADCTSC_work(nsamples);      // TODO sample size
}

void ADC::DefineChannels(vector<unsigned> nch) {	
	assert(nch.size() <= 7);                           // N.B. The channels are AIN[0:6]
	for (unsigned i=0; i < nch.size(); i++) {
		assert(nch[i] < 7);
		unsigned c = nch[i];
		channels_enabled[c] = true;                     // Enable the selected channel
	}
}

ostream& operator<<(ostream& os, const ADC& myADC) {
	os << "Channel:\tClock Divider:\tOpen Delay:\tSample Delay:\tAverage Sample:\tBuffer size:\tStep Mode" << endl << endl;
	for (unsigned i=0; i < myADC.channels_enabled.size(); i++)
		if (myADC.channels_enabled[i]) {
			os << "AIN" << i << "\t\t" << myADC.clk_div << "\t\t" << myADC.open_dly << "\t\t" << myADC.sample_dly;
			os << "\t\t" << myADC.averages[i] << "\t\t" << myADC.buffers[i].size() << "\t\t" << myADC.step_modes[i] << endl;
		}
	return os;
}