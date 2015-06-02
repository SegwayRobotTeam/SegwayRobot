#include "PID.hpp"

PID::PID()
{
	Kp = 0;
	Ki = 0;
	Kd = 0;
	T = 0;
	integral = 0;
	sat.first  = 0;
	sat.second = 0;
	out = 0;
}

PID::PID(float p, float i, float d, float t, pair<float,float> s)
{
	SetParameters(p,i,d,t);
	integral = 0;
	sat = s;
	out = 0;
}

void PID::SetParameters(float p, float i, float d, float t)
{
	Kp = p;
	Ki = i;
	Kd = d;
	T = t;
}

float PID::CalculateNewValue(float in)
{
	assert(T != 0);
	integral = (integral + in)*T/2;
	out = Kp*in + Kd*(in-out)/T + Ki*(integral);
	if ( abs(out) < sat.first || abs(out) > sat.second )
		return copysign(sat.first,out);
	else
		return out;
}