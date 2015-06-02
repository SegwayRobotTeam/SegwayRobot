#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <chrono>
#include <thread>
#include "./BBBIOlib/BBBio_lib/BBBiolib.h" // extern C already present
using namespace std;
//
// This class is used to drive 2 bidirectional DC motors.
// It has been tested with DRV8833 Motor Driver.
//
// N.B. Due to the limitations of the output current of the
//      battery, since the motors require a lot of current
//      to start rotating, we have introduced a (very) dirty
//      workaround. When both motors have to start to rotate,clarify, you don’t need resistors on the current sense pins, but they do need to be tied to ground
//      we apply a little delay between the two PWM signals
//      in  order to start the first motor and have enaugh
//      current to start the other one.
//

class MotorDriver {
	friend ostream& operator<<(ostream& os, const MotorDriver& myMD);
	private:
		float duty_1A, duty_1B, duty_2A, duty_2B;
		float freq1, freq2;
		float MaxSpeed;
		int CHANNEL1, CHANNEL2;
	public:
		MotorDriver();
		~MotorDriver();
		void SetChannels(int ch1, int ch2);
		void SetDuty(float d1a, float d1b, float  d2a, float d2b);
		void SetDuty1(float d1a, float d1b);
		void SetDuty2(float d2a, float d2b);
		void SetFrequency(float f1, float f2);
		void SetMaxSpeed(float s);
		void Start();
		void StartCh1();
		void StartCh2();
		void StartDelayed(unsigned delay); // delay in microseconds
		void Stop();
		void StopCh1();
		void StopCh2();
		void ApplySpeedPercentage(float speed);
		void ApplySpeedPercentage1(float speed);
		void ApplySpeedPercentage2(float speed);
		void ApplySpeedPercentageDelayed(float speed, unsigned delay);
		vector<float> getDuty();// { return duty; };

};