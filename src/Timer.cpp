#include "Timer.h"

#include "Console.h"

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
