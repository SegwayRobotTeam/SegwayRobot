#include "MotorDriver.hpp"

//
// Constructors / Destructors
//
MotorDriver::MotorDriver() {
	duty_1A = 0;
	duty_1B = 0;
	duty_2A = 0;
	duty_2B = 0;
	freq1 = 0;
	freq2 = 0;
	CHANNEL1 = -1;
	CHANNEL2 = -1;
	iolib_init();
}

MotorDriver::~MotorDriver() {
	Stop();
	iolib_free();
}

//
// Set methods
//
void MotorDriver::SetChannels(int ch1, int ch2) {
	if (ch1 == 0)
		CHANNEL1 = 0;
	else if (ch1 == 1)
		CHANNEL1 = 1;
	else if (ch1 == 2)
		CHANNEL1 = 2;
	else
		assert(0);

	if (ch2 == 0)
		CHANNEL2 = 0;
	else if (ch2 == 1)
		CHANNEL2 = 1;
	else if (ch2 == 2)
		CHANNEL2 = 2;
	else
		assert(0);
	assert(CHANNEL1 != CHANNEL2);
}

void MotorDriver::SetDuty1(float d1a, float d1b) {  // Oss: if a duty is 0 the BBBIOlib is not able to
	assert(d1a >= 0 && d1b <= 100);                  //      apply the right frequency but only freq/2.
	assert(CHANNEL1 >= 0);                           // Dirty solution: if duty=0 -> set duty=0.1%
	//
	if (d1a == 0)
		duty_1A = 1;
	else
		duty_1A = d1a;
	//
	if (d1b == 0)
		duty_1B = 1;
	else
		duty_1B = d1b;
	//
	//cout << freq1 << endl;
	BBBIO_PWMSS_Setting(CHANNEL1, freq1, duty_1A, duty_1B);
}

void MotorDriver::SetDuty2(float d2a, float d2b) {
	assert(d2a >= 0 && d2b <= 100);
	assert(CHANNEL2 >= 0);
	//
	if (d2a == 0)
		duty_2A = 1;
	else
		duty_2A = d2a;
	//
	if (d2b == 0)
		duty_2B = 1;
	else
		duty_2B = d2b;
	//duty_2A = d2a;
	//duty_2B = d2b;
	//cout << freq2 << endl;
	//cout << duty_2A << " " << duty_2B << endl;
	BBBIO_PWMSS_Setting(CHANNEL2, freq2, duty_2A, duty_2B);
}

void MotorDriver::SetDuty(float d1a, float d1b, float  d2a, float d2b) {
	SetDuty1(d1a, d1b);
	SetDuty2(d2a, d2b);
}

void MotorDriver::SetFrequency(float f1, float f2) {
	freq1 = f1;
	freq2 = f2;
}

void MotorDriver::SetMaxSpeed(float s) {
	MaxSpeed = s;
}

//
// Start / Stop methods
//
void MotorDriver::StartCh1() {
	assert(CHANNEL1 >= 0);
	BBBIO_ehrPWM_Enable(CHANNEL1);
}

void MotorDriver::StartCh2() {
	assert(CHANNEL2 >= 0);
	BBBIO_ehrPWM_Enable(CHANNEL2);
}

void MotorDriver::Start() {
	StartCh1();
	StartCh2();
}


void MotorDriver::StopCh1() {
	BBBIO_ehrPWM_Disable(CHANNEL1);
}

void MotorDriver::StopCh2() {
	BBBIO_ehrPWM_Disable(CHANNEL2);
}

void MotorDriver::Stop() {
	StopCh1();
	StopCh2();
}

void MotorDriver::ApplySpeedPercentage1(float speed) {
	assert(speed <= 100);
	assert(speed >= -100);
	if (speed > 0) {
		SetDuty1(speed, 0);
		StartCh1();
	}
	else if (speed < 0) {
		SetDuty1(0, -speed);
		StartCh1();
	}
	else {
		SetDuty1(0, 0);
		StopCh1();
	}
}

void MotorDriver::ApplySpeedPercentage2(float speed) {
	assert(speed <= 100);
	assert(speed >= -100);
	if (speed > 0) {
		SetDuty2(speed, 0);
		StartCh2();
	}
	else if (speed < 0) {
		SetDuty2(0, -speed);
		StartCh2();
	}
	else {
		SetDuty2(0, 0);
		StopCh2();
	}
}

void MotorDriver::ApplySpeedPercentage(float speed) {
	assert(speed <= 100);
	assert(speed >= -100);
	if (speed > 0) {
		SetDuty1(speed, 0);
		SetDuty2(speed, 0);
		Start();
	}
	else if (speed < 0) {
		SetDuty1(0, -speed);
		SetDuty2(0, -speed);
		Start();
	}
	else {
		SetDuty1(0, 0);
		SetDuty2(0, 0);
		Stop();
	}
}

ostream& operator<<(ostream& os, const MotorDriver& myMD) {
	os << "\t\tPWMSSX" << "\tFreq\tdutyA\tdutyB" << endl;
	os << "CHANNEL1:\t" << myMD.CHANNEL1 << "\t" << myMD.freq1 << "\t" << myMD.duty_1A << "\t"<< myMD.duty_1B << endl;
	os << "CHANNEL2:\t" << myMD.CHANNEL2 << "\t" << myMD.freq2 << "\t" << myMD.duty_2A << "\t"<< myMD.duty_2B << endl;
	return os;
}

/*void MotorDriver::StartDelayed(unsigned delay) {
	// 1 -> delay -> 2
	if (delay > 0) {
		StartCh1();
		this_thread::sleep_for(chrono::microseconds(delay));
		StartCh2();
	}
	// 2 -> delay -> 1
	else {
		StartCh1();
		this_thread::sleep_for(chrono::microseconds(delay));
		StartCh2();
	}
}*/

/*void MotorDriver::ApplySpeedPercentageDelayed(float speed, unsigned delay) {
	assert(speed <= 100);
	assert(speed >= -100);
	if (delay > 0)
	{
		if (speed > 0) {
			SetDuty1(speed, 0);
			SetDuty2(speed, 0);
			//
			StartCh1();
			this_thread::sleep_for(chrono::microseconds(delay));
			StartCh2();
		}
		else if (speed < 0) {
			SetDuty1(0, -speed);
			SetDuty2(0, -speed);
			//
			StartCh1();
			this_thread::sleep_for(chrono::microseconds(delay));
			StartCh2();
		}
		else {
			SetDuty1(0, 0);
			SetDuty2(0, 0);
			//
			Stop();
		}
	}
	else
	{
		if (speed > 0) {
			SetDuty1(speed, 0);
			SetDuty2(speed, 0);
			//
			StartCh2();
			this_thread::sleep_for(chrono::microseconds(delay));
			StartCh1();
		}
		else if (speed < 0) {
			SetDuty1(0, -speed);
			SetDuty2(0, -speed);
			//
			StartCh2();
			this_thread::sleep_for(chrono::microseconds(delay));
			StartCh1();
		}
		else {
			SetDuty1(0, 0);
			SetDuty2(0, 0);
			Stop();
		}
	}
}*/