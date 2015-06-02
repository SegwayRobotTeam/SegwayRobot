#pragma once

#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

class PID
{
	private:
		float Kp, Ki, Kd;
		float integral;
		float T;
		float out;
		pair<float,float> sat;
	public:
		PID();
		PID(float p, float i, float d, float t, pair<float,float> s);
		//~PID();
		void SetParameters(float p, float i, float d, float t);
		float CalculateNewValue(float in);
};