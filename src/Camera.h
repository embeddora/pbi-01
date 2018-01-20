#ifndef __CAMERA_H__
#define __CAMERA_H__

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

#include "Types.h" //newtypes

#include "Math.h" //newmath

#include "Physic.h"

#include "Frustum.h"

// Класс камеры
class newCamera: public newBaseObject
{
public:
	newCamera(void);

	~newCamera(void);

	/* Имя камеры */
	newString Name;

	/* Стиль камеры */
	newEnum CamStyle;

	/* Функция установки стиля камеры */
	void SetCameraStyle(newEnum style);

	/* Функция возвращающая стиль камеры !Сейчас не работает! */
	/* newEnum GetCameraStyle() */

	/* Фрустум */
	newFrustum Frustum;

	/* Ссылка на физический объект */
	newPhysicObject *Pobj;

	/* Присоеденить физический объект к другому физическому объекту по ссылке */
	void AttachTo(newPhysicObject *obj) {Pobj=obj;}

	/* Присоеденить физический объект к другому физическому объекту по имени */
	void AttachTo(const newString &PobjName) {Pobj=&PhysicObject[PobjName];}

	/* Скорость, которая прибавится в текущей обработке */
	newVector3f DeltaSpeed;

	/* Двигаться. Входные параметры: Направление и Скорость */
	void Move(newEnum dir,float speed);

	/* Применить скорость к объекту */
	void ApplyTransform(UINT64 &DeltaTime);
};

/* Тип карты камер */
typedef std::map<newString, newCamera> Cameras;

/* Глобальный массив камер */
extern Cameras Camera;

/* Новая камера */
newCamera *newNewCamera(const newString &Name);

/* Генерируем имя камеры */
newString newGenCameraName();

#endif /* ifndef-define include guard */
