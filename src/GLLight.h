#ifndef __GLLIGHT_H__
#define __GLLIGHT_H__

#include "Light.h"

class newGLLight: public newLight
{
	/* Индекс света в массиве OpenGL */
	UINT Index;

	/* Тип затухания: Констаннтный, Линейный, Квадратичный */
	UINT Att_type;

public:

	/* Конструктор */
	newGLLight(void);

	/* Диструктор */
	~newGLLight(void);

	/* Включаем свет */
	void Enable();

	/* Выключаем свет */
	void Disable();

	/* Применить */
	void Apply();

	/* Установка затухания */
	void SetAttenuation(newEnum att_type, float att);
};

#endif /* ifndef-define include guard */
