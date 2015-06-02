#pragma once

const double PI = 3.1415926;
const double RAD2DEG = 180/PI;
const double DEG2RAD = PI/180;
//
const double F1 = 200;
const double F2 = 2000;
const double DELTAT = 1/F1;          // 200Hz
const double DELTAT_CURRENT = 1/F2;  // 2kHz
//
const double CURRENT_COND_BIAS1 = 0.612;  // Bias of first  channel conditioning circuit
const double CURRENT_COND_BIAS2 = 0.612;  // Bias of second channel conditioning circuit
const double CURRENT_COND_GAIN = 2;       // Gain of conditioning circuit
const double CURRENT_SENSOR_SENS = 0.185; // Current sensors sensitivity (V/A)
const double KE_PHI = 0.0057363;          // EMF motor constant
//
const float DIAMETER = 0.07;	     // 7cm  Wheel diameter
const float RADIUS = DIAMETER/2;
const float GEAR_RATIO = 34;       // 1:34 Gear train ratio 
const float ENCODERCOUNTS = 12;    // Total number periods per 360°
const float NEDGE = 2;             // Number of edges measured in the period by the encoder;