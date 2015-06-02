#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
using namespace std;

//
// N.B. the /ocp.*/ could change!
//
//const unsigned DELTAT = 0.1; // TODO: extern?
const string eQEP0 = "/sys/devices/ocp.3/48300000.epwmss/48300180.eqep";
const string eQEP1 = "/sys/devices/ocp.3/48302000.epwmss/48302180.eqep";
const string eQEP2 = "/sys/devices/ocp.3/48304000.epwmss/48304180.eqep";

class eQEP
{
	friend ostream& operator<<(ostream& os, const eQEP& myeQEP);
	private:
		string path;
		int position;
		unsigned channel;
		unsigned mode;
		float period;   // sec
	public:
		eQEP();
		eQEP(unsigned channel, unsigned mode, float period);
		~eQEP();
		void Enable();
		void Disable();
		void AcquireData();
		void SetPosition(int position);
		void SetPeriod(float period);
		void SetMode(unsigned mode);
		void SetChannel(unsigned ch);
		void CheckState() const;
		bool IsEnabled() const;
		int GetPosition() const { return position; };
		float GetPeriod() const { return period; };
		unsigned GetMode() const { return mode; };
		unsigned GetChannel() const { return channel; };
	
};