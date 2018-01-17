#ifndef __TIMER_H__
#define __TIMER_H__

#include "Types.h"

/* Класс таймера */
class newTimer
{

public:

	newTimer(void);

	~newTimer(void);

	/* Для хранения времени */
	UINT64 StartTime;

	/* Прошедшее время */
	UINT64 ElapsedTime;

	/* Сохраняем время */
	void newStartTiming();

	/* Считаем разницу */
	UINT64 newTimeElapsed();
};

/* Текущее значения счетчика */
UINT64 newTime();

/* Возвращаем в тиках */
UINT64 newGetTicksTime();

/* Конвертируем в секундах */
double newConvertTicksToSeconds(UINT64 ticks);

/* Инициализация*/
void newInitTimeOperation();

#endif /* ifndef-define include guard */
