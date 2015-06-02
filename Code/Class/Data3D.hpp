#pragma once

#include <iostream>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;

//
// This class is only a Vector3d with some handy functions and naming
// It is useful to easy visualize the x,y,z axis of a 3D space
//
// NB: not all operators are overloaded
//

class Data3D {
	friend ostream& operator<<(ostream& os, const Data3D& data);
	private:
		Vector3d axis;
	public:
		// Constructors
		Data3D();
		Data3D(double xdata, double ydata, double zdata);
		Data3D(Vector3d data);
		//void set(int x, int y, int z);
		// Get
		double getX() const { return axis(0,0); };
		double getY() const { return axis(1,0); };
		double getZ() const { return axis(2,0); };
		// Set
		void set(double x, double y, double z);
		void set(Vector3d data);
		// Operators
		Data3D& operator=(const Data3D& data);
		Data3D& operator=(const Vector3d& data);
		Data3D operator+(const Data3D& data) const;
		Data3D operator-(const Data3D& data) const;
		Data3D operator*(double mult) const;
		Data3D operator/(double mult) const;
		void operator*=(double mult);
};