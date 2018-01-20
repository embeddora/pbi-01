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

class newTimer
{

public:

	newTimer(void);

	~newTimer(void);

	UINT64 StartTime;

	UINT64 ElapsedTime;

	void newStartTiming();

	UINT64 newTimeElapsed();
};

UINT64 newTime();

UINT64 newGetTicksTime();

double newConvertTicksToSeconds(UINT64 ticks);

void newInitTimeOperation();

#endif /* ifndef-define include guard */
