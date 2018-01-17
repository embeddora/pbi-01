#ifndef __CAMERA_H__
#define __CAMERA_H__

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
