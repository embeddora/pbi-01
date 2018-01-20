#ifndef __TIMER_H__
#define __TIMER_H__

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
