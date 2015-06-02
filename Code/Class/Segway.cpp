#include "Segway.hpp"

Segway::Segway(// MotorDriver
					unsigned PWMfreq,
		         pair<unsigned, unsigned> motor_channels,
		         // eQEP
		         pair<unsigned,unsigned> eqep_channels,
		         unsigned eqep_period,
		         unsigned eqep_mode,
		         // ADC
		         unsigned clk_div,
		         unsigned open_dly,
		         unsigned sample_dly,
		         unsigned average,
		         unsigned buffer_size,
		         vector<unsigned> adc_channels,
		         string step_mode)
//eQEP1(eqep_channels.first,) eQEP2(eqep_channels.second, )
{
	//
	// IMU
	//
	myIMU.Init();

	//
	// Motor Driver
	//
	myMD.SetChannels(motor_channels.first, motor_channels.second);
	myMD.SetFrequency(PWMfreq, PWMfreq);

	//
	// eQEP
	//
	eQEP1.SetChannel(eqep_channels.first);
	eQEP1.SetPeriod(eqep_period);
	eQEP1.SetMode(eqep_mode);
	eQEP1.Enable();
	//
	eQEP2.SetChannel(eqep_channels.second);
	eQEP2.SetPeriod(eqep_period);
	eQEP2.SetMode(eqep_mode);
	eQEP2.Enable();

	//
	// Initialize position and speed
	//
	position.first  = 0;
	position.second = 0;
	//
	speed.first  = 0;
	speed.second = 0;
	//
	angular_position.first  = 0;
	angular_position.second = 0;
	//
	angular_speed.first  = 0;
	angular_speed.second = 0;
	//
	angular_position_old.first = 0;
	angular_position_old.second = 0;

	//
	// ADC
	//
	myADC.Set(clk_div, open_dly, sample_dly);
	//
	myADC.DefineChannels(adc_channels);
	//
	for (unsigned i=0; i < myADC.GetChannelsEnabled().size(); i++)
		if (myADC.GetChannelsEnabled()[i])
			myADC.SetChannel(i, step_mode, average, buffer_size);
}

Segway::~Segway()
{
	Stop();
}

void Segway::AcquireData(unsigned num_adc_samples)
{
	//
	// IMU
	//
	myIMU.getNewValues();

	//
	// eQEP
	//
	// N.B. overflow of the encoder counter reglected since
	//      the segway is not supposed to travel so much
	//
	//pair<float,float> position_old = position;
	//pair<float,float> angular_position_old = angular_position;
	//
	//
	angular_position_old = angular_position;
	//
	eQEP1.AcquireData();
	eQEP2.AcquireData();
	//
	angular_position.first  = GeteQEP1value() / (ENCODERCOUNTS*NEDGE) * 360;
	angular_position.second = GeteQEP2value() / (ENCODERCOUNTS*NEDGE) * 360;
	//
	angular_speed.first  = (angular_position.first  - angular_position_old.first ) / DELTAT;
	angular_speed.second = (angular_position.second - angular_position_old.second) / DELTAT;
	//
	position.first  = angular_position.first * DEG2RAD * RADIUS/GEAR_RATIO;
	position.second = angular_position.second* DEG2RAD * RADIUS/GEAR_RATIO;
	//
	speed.first  = angular_speed.first  * DEG2RAD * RADIUS / GEAR_RATIO;
	speed.second = angular_speed.second * DEG2RAD * RADIUS / GEAR_RATIO;

	//
	// ADC
	//
	AcquireADCData(num_adc_samples);
}

void Segway::AcquireADCData(unsigned nsampl)
{
	for (unsigned i=0; i < myADC.GetChannelsEnabled().size(); i++)
	{
		assert(myADC.GetBuffer(i).size() <= nsampl);
		//
		if (myADC.GetChannelsEnabled()[i])
			myADC.EnableChannel(i);
	}
	myADC.Start(nsampl);
}

float Segway::GetADCvalue(unsigned adc_channel)
{
	return ( (float)myADC.GetBuffer(adc_channel)[0] / 4095.0f ) * 1.8f; // TODO: for now only first value of the buffer
}

void Segway::SetSpeed(int speed)
{
	myMD.ApplySpeedPercentage(speed);
}

void Segway::SetSpeed1(int speed)
{
	myMD.ApplySpeedPercentage1(speed);
}

void Segway::SetSpeed2(int speed)
{
	myMD.ApplySpeedPercentage2(speed);
}

void Segway::Stop()
{
	eQEP1.Disable();
	eQEP2.Disable();
	myMD.Stop();
}