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

#include "Timer.h"

#include "Console.h"

namespace std { using namespace __gnu_cxx; }


typedef union _LARGE_INTEGER {
  struct {
    unsigned long LowPart;
    unsigned long  HighPart;
  };
  struct {
    unsigned long LowPart;
    unsigned long  HighPart;
  } u;
  long long QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

void QueryPerformanceFrequency(
   LARGE_INTEGER *lpFrequency
)
{
	printf("[error] on cirrent platf. the '%s' yet to be defined on \n", __func__);
}

void QueryPerformanceCounter(
  LARGE_INTEGER *lpPerformanceCount
)
{
	printf("[error] on cirrent platf. the '%s' yet to be defined on \n", __func__);
}

newTimer::newTimer(void)
{

}

newTimer::~newTimer(void)
{
}

/* ������� ������ ������� */
UINT64 freq;


/* �������������� ��� �������� �� �������� */
void newInitTimeOperation()
{
LARGE_INTEGER s;

	/* �������� ������� */
	QueryPerformanceFrequency(&s);

	/* ��������� � */
	freq=s.QuadPart;

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������������� �������� ������ �������.");
}


/* ������� �������� ����������� �������� */
UINT64 newTime()
{
LARGE_INTEGER s;

	/* �������� ������� */
	QueryPerformanceCounter(&s);

	/* ���������� ������ �������� ����� */
	return s.QuadPart;
}
  
/* ���������� ����� � ����� */
UINT64 newGetTicksTime()
{
LARGE_INTEGER s;

	/* �������� ������� */
	QueryPerformanceCounter(&s);

	/* ������� ������� */
	return ((s.QuadPart)*10000/freq);
}

/* �������� ������� ����� */
void newTimer::newStartTiming()
{
LARGE_INTEGER s;

	/* �������� ������� */
	QueryPerformanceCounter(&s);

	/* ���������� ����� */
	StartTime=s.QuadPart;
}

/* ���������� ��������� ����� */
UINT64 newTimer::newTimeElapsed()
{
	ElapsedTime=((newTime()-StartTime)*10000/freq);

	return ElapsedTime;
}

/* ����������� ���� � ������� */
double newConvertTicksToSeconds(UINT64 ticks)
{
	return (double)(signed int)(ticks/freq);
}
