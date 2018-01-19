#ifndef __FONT_H__
#define __FONT_H__

#include "Types.h"

#include "Texture.h"

// Класс шрифта
class newFont
{
public:
	// Конструктор
	newFont(void)
	{
		Width=
		Height=1;
		Spacing=6;
		Color=newColor4ub(255,255,255,255);
		Spacing=0;
	}
	~newFont(void);
	// Название шрифта
	newString Name;
	// Ширина
	float Width;
	// Высота
	float Height;
	// Текстура
	newTexture *TextureLink;
	// Прозрачность
	newEnum BlendFunc;
	// Базовая
	UINT Base;
	// Растояние между буквами
	UINT Spacing;
	// Растояния до реальных букв
	BYTE FTrim[256][2];
	// Цвет
	newColor4ub Color;
	// Загрузка теустуры и нахождение пробелов между буквами
	// Zero - это пороговое значение. Ниже это ноль, выше есть пиксель.
	BOOL LoadTextureAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);
};

// Глобальный массив шрифтов
typedef std::map<newString,newFont> Fonts;

extern Fonts Fontchen;

// Новый шрифт
newFont *newNewFont(const newString &Name);
newFont *newDeleteFont(const newString &Name);
// Выбор шрифта
BOOL newSelectFont(const newString &Name);

BOOL newLoadFontAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);


#endif /* ifndef-define include guard */
