#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

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

#include "Types.h"

/* класс Фрустума */
class newFrustum
{
protected:

	/* Угол обзора */
	float FoV;

	/* Ближняя плоскость отсечения */
	float zNear;

	/* Дальняя плоскость отсечения */
	float zFar;

public:

	newFrustum(void);

	~newFrustum(void);

	/* Флаг изменения, если TRUE, изменения вступят в силу */
	BOOL Reshape;

	/* Возвращает угол обзора */
	float GetFoV() {return FoV;}

	/* Возвращает ближнюю плоскость */
	float GetzNear() {return zNear;}

	/* Возвращает дальнюю плоскость */
	float GetzFar() {return zFar;}

	/* Устанавливает угол обзора */
	void SetFoV(float fov) {FoV=fov;Reshape=TRUE;}

	/* Устанавливает ближнюю плоскость */
	void SetzNear(float znear) {zNear=znear;Reshape=TRUE;}

	/* Устанавливает дальнюю плоскость */
	void SetzFar(float zfar) {zFar=zfar;Reshape=TRUE;}
};

#endif /* ifndef-define include guard */
