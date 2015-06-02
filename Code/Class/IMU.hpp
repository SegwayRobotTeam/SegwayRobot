#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <cassert>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <inttypes.h>
#include "Data3D.hpp"
#include "altimu.hpp"  // Sensors registers mapping
#include "constants.hpp"

using namespace std;
using namespace Eigen;
//
// Sensors constants:
//
const int FS_GYRO = 245;       // Full Scale Gyroscope
const int FS_ACC  = 2;         // Full Scale Accelerometer
const int COMP = 150;          // Compensation: multiplication factor of sigma_acc 
const double SENSITIVITY_ACC_2G = -0.061;    //   m g/LSB
const double SENSITIVITY_GYRO_245DPS = 8.75; // m dps/LSB
const double NONLIN_GYRO = 0.5/100; // 1Sigma Gyroscope      0.5%
const double NONLIN_ACC  = 0.2/100; // 1Sigma Accelerometer  0.2%
//
// Kalman Filter constants:
//
const double DELTA_GYRO = NONLIN_GYRO*FS_GYRO;     // Worst case uncertainty
const double DELTA_ACC  = NONLIN_ACC*FS_ACC*COMP;  // Worst case uncertainty
const double R = pow(DELTA_ACC,2);
const double Q11 = pow(DELTA_GYRO,2)*DELTAT;
const double Q22 = 0.01;
//
// Complementary Filter constants (see getNewValues()):
//
const float TAU = 0.1; // Seconds
const float A_COMPL = TAU / (TAU + DELTAT);

//               x: right
// Orientation:  y: down
//               z: in

class IMU {
	friend ostream& operator<<(ostream& os, const IMU& myIMU);
	private:
		int i2cnode;
		char txBuf[10];
		char rxBuf[10];
		Data3D gyro, accel;	// Input Data
		Data3D theta_acc;    // Calculated Data
		Data3D theta, drift; // Estimated Data
		//
		// Kalman filter
		//
		Matrix2d A, Px, Pz, Q, I;
		Vector2d B, Ctr;
		Vector2d Xhat, Zhat;
		//
		// Temporary variables for Kalman
		//
		Matrix2d P;
		Vector2d S, Shat, y;
		Vector2d K;
	public:
		IMU();                                         // Constructor: Set the internal values to zero initial condition
		void Init();											  // Start and set I2C, Accelerometer, gyroscope
		void getNewValues();                           // Fill gyro, accel, theta_acc, theta, drift from sensors + estimation
		Data3D ThetaFromAcc(const Data3D& data_acc);   // Calculate euler angles from the projection into the axis of g vector 
		Vector2d KalmanFilter1D(double gyro_entry, double accel_entry, Matrix2d Pold, Vector2d ShatOld); // Estimate [theta, drift] with sensor fusion
		Data3D EstimateAngle();                        // Call KalmanFilter1D for both the possible axis
		void CleanBuffers();                           // Clean the I2C buffers
		Data3D getTheta()    const { return theta;      };
		Data3D getGyro()     const { return gyro;       };
		Data3D getAccel()    const { return accel;      };
		Data3D getThetaAcc() const { return theta_acc;  };
		Data3D getGyroComp() const { return gyro-drift; };
		//
		void I2C_init();             // Initialization of the I2C node
		void LSM303DLHC_A_init();    // Initialization of the Accelerometer
		void LSM303DLHC_MT_init();   // Initialization of the Magnetometer (not used nor tested)
		void L3GD20H_init();         // Initialization of the Gyroscope
		Data3D LSM303DLHC_A_read();  // Read the acceleration
		Data3D L3GD20H_read();       // Read the angular speed
};