#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "MotorDriver.hpp"
using namespace std;

int main()	
{
	MotorDriver myMD;
	myMD.SetChannels(1,2);
	myMD.SetFrequency(1000, 1000);
	int speedpercent = 100;
	/*for (int i=0; i<6; i++) {
		myMD.ApplySpeedPercentage(speedpercent);
		cout << myMD;
		speedpercent -= 5;
		this_thread::sleep_for(chrono::microseconds(2*1000000)); // 2sec
	}*/
	myMD.ApplySpeedPercentage(speedpercent);
	unsigned delay = 10*1000000;
	this_thread::sleep_for(chrono::microseconds(delay));
	myMD.Stop();
	//myMD.ApplySpeedPercentage(speedpercent); // 100 microsec
	/*myMD.SetDuty(50,50,50,50);
	//cout << myMD;
	myMD.Start();
	this_thread::sleep_for(chrono::microseconds(10*1000000)); // 2sec
	myMD.StopCh2();*/
}