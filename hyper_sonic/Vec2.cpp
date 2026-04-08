#include "Vec2.h"
#include <cmath>


// Constructor
Vec2::Vec2() : x(0.0), y(0.0) {}
Vec2::Vec2(double xVal, double yVal) : x(xVal), y(yVal){}

// Math operations: 

Vec2 Vec2::operator*(double scalar) const {
	return { x * scalar, y * scalar };
}

Vec2 Vec2::operator/(double scalar) const {
	return { x / scalar, y / scalar };
}

Vec2 Vec2::operator+(const Vec2& other) const {
	return { x + other.x, y + other.y };
}

double Vec2::getLength() const {
	double length = std::sqrt(x * x + y * y);
	return length;
}

Vec2 Vec2::normalize() const {
	double length = getLength();

	// safety check to avoid division by zero
	if (length <= 1e-12) {
		return { 0.0, 0.0 };
	}

	return { x / length, y / length };
}

double Vec2::crossProduct(const Vec2& other) const {
	return x * other.y - y * other.x;
}

// Print overload
std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
 }