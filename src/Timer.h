#ifndef __TIMER_H__
#define __TIMER_H__

#include "Types.h"

/* ����� ������� */
class newTimer
{

public:

	newTimer(void);

	~newTimer(void);

	/* ��� �������� ������� */
	UINT64 StartTime;

	/* ��������� ����� */
	UINT64 ElapsedTime;

	/* ��������� ����� */
	void newStartTiming();

	/* ������� ������� */
	UINT64 newTimeElapsed();
};

/* ������� �������� �������� */
UINT64 newTime();

/* ���������� � ����� */
UINT64 newGetTicksTime();

/* ������������ � �������� */
double newConvertTicksToSeconds(UINT64 ticks);

/* �������������*/
void newInitTimeOperation();

#endif /* ifndef-define include guard */
