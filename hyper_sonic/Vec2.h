#pragma once
#include <iostream>

struct Vec2 {
public:
	double x;
	double y;

	// Math operations 
	Vec2 operator*(double scalar) const;
	Vec2 operator/(double scalar) const;

	Vec2 operator+(const Vec2& other) const;

	double getLength() const;
	Vec2 normalize() const;
	double crossProduct(const Vec2& other) const;
	
};

// Print overload
std::ostream& operator<<(std::ostream& os, const Vec2& vec);