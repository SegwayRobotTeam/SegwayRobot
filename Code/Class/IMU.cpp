#include "IMU.hpp"

IMU::IMU()
{
	A << 1, DELTAT, 0, 1;  // A: system state matrix
	B << DELTAT,0;         // B: input vector
	Ctr << 1, 0;           // C: output vector
	Q << Q11, 0, 0, Q22;   // Q: covariance matrix
	Px = Q;                // Pz: error covariace for z
	Pz = Q;                // Py: error covariace for y
	P << 0, 0, 0, 0;       // P: temporary error covariance
	I << 1, 0, 0, 1;       // I: 2x2 identity matrix
	Xhat << 0, 0;          // State vector of estimate for Z data
	Zhat << 0, 0;          // State vector of estimate for Y data
	S << 0,0;              // Temporary xdot state vector
	Shat << 0,0;           // Temporary xhat state vector
	CleanBuffers();        // Clean the I2C buffers
}

void IMU::CleanBuffers()
{
	memset(rxBuf, 0, sizeof(rxBuf));  // Cleans both the buffers
   memset(txBuf, 0, sizeof(txBuf));  // used for I2C communication
}

void IMU::I2C_init()
{  //TODO if it's not on i2c-1?
	// Set up the node
	if ( (i2cnode = open("/dev/i2c-1", O_RDWR)) < 0)
		assert(0);
	//
	// Check the response of the Accelerometer
	//
	CleanBuffers();                                  // Clean the buffers rxBuf and txBuf
	ioctl(i2cnode, I2C_SLAVE, LSM303DLHC_ADDRESS);   // Set the /dev/i2c-1 node to send to the right slave
   txBuf[0] = LSM303DLHC_WHO_AM_I;						 // Set the WHO_AM_I register
   write(i2cnode, txBuf, 1);                        // Send the message
   read(i2cnode, rxBuf, 1);                         // Read the response
   assert(int(rxBuf[0]) == 73);                     // Compare to the constant value
   //
	// Check the response of the Gyroscope
	//
	CleanBuffers();                                  // Clean the buffers rxBuf and txBuf
	ioctl(i2cnode, I2C_SLAVE, L3GD20H_ADDRESS);      // Set the /dev/i2c-1 node to send to the right slave
   txBuf[0] = L3GD20H_WHO_AM_I;						    // Set the WHO_AM_I register
   write(i2cnode, txBuf, 1);                        // Send the message
   read(i2cnode, rxBuf, 1);                         // Read the response
   assert(int(rxBuf[0]) == 215);                    // Compare to the constant value
}

void IMU::LSM303DLHC_A_init() // Accelerometer
{
	CleanBuffers();
	//
	ioctl(i2cnode, I2C_SLAVE, LSM303DLHC_ADDRESS);
	//
   txBuf[0] = LSM303DLHC_CTRL1;
   txBuf[1] = 0b10100111;        // 1600Hz, Continuous update, All axes enabled
   write(i2cnode, txBuf, 2);
   //
   txBuf[0] = LSM303DLHC_CTRL2;
   txBuf[1] = 0b00000000;        // 773Hz AA filter, +-2g, 
   write(i2cnode, txBuf, 2);
}

void IMU::LSM303DLHC_MT_init() // Magnetometer+Temperature
{
	CleanBuffers();
	//
   ioctl(i2cnode, I2C_SLAVE, LSM303DLHC_ADDRESS);
   //
   txBuf[0] = LSM303DLHC_CTRL5;
   txBuf[1] = 0b11110100;        // Temperature enabled, Magnetometer HighRes, 100Hz
   write(i2cnode, txBuf, 2);
   //
   txBuf[0] = LSM303DLHC_CTRL6;
   txBuf[1] = 0b00000000;        // +-2 gauss
   write(i2cnode, txBuf, 2);
}

Data3D IMU::LSM303DLHC_A_read()
{
	Data3D accel_data;
	CleanBuffers();
	//
   ioctl(i2cnode, I2C_SLAVE, LSM303DLHC_ADDRESS);
   //
   txBuf[0] = LSM303DLHC_OUT_X_L_A | (1 << 7);
   write(i2cnode, txBuf, 1);
   read(i2cnode, rxBuf, 6);
   double x = int16_t( ((rxBuf[1] << 8 | rxBuf[0])) )*SENSITIVITY_ACC_2G; //
   double y = int16_t( ((rxBuf[3] << 8 | rxBuf[2])) )*SENSITIVITY_ACC_2G; // Decomplement + conversion to mg (FS = +-2g)
   double z = int16_t( ((rxBuf[5] << 8 | rxBuf[4])) )*SENSITIVITY_ACC_2G; //
   //
   accel_data.set(x,y,z);
   return accel_data;
}

void IMU::L3GD20H_init() // Gyroscope
{
	CleanBuffers();
	//
	ioctl(i2cnode, I2C_SLAVE, L3GD20H_ADDRESS);
	//
   txBuf[0] = L3GD20H_CTRL1;
   txBuf[1] = 0b11001111;        // 800Hz, 30Hz cut off, All axes enabled -> LOW_ODR must be 0
   write(i2cnode, txBuf, 2);
   //
   txBuf[0] = L3GD20H_CTRL2;
   txBuf[1] = 0b00000111;        // 0.5Hz HP Filter
   write(i2cnode, txBuf, 2);
   //
   //txBuf[0] = L3GD20H_CTRL4;
   //txBuf[1] = 0b00000000;        // 245dps  (already default)
   //write(i2cnode, txBuf, 2);
   //
   txBuf[0] = L3GD20H_CTRL5;
   txBuf[1] = 0b00010000;        // Enable HP filter
   write(i2cnode, txBuf, 2);
}

Data3D IMU::L3GD20H_read()
{
	Data3D gyro_data;
   ioctl(i2cnode, I2C_SLAVE, L3GD20H_ADDRESS);
   //
	CleanBuffers();
	//
   txBuf[0] = L3GD20H_OUT_X_L | (1 << 7);
   //
   write(i2cnode, txBuf, 1);
   read(i2cnode, rxBuf, 6);
   double x = int16_t( ((rxBuf[1] << 8 | rxBuf[0])) )*SENSITIVITY_GYRO_245DPS/1000; //
   double y = int16_t( ((rxBuf[3] << 8 | rxBuf[2])) )*SENSITIVITY_GYRO_245DPS/1000; // Decomplement + conversion to dps (FS = +-245dps)
   double z = int16_t( ((rxBuf[5] << 8 | rxBuf[4])) )*SENSITIVITY_GYRO_245DPS/1000; //
   // 
   gyro_data.set(x,y,z);
   return gyro_data;
}

void IMU::Init()
{
	CleanBuffers();
	I2C_init();             // I2C
	LSM303DLHC_A_init();    // Accelerometer
   //LSM303DLHC_M_init();  // Magnetometer
   L3GD20H_init();         // Gyroscope
   //LPS331AP_init();      // Barometer + Temperature (not implemented)
}

void IMU::getNewValues()
{
	accel = LSM303DLHC_A_read();      // Read the Accelerometer
	gyro = L3GD20H_read();            // Read the Gyroscope
	theta_acc = ThetaFromAcc(accel);  // Compute the angle from the accelerometer values
	//
	// Integral filter:
	//
	//theta = theta + ((gyro_old+gyro)/20); 
	//
	// Complementary filter:
	//  
	//theta = (theta + (gyro_old+gyro)*DELTAT/2)*A_COMPL + theta_acc*(1-A_COMPL);
	//
	// Kalman filter:
	//
	theta = EstimateAngle();
}

Data3D IMU::ThetaFromAcc(const Data3D& data_acc)
{
	Data3D theta_acc_data;
	//
	double zdata = -atan2(data_acc.getX(), sqrt(pow(data_acc.getZ(),2.0)+pow(data_acc.getY(),2.0)));
	double xdata = atan2(data_acc.getZ(), sqrt(pow(data_acc.getX(),2.0)+pow(data_acc.getY(),2.0)));
	//
	theta_acc_data.set(xdata, 0, zdata);
	return theta_acc_data*RAD2DEG;
}

Vector2d IMU::KalmanFilter1D(double gyro_entry, double theta_acc_entry, Matrix2d Pold, Vector2d ShatOld)
{
	P = Pold;                                    // Old error covariance
	S = A*ShatOld + B*gyro_entry;                // Project the state ahead
	double y = theta_acc_entry;
	double inn = y - Ctr.transpose()*S;          // Calculate the error of the estimated state compared to the accelerometer data
	double covInn = Ctr.transpose()*P*Ctr + R;
	P = A*P*A.transpose() + Q;                   // Project the error covariance ahead
	K = A*P*Ctr/covInn;                          // Compute the Kalman gain
	Shat = S + K*inn;                            // Update estimate with measurement
	P = (I-K*Ctr.transpose())*P;                 // Update error covariance
	return Shat;
}

Data3D IMU::EstimateAngle()
{
	Data3D theta_est, drift_est;
	//
	Xhat = this->KalmanFilter1D(gyro.getX(), theta_acc.getX(),Px, Xhat);
	Zhat = this->KalmanFilter1D(gyro.getZ(), theta_acc.getZ(),Pz, Zhat);
	
	//
	theta_est.set(Xhat(0,0), 0, Zhat(0,0));
	drift_est.set(Xhat(1,0), 0, Zhat(1,0));
	//
	theta = theta_est;
	drift = drift_est;
	return theta_est;
}

ostream& operator<<(ostream& os, const IMU& myIMU) {
	cout << "---------------------------------------------------------------------------------------------------\n";
	cout << "gyro"      << "\t\t[dps]\t"    << myIMU.gyro;
	cout << "accel"     << "\t\t[mg]\t"     << myIMU.accel;
	cout << "theta_acc" << "\t[deg]\t"      << myIMU.theta_acc;
	cout << "theta"     << "\t\t[deg]\t"    << myIMU.theta;
	cout << "drift"     << "\t\t[deg]\t"    << myIMU.drift;
	return os;
}