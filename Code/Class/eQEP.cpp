#include "eQEP.hpp"

eQEP::eQEP()	// TODO
{
	channel = 0;
	position = 0;
	mode = 0;
	period = 0;
}

eQEP::eQEP(unsigned ch, unsigned m, float per)
{
	SetChannel(ch);
	SetPosition(0);
	SetMode(m);
	SetPeriod(per);
	Enable();
}

eQEP::~eQEP()
{
	Disable();
}

void eQEP::Enable()
{
	ofstream enabled_file( (path+"/enabled").c_str() );
	assert(enabled_file.good());
	//
	enabled_file << 1;
}

void eQEP::Disable()
{
	ofstream enabled_file( (path+"/enabled").c_str() );
	assert(enabled_file.good());
	//
	enabled_file << 0;
}

bool eQEP::IsEnabled() const
{
	ifstream enabled_file( (path+"/enabled").c_str() );
 	assert(enabled_file.good());
 	//
 	unsigned state;
 	enabled_file >> state;
 	if (state == 0)
 		return false;
 	else if (state == 1)
 		return true;
 	else
 		assert(0);
}

void eQEP::SetChannel(unsigned ch) {
	switch ( ch )
	{
		case 0:
			path = eQEP0;
			break;
		case 1:
			path = eQEP1;
			break;
		case 2:
			path = eQEP2;
			break;
		default:
			assert(0);
	}

	channel = ch;
}

void eQEP::SetMode(unsigned m)
{
	assert(m<=1);
	//
	ofstream mode_file( (path+"/mode").c_str() );
	assert(mode_file.good());
	//
	mode_file << m;
	mode = m;
}

void eQEP::SetPeriod(float per)
{
	//assert(per<=40000000000);
	assert(per <= 40);
	assert(per >= 0.000050); // Otherwise the BBB freezes
	//
	ofstream period_file( (path+"/period").c_str() );
	assert(period_file.good());

	int period_us = per * 1000000.0f;
	string period_us_string = to_string(period_us).append("000");
	//
	period_file << period_us_string << endl;
	//
	period = per;
}

void eQEP::SetPosition(int pos)
{
	//assert(per<=40000000000);
	ofstream position_file( (path+"/position").c_str() );
	assert(position_file.good());
	position_file << pos;
}

void eQEP::AcquireData() {
	//cout << "Bool: " << IsEnabled() << endl;
	if (not IsEnabled())
		Enable();
	//
 	ifstream position_file( (path+"/position").c_str() );
 	assert(position_file.good());
 	//
 	position_file >> position;
}

// void eQEP::AcquireData() {
// 	ifstream position_file(path+/"position");
// 	assert(position_file.good());
// 	old_position = position;
// 	position << position_file;
// 	speed = (position + old_position)*DELTAT/2;  // Trapezoid
// }
void eQEP::CheckState() const
{
	ifstream mode_file( (path+"/mode").c_str() );
	ifstream period_file( (path+"/period").c_str() );
 	assert(mode_file.good());
 	assert(period_file.good());
 	//
 	unsigned current_mode;
 	float current_period;
 	mode_file >> current_mode;
 	period_file	>> current_period;
 	//
 	assert( mode ==  current_mode);
 	//cout << (float) period*1000000 << "\t" << (float) current_period/1000.0 << endl;
 	//cout << ((float) period*1000000 - (float) current_period/1000.0) << endl;
 	assert( ((float) period*1000000 - (float) current_period/1000.0) < 0.1 );
}

ostream& operator<<(ostream& os, const eQEP& myeQEP) {
	myeQEP.CheckState();
	os << "Channel:\tEQEP" << myeQEP.channel << endl;
	string Mode;
	if (myeQEP.mode == 0)
		Mode = "Absolute";
	else if (myeQEP.mode == 1)
		Mode = "Relative";
	else
		assert(0);
	os << "Mode:\t\t" + Mode << endl;
	os << "Period:\t\t" << myeQEP.period << " [s]" << endl;
	return os;
}

