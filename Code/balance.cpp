#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <Eigen/Dense>
#include "Class/Segway.hpp"
#include "Class/PID.hpp"
using namespace std;

int main()
{
	//********************************************************************************
	// eQEP parameters
	//********************************************************************************
	pair<unsigned,unsigned> eqep_channels(0,1);  // eQEP0, eQEP1
	unsigned eqep_period = 30;                   // seconds
	unsigned eqep_mode = 0;                      // absolute

	//********************************************************************************
	// Motor Driver parameters
	//********************************************************************************
	unsigned PWMfreq = 60000;                    // 60kHz
	pair<unsigned,unsigned> motor_channels(1,2); // EHRPWM1, EHRPWM2

	//********************************************************************************
	// ADC Parameters
	//********************************************************************************
	unsigned clk_div     = 20;            // To evaluate these parameters
	unsigned open_dly    = 0;             // see:
	unsigned sample_dly  = 1;             // Class/BBBIOlib/Toolkit/Toolkit_ADC_CALC/
	unsigned average     = 16;            // Used: 80kHz with 16 samples average -> 5kHz
	unsigned buffer_size = 1;             // Since we acquire a single value (averaged)
	string step_mode("SW_CONTINUOUS");
	unsigned adc_ch1 = 1;
	unsigned adc_ch2 = 2;
	unsigned chvec[] = {adc_ch1,adc_ch2}; // Enabled: AIN1 and AIN2
	vector<unsigned> adc_channels(chvec, chvec + sizeof(chvec) / sizeof(unsigned) );

	//********************************************************************************
	// Class Constructor
	//********************************************************************************
	Segway mySegway( // MotorDriver
	                 PWMfreq,
	                 motor_channels,
	                 // eQEP
	                 eqep_channels,
	                 eqep_period,
	                 eqep_mode,
	                 // ADC
	                 clk_div,
	                 open_dly,
	                 sample_dly,
	                 average,
	                 buffer_size,
	                 adc_channels,
	                 step_mode);

	// TODO: start the DCDC

	//
	// Sleep variable
	//
	int delay = DELTAT*1000000;               // State space loop
	int delay_cur = DELTAT_CURRENT*1000000;   // Motors current loop
	//
	// State estimate values
	//
	float theta=0;             // Angle to maintein equal to 0
	float omega=0;             // Angular speed 
	float position=0;          // Position to mainten equal to 0
	float speed=0;             // Linear speed
	float torque_ref;          // LQR output
	//
	Vector4d state_vector(theta, omega, position, speed);	// Initial conditions  
	//
	// LQR gain
	//
	Vector4d K(-21.9707, -2.1908, -3.1623, -2.9556); // theta, theta_dot, x, x_dot
	//
	// Motor controller values
	//
	pair<float,float> angular_position;
	pair<float,float> angular_speed;
	pair<float,float> current;
	pair<float,float> position_pair;
	pair<float,float> speed_pair;
	float voltage1, voltage2;      // In percent with respect to duty=100%
	float current_ref;

	//
	// PID controller
	//
	float Kp = 0;
	float Ki = 0;
	float Kd = 0;
	float sat_max = 100;
	float sat_min = -100;
	pair<float,float> sat(sat_min, sat_max);
	//
	PID PIDcurrent1(Kp,Ki,Kd,DELTAT, sat);
	PID PIDcurrent2(Kp,Ki,Kd,DELTAT, sat);
	//
	float error1, error2;
	float i1, i2;

	//
	// TODO: remove once the loop is ready
	//
	mySegway.SetSpeed(-100);

	//
	// Number of samples 
	//
	unsigned Nsamples = 300;

	//********************************************************************************
	// CONTROL LOOP
	//********************************************************************************
	chrono::duration<double, micro> elapsed_fast;
	chrono::duration<double, milli> elapsed_slow;
	//for (;;)
	for (unsigned i=0; i<Nsamples; i++)
	{
		//
		// Start the counter to debug the slower cicle time
		//
		auto start_slow = std::chrono::high_resolution_clock::now();

		//********************//
		//  Acquire new Data  //
		//********************//
		mySegway.AcquireData(1);
		//
		// Save the new state variables
		//
		// N.B. X: horizontal axis (We control this angle)
		//      Y: vertical axis
		//
		theta = mySegway.GetTheta().getX();
		omega = mySegway.getGyroComp().getX();
		//
		position_pair = mySegway.GetPosition();                      // Acquire the 2 channels position
		position = (position_pair.first + position_pair.second)/2;   // Save the average of the 2 (turn not implemented)
		//
		speed_pair = mySegway.GetPosition();                         // Same for speed
		speed = (speed_pair.first + speed_pair.second)/2;

		//
		// Populate the new state vector
		//
		state_vector << omega, theta, speed, position;

		//
		// Save the new values to control the motor
		// TODO remove this part and place inside the faster loop
		//
		//current.first  = -1 * ( mySegway.GetADCvalue(1)/2 - 0.612 ) / 0.185;
		current.first  = -1 * ( mySegway.GetADCvalue(adc_ch1)/CURRENT_COND_GAIN - CURRENT_COND_BIAS1 ) / CURRENT_SENSOR_SENS;
		current.second = -1 * ( mySegway.GetADCvalue(adc_ch2)/CURRENT_COND_GAIN - CURRENT_COND_BIAS1 ) / CURRENT_SENSOR_SENS;
		angular_position = mySegway.GetAngularPosition();  //
		angular_speed = mySegway.GetAngularSpeed();        // TODO: useless since we control the torque (current)

		//
		// Print all the values (only for debugging)
		//
		cout << "Theta:\t " << theta << endl;
		cout << "Omega:\t"  << omega << endl;
		cout << "<AIN1,AIN2>:\t\t<\t"  << mySegway.GetADCvalue(adc_ch1) << "\t\t"
		                               << mySegway.GetADCvalue(adc_ch2) << "\t>" << endl;
		cout << "<I1,I2>:\t\t<\t"      << current.first           << "\t"
		                               << current.second          << "\t>" << endl;
		cout << "<Ang Position>:\t\t<\t" << angular_position.first << "\t"
		                                 << angular_position.second << "\t>" << endl;
		cout << "<Ang Speed>:\t\t<\t"    << angular_speed.first    << "\t\t"
		                                 << angular_speed.second    << "\t>" << endl;
		cout << "<Position>:\t\t<\t"   << position_pair.first << "\t\t"
		                               << position_pair.second << ">" << endl;
		cout << "<Speed>:\t\t<\t"      << speed_pair.first    << "\t\t"
		                               << speed_pair.second    << "\t>" << endl;

		//
		// Apply the LQR gain
		//
   	torque_ref = state_vector.transpose()*K;
   	current_ref = KE_PHI * torque_ref;              // TODO Saturate current?
   	//cout << "Current_ref: " << current_ref << endl;
   	

   	//
   	// Motor controller
   	//
		/*for (unsigned j=0; j<0.9*(DELTAT/DELTAT_CURRENT); j++)
		{
			//
			// Start the counter to debug the slower cicle time
			//
			auto start_fast = chrono::high_resolution_clock::now();

			mySegway.AcquireADCData(1);
			//
			// Calculate the error
			//
			i1 = mySegway.GetADCvalue(adc_ch1);
			i2 = mySegway.GetADCvalue(adc_ch2);
			//
			error1 = current_ref - i1;
			error2 = current_ref - i2;

			//
   		// PID motors
   		//
   		voltage1 = PIDcurrent1.CalculateNewValue(error1);
   		voltage2 = PIDcurrent2.CalculateNewValue(error2);
   		//
   		mySegway.SetSpeed1(voltage1);
   		mySegway.SetSpeed2(voltage2);

			this_thread::sleep_for(chrono::microseconds((unsigned)delay_cur));
			auto end_fast = chrono::high_resolution_clock::now();
			elapsed_fast = end_fast - start_fast;
			cout << "Waited: " << elapsed_fast.count() << " us" << endl;

		}*/

		//
		// Sleep for a sample time
		//
		this_thread::sleep_for(chrono::microseconds((unsigned)delay));
		auto end_slow = chrono::high_resolution_clock::now();
		elapsed_slow = end_slow - start_slow;
		cout << "Waited: " << elapsed_slow.count() << " ms" << endl;
		cout << endl;
	}
	mySegway.Stop();
}