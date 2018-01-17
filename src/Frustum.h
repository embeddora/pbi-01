#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

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
