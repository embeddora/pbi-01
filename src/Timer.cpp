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

/* Частота биения таймера */
UINT64 freq;


/* Инициализируем все операции со временем */
void newInitTimeOperation()
{
LARGE_INTEGER s;

	/* получаем частоту */
	QueryPerformanceFrequency(&s);

	/* сохраняем её */
	freq=s.QuadPart;

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Инициализация тайминга прошла успешно.");
}


/* Текущее значение глобального счетчика */
UINT64 newTime()
{
LARGE_INTEGER s;

	/* Получаем счетчик */
	QueryPerformanceCounter(&s);

	/* Возвращаем только четвёртую часть */
	return s.QuadPart;
}
  
/* Возвращаем время в тиках */
UINT64 newGetTicksTime()
{
LARGE_INTEGER s;

	/* Получаем счетчик */
	QueryPerformanceCounter(&s);

	/* Простая формула */
	return ((s.QuadPart)*10000/freq);
}

/* Засекаем текущее время */
void newTimer::newStartTiming()
{
LARGE_INTEGER s;

	/* Получаем счетчик */
	QueryPerformanceCounter(&s);

	/* Запоминаем время */
	StartTime=s.QuadPart;
}

/* Возвращает прошедшее время */
UINT64 newTimer::newTimeElapsed()
{
	ElapsedTime=((newTime()-StartTime)*10000/freq);

	return ElapsedTime;
}

/* Преобразуем тики в секунды */
double newConvertTicksToSeconds(UINT64 ticks)
{
	return (double)(signed int)(ticks/freq);
}
