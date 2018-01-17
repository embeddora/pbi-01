#include "Frustum.h"

/* Конструктор фрустума */
newFrustum::newFrustum(void)
{
	/* Угол обзора 120 градусов */
	FoV=120;

	/* Ближняя плоскость */
	zNear=1;

	/* Дальняя плоскость */
	zFar=100;

	/* Изменился? */
	Reshape=TRUE;
}

newFrustum::~newFrustum(void)
{
}
