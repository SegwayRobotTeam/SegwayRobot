#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "eQEP.hpp"
#include "MotorDriver.hpp"
#include <fstream>
#include <string>
using namespace std;

int main()	
{
	unsigned device = 1; // eQEP1
	unsigned mode = 0;   // Absolute mode
	float period = 30;   // sec

	eQEP myeQEP(device, mode, period);
 	cout << myeQEP;

 	//
 	// Set up the motor driver to
 	// mimic the encoder input
 	//
 	MotorDriver myMD;
	myMD.SetChannels(1,2);
	myMD.SetFrequency(1000,1000);
	myMD.SetDuty(50,50,50,50);
	myMD.Start();

	for (;;)
	{
		myeQEP.AcquireData();
		cout << "Position: " << myeQEP.GetPosition() << endl;
		this_thread::sleep_for(chrono::microseconds((int)(0.1*1000000))); // 0.1sec
	}


 	//


	/*string path("/sys/devices/ocp.3/48302000.epwmss/48302180.eqep");
	ifstream position_file( (path+"/position").c_str() );
 	assert(position_file.good());*/
 	//char position = position_file.get();
 	//cout << position;
 	
 	/*float num;
 	position_file >> num;
 	cout << num;*/



 	/*cout << "Period: " << period << endl;
	eQEP myeQEP(device, mode, period);
	myeQEP.SetPosition(10);
	myeQEP.SetPeriod(0.1);
	//cout << "New Period: " << myeQEP.GetPeriod();
	myeQEP.AcquireData();
	//cout << myeQEP.GetPosition() << endl;
	//cout << myeQEP.GetMode() << endl;
	//cout << myeQEP.GetPeriod() << endl;
	cout << "Old Mode: " << myeQEP.GetMode() << endl;
	myeQEP.SetMode(1);
	cout << "New Mode: " << myeQEP.GetMode() << endl;*/



	//
	/*MotorDriver myMD;
	myMD.SetChannels(1,2);
	myMD.SetFrequency(1000,1000);
	myMD.SetDuty(50,50,50,50);
	myMD.Start();*/
	//
	//cout << myeQEP.GetPeriod();
	/*for(;;)
	{
		myeQEP.AcquireData();
		cout << "Position: " << myeQEP.GetPosition() << endl;
		this_thread::sleep_for(chrono::microseconds((int)(0.1*1000000))); // 0.1sec
	}*/
}