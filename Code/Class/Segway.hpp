#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include "IMU.hpp"
#include "ADC.hpp"
#include "Data3D.hpp"
#include "MotorDriver.hpp"
#include "eQEP.hpp"
#include "constants.hpp"
using namespace std;

class Segway
{
	friend ostream& operator<<(ostream& os, const Segway& mySegway);
	private:
		IMU myIMU;
		ADC myADC;
		MotorDriver myMD;
		eQEP eQEP1, eQEP2;
		pair<float,float> position, angular_position, angular_position_old;
		pair<float,float> speed, angular_speed;
	public:
		Segway(	// MotorDriver
			      unsigned PWMfreq,
		         pair<unsigned,unsigned> motor_channels,
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
		         string step_mode);
		~Segway();
		//
		void AcquireData(unsigned num_adc_samples);
		void AcquireADCData(unsigned num_adc_samples);
		//
		Data3D GetTheta() const { return myIMU.getTheta(); };
		float GetADCvalue(unsigned adc_channel);               // Return the buffer: TODO
		int GeteQEP1value() const { return eQEP1.GetPosition(); };
		int GeteQEP2value() const { return eQEP2.GetPosition(); };
		IMU GetIMU() const { return myIMU; };
		ADC GetADC() const { return myADC; };
		eQEP GeteQEP1() const {return eQEP1; };
		eQEP GeteQEP2() const {return eQEP2; };
		MotorDriver GetMotorDriver() const { return myMD; };
		//int GetPosition1() const { return eQEP1.GetPosition(); };
		//int GetPosition2() const { return eQEP2.GetPosition(); };
		Data3D getGyroComp() const { return myIMU.getGyroComp(); };
		pair<float,float> GetPosition() const { return position; };
		pair<float,float> GetSpeed() const { return speed; };
		pair<float,float> GetAngularPosition() const { return angular_position; };
		pair<float,float> GetAngularSpeed() const { return angular_speed; };
		//
		void SetSpeed(int speed);
		void SetSpeed1(int speed);
		void SetSpeed2(int speed);
		void Stop();
};