#ifndef __ANGLES_H__
#define __ANGLES_H__

#include "Vector.h"

/* Structure type to represent Euler angles */
struct newEulerAngle
{
	/* X, Y, Z corresondingly */
	float Pitch, Yaw, Roll;
};

/* Structure type to represent axis angle */
struct newAxisAngle

{	/* Angle */
	float Angle;

	/* Axis */
	newVector3f Axis;

	/* Standard constructor */
	inline newAxisAngle(): Angle(0), Axis(0,0,1)  {};

	/* Constructor for angle and vector */
	inline newAxisAngle(float angle, newVector3f &axis): Axis(axis), Angle(angle){};

	/* Constructor for angle and three vector coordinates */
	inline newAxisAngle(float angle, float axis_x, float axis_y, float axis_z): Axis(axis_x,axis_y,axis_z), Angle(angle) {};
};

#endif /* ifndef-define include guard */
