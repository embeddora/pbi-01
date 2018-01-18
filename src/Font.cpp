#include "Font.h"

#include "Render.h"

/* Глобальный массив шрифтов */
Fonts Fontchen;

/* Создание нового шрифта, возращает индекс */
newFont *newNewFont(const newString &Name)
{
	/* Увеличиваем массив, добавляем в конец */
	Fontchen[Name]=newFont();

	/* Возращаем индекс последнего элемента */
	return &Fontchen[Name];
}

newFont *newDeleteFont(const newString &Name)
{
	Fontchen.erase(Name);

	return NULL;
}

/* Диструктор пустой */
newFont::~newFont(void)
{
}

/* Загружаем текстуру и заполняем таблицу растояний между буквами */
BOOL newFont::LoadTextureAndCheckSpace(const newString &FileName,const newString &FontName,BYTE Zero)
{
	/* Проверяем, текстура может уже быть загружена в массив */
	if (Texture.find(FileName)==Texture.end())
	{
		/* Иначе делаем новую текстуру в массиве */
		TextureLink=TextureMan.newNewItem();

		/* Устанавливаем флаг текстуры шрифта */
		TextureLink->Type=NEW_TEXTURE_FONT;

		/* Если текстура не грузится из файла, то выходим */
		if (TextureLink->LoadFromFile(FileName)!=NEW_OK)
			
			return FALSE;

		/* Закачиваем текстуру в видео-память */
		MainRender.TextureUpload(TextureLink);
	}

	/* Ссылка на текстуру */
	newTexture *tex=TextureLink;

	/* Ссылка на текстель в текстуре */
	/* newColor3ub *col=(newColor3ub *)&tex->Data[0]; */

	BYTE *col=&tex->Data[0];

	BYTE SpaceWidth=0;

	/* Цикл по всем буквам, код символа равен x+y*16 */
	for (int y=0;y<16;y++)

		for (int x=0;x<16;x++)
		{
			/* Le Принцип:
			1.	Вычисляем квадрат 16х16, в котором должна быть буква.
			2.	Ищем первую вертикальную линию содержащую точку выше заданного порога, начиная слева и потом справа.
			3.	Получаем FTrim - таблицу растояний между буквами. */
			if ((x+16*y)==32)
			{
				FTrim[x+y*16][0]=0;

				FTrim[x+y*16][1]=16;

				continue;
			}

			int cx,cy;

			cy=y*16;

			cx=x*16-1;

			BOOL space=TRUE;

			while (space)
			{
				cx++;

				for (int t=0;t<16;t++)

					if (col[(cx+(cy+t)*256)*tex->Bpp/8]>Zero)
					{
						space=FALSE;

						break;
					}
					
			}
			FTrim[x+y*16][0]=cx-x*16;

			cy=y*16;

			cx=(x+1)*16;

			space=TRUE;

			while (space)
			{
				cx--;

				for (int t=0;t<16;t++)

					if (col[(cx+(cy+t)*256)*tex->Bpp/8]>Zero)
					{
						space=FALSE;

						break;
					}
				
			}
			FTrim[x+y*16][1]=cx+1-FTrim[x+y*16][0]-x*16;

			if (FTrim[x+y*16][1]<16)

				SpaceWidth=NEW_MAX(FTrim[x+y*16][1],SpaceWidth);
		}

		FTrim[32][1]=2*SpaceWidth/3;
	
	/* Возращаемся успешно */
	return TRUE;
}

/* Функция выбора шрифта по имени */
BOOL newSelectFont(const newString &Name)
{
	if (Fontchen.find(Name)!=Fontchen.end()) 
	{
		MainRender.CurFont=&Fontchen[Name];

		return TRUE;
	}
	return FALSE;
}

BOOL newLoadFontAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero)
{
	if (Fontchen.find(FontName)==Fontchen.end())
	{
		MainRender.CurFont=newNewFont(FontName);

		if (!MainRender.CurFont->LoadTextureAndCheckSpace(FileName,FontName,Zero))
		{
			MainRender.CurFont=newDeleteFont(FontName);

			return FALSE;
		}
		else return TRUE;
	}
	return FALSE;
}
