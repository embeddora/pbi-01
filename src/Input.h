#ifndef __INPUT_H__
#define __INPUT_H__

#include "Types.h"

#include "Math.h"

/* Структура клавиатуры */
typedef struct
{
	/* Хранит TRUE / FALSE для каждой клавиши */
	BOOL keyDown [256];

} Keys;

/* Класс управлением ввода */
class newInput
{
public:

	newInput(void);

	~newInput(void);

	/* Вкл./Выкл. */
	BOOL Enabled;

	/* Мышь подвинули */
	BOOL MouseMoved;

	/* Структура клавиш */
	Keys  keys;		

	/* Кнопки мыши */
	BOOL MouseButton[3];

	/* Положение мыши */
	newVector2f MousePos;

	/* Координаты центра экрана для мыши */
	newVector2f MouseCenter;

	/* Колесо мыши */
	newVector2f MouseWheel;

	/* Инициализация */
	void Init();

	/* Обработать ввод */
	void ProcessInput();
};

/* Глобальная переменная */
extern newInput MainInput;

#endif /* ifndef-define include guard */
