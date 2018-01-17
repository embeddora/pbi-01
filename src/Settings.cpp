#include "Settings.h"

#include "Console.h"

#include "File.h"

#include "Render.h"

/* Глобальная переменная */
newSettings MainSettings;

newSettings::newSettings(void)
{
	FOV = 0;

	/* Рисовать fps? */
	MainRender.ISfps= 0;

	/* Ширина экрана */
	SCREEN_WIDTH= 640;

	/* Высота экрана */
	SCREEN_HEIGHT= 480;

	/* Бит на пиксель */
	BPP= 32;

	/* Битов на глубину */
	DEPTH= 24;

	/* Полноэкран? */
	FULLSCREEN= 0;

	/* Высота экрана */
	SCREEN_frequency= 120;

	/* Ближняя плоскость отсечения */
	ZNEAR = 1 ;

	/* Дальняя плоскость отсечения */
	ZFAR = 10000;

	/* Заголовок окна */
	TITLE= "Engine: Empty window";

	/* Картинка для консоли */
	CONSOLE.TEXTURE_FILE= "";

	/* Цвет консоли */
    CONSOLE.COLOR.RGBColor.a= 200 ;

	/* Шрифт консоли */
	CONSOLE.FONT_FILE= "";

	/* Положение по Y консоли */
	CONSOLE.YPOS= 480;

	/* Скорость движения консоли */
	CONSOLE.SPEED= 400;

	/* Скорость движения камеры */
	Camera_Speed= 500;

	/* Отрисовывать нормали */
	Render.DrawNormals= 0;

	/* Отрисовывать помощь */
	Render.DrawHelp= 0;
}

newSettings::~newSettings(void)
{
}

/* Загрузка настроек из файла */
void newSettings::LoadFromFile(const newString &Filename)
{
	MainConsole.Add(0,"Загрузка конфиг-файла: %s",Filename.c_str());

	newString cmd="";

	/* Открываем файл; FILE *file=fopen(Filename.c_str(),"r"); */
	newFile sfile;

	newResult res;

	if ( (res = sfile.Open(Filename, NEW_FILE_READ, FALSE)) != NEW_OK)
		
		return;
	/* Читаем и посылаем как команду в консоль */
	while (sfile.ReadTextString(cmd))
	{
		if (!(cmd.substr(0,2)=="//"))

			MainConsole.ExecCMD(cmd);
	}

	MainConsole.Add(0,"Конфиг-файл %s загружен.",Filename.c_str());
}
