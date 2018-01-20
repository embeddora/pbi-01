#ifndef __ANGLES_H__
#define __ANGLES_H__

/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract:  
 */

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