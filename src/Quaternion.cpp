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



#include "Quaternion.h"



// Сферико-линейная интерполяция
newQuaternionf Slerp(const newQuaternionf &src, const newQuaternionf &dst, float factor)
{
    double fAngle = acos(src.X * dst.X + src.Y * dst.Y + src.Z * dst.Z + src.W * dst.W);
    if ( NEW_FLOAT_EQ(fAngle,0))
        return src;
	if ( NEW_FLOAT_EQ( fAngle - NEW_PI ,0)) return src;
		NEW_LIMIT_RANGE(0,factor,1);

    double fInvSin = 1.0f/sin(fAngle);
    double fCoeff0 = sin((1.0f-factor)*fAngle)*fInvSin;
    double fCoeff1 = sin(factor*fAngle)*fInvSin;
	return newQuaternionf(
		float(fCoeff0 * src.X + fCoeff1 * dst.X),
		float(fCoeff0 * src.Y + fCoeff1 * dst.Y),
		float(fCoeff0 * src.Z + fCoeff1 * dst.Z),
		float(fCoeff0 * src.W + fCoeff1 * dst.W));
}

// Применение кватерниона к вектору
void newApplyQuaterion(newVector3f &v, const newQuaternionf &q)
{
	newQuaternionf r ( v.X * q.W + v.Z * q.Y - v.Y * q.Z, 			
							v.Y * q.W + v.X * q.Z - v.Z * q.X, 			
							v.Z * q.W + v.Y * q.X - v.X * q.Y,
							v.X * q.X + v.Y * q.Y + v.Z * q.Z 
						  );
	v.X = q.W * r.X + q.X * r.W + q.Y * r.Z - q.Z * r.Y;
	v.Y = q.W * r.Y + q.Y * r.W + q.Z * r.X - q.X * r.Z;
	v.Z = q.W * r.Z + q.Z * r.W + q.X * r.Y - q.Y * r.X;

}
