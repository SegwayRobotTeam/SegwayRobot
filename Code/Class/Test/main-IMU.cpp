#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "../IMU.hpp"
using namespace std;


int main()
{

	//
	// IMU Test Driver
	//
	IMU myIMU;
	myIMU.Init();
	uint delay = DELTAT*1000000;
	for (;;) {
		myIMU.getNewValues();
		cout << myIMU;
		//cout << myIMU.getTheta();
		this_thread::sleep_for(chrono::microseconds(delay));
	}
}