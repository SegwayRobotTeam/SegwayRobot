#include "Data3D.hpp"

Data3D::Data3D() {}

Data3D::Data3D(double xdata, double ydata, double zdata)
{
	axis << xdata, ydata, zdata;
}

void Data3D::set(double xdata, double ydata, double zdata)
{
	axis << xdata, ydata, zdata;
}

Data3D Data3D::operator*(double mult) const
{
	Data3D res;
	res.set(this->getX()*mult, this->getY()*mult, this->getZ()*mult);
	return res;
}

Data3D Data3D::operator/(double mult) const
{
	Data3D res;
	res.set(this->getX()/mult, this->getY()/mult, this->getZ()/mult);
	return res;
}


Data3D Data3D::operator+(const Data3D& data) const
{
	Data3D res;
	res.set(this->getX()+data.getX(), this->getY()+data.getY(), this->getZ()+data.getZ());
	return res;
}

Data3D Data3D::operator-(const Data3D& data) const
{
	Data3D res;
	res.set(this->getX()-data.getX(), this->getY()-data.getY(), this->getZ()-data.getZ());
	return res;
}

Data3D& Data3D::operator=(const Data3D& data)
{
	axis = data.axis;
	return *this;
}

Data3D& Data3D::operator=(const Vector3d& dataaxis)
{
	axis = dataaxis;
	return *this;
}

void Data3D::operator*=(double mult)
{
	*this=(*this)*mult;
}

ostream& operator<<(ostream& os, const Data3D& data)
{
	os.precision(6); // TODO
	os << fixed;
	os << "x: " << data.getX() << ",\t\t\ty: " << data.getY() << ",\t\t\tz: " << data.getZ() << "\n";
	return os;
}