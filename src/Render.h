#ifndef __RENDER_H__
#define __RENDER_H__

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

#include "Settings.h"

#include "Material.h"

#include "Font.h"

#include "Timer.h"

#include "Camera.h"

#include "RenderObject.h"

#include "Light.h"



struct newDisplayList
{
	UINT Index;

	void Begin();

	std::list<newRenderObject *> objlist;

	void End();

	void Compile();

	void Draw();

	newMaterial *mtl;

	UINT TriCount;
};

/* Класс основного рендера */
class newRender
{
private:

	std::list<newDisplayList> DList;

public:

	newRender(void);

	~newRender(void);

	/* Инициализация OpenGL */
	BOOL SetupOpenGL(GL_Window *window);

	/* Деинициализация OpenGL */
	BOOL DestroyWindowGL (GL_Window* window);

	/* Коррекция окна */
	void Reshape(int width, int height);

	/* Востановление, после смены видео режима */
	void RestoreAll();

	newString VendorName;

	newString RendererName;

	newString VersionName;

	newString Extensions;

	/* Количество отрисованых треугольников в кадре */
	UINT Tri_per_frame;

	/* Количество треугольников в секунду */
	UINT Tri_per_second;

	/*  Накопитель треугольников в секунду */
	UINT Tri_accum;

	/* Номер текущего кадра */
	UINT64 Frame_Num;

	/* Кадров в секунду */
	UINT fps;

	/* Таймер для fps */
    newTimer FPSTimer;

	/* Считать fps? */
	BOOL ISfps;

	/* Время между обновлениями fps */
	UINT64 FPSTime;

	/* Последний кадр обновления fps */
	UINT64 LastFPSFrame;

	/* Начать считать fps, промежуток в секундах */
	void StartCalculateFPS(float Time);

	/* Обновить fps, время в тиках */
	void UpdateFPS(UINT64 &DeltaTime);

	/* Ссылка на текущую текстуру */
	UINT CurTexID;

	/* Закачиваем в видео карточку данные по текстуре */
	void TextureUpload(newTexture *tex);

	/* Включаем текстуру по ссылке на текстуру */
	void TextureEnable(newTexture *tex);

	/* Выключаем текстуру */
	void TextureDisable();

	/* Операция для смешивания текстуры */
	void TextureOperation(newEnum op);

	/* Обновление текстуры */
	void TextureUpdate(newTexture *tex);

	/* Режим отрисовки (3D, 2D для текста, бильборды) */
	newEnum CurrentRenderMode;

	void RenderMode(newEnum mode);

	/* Операции со шрифтами */

	/* Текущий шрифт */
	newFont *CurFont;

	/* Ширина экрана для шрифта */
	UINT FontDisplayWidth;

	/* Высота экрана для шрифта */
	UINT FontDisplayHeight;

	/* Настройка двухмерного режима */
	void TextMode2D(UINT Width, UINT Height)
	{
		FontDisplayWidth=Width;

		FontDisplayHeight=Height;

		ReshapeConsole();
	}

	/* Создание шрифта */
	BOOL CreateFont(newFont *font);

	/* Уничтожение шрифта */
	void KillFont(newFont *font);

	/* Создание консоли */
	void CreateConsole();

	/* Отрисовка консоли */
	void DrawConsole();

	/* Дисплей лист для консоли */
	UINT ConsoleDS;
	
	/* Перезагрузить консоль */
	void ReshapeConsole();

	/* Обновить консоль */
	void UpdateConsole(UINT64 DeltaTime);

	/* Текущая камера */
	newCamera *CurrentCamera;

	/* Применить камеру */
	void ApplyCamera();

	void MakeList(newRenderObject &ro);

	void CallList(newRenderObject &ro);

	void SetColor(newVector3f Color);

	void SetColor(newVector4f Color);

	void DumpDraw(newRenderObject &ro);

	newDisplayList *GetNewDL();

	void DeleteDL(newDisplayList *dl);

	UINT GetDisplayListCount();

	void CompileDispalyLists();

	void RenderDisplayLists();

	void InitLights();

	newLight * CreateLight(newEnum ltype);

	void DeleteLight(newLight *light);

	void SetMaterial(newMaterial *mtl);
};

// ATTENTION: Оставил место для поддержки разных API. Если сделать основ
// ной класс с виртуальными функциями , и наследовать с переопределением,
// то возможно сделать разные рендеры.
class newGLRender :	public newRender
{
public:

	newGLRender(void);

	~newGLRender(void);
};

/* Глобальная переменная */
extern newRender MainRender;

/* Функция сброса экрана в буффер */
void newGetScreen(BYTE *Data);

/* Вывод текста */
void newPrint2D(float x,float y,const char *Text, ...);

#endif /* ifndef-define include guard */
