#include "Font.h"

#include "Render.h"

/* ���������� ������ ������� */
Fonts Fontchen;

/* �������� ������ ������, ��������� ������ */
newFont *newNewFont(const newString &Name)
{
	/* ����������� ������, ��������� � ����� */
	Fontchen[Name]=newFont();

	/* ��������� ������ ���������� �������� */
	return &Fontchen[Name];
}

newFont *newDeleteFont(const newString &Name)
{
	Fontchen.erase(Name);

	return NULL;
}

/* ���������� ������ */
newFont::~newFont(void)
{
}

/* ��������� �������� � ��������� ������� ��������� ����� ������� */
BOOL newFont::LoadTextureAndCheckSpace(const newString &FileName,const newString &FontName,BYTE Zero)
{
	/* ���������, �������� ����� ��� ���� ��������� � ������ */
	if (Texture.find(FileName)==Texture.end())
	{
		/* ����� ������ ����� �������� � ������� */
		TextureLink=TextureMan.newNewItem();

		/* ������������� ���� �������� ������ */
		TextureLink->Type=NEW_TEXTURE_FONT;

		/* ���� �������� �� �������� �� �����, �� ������� */
		if (TextureLink->LoadFromFile(FileName)!=NEW_OK)
			
			return FALSE;

		/* ���������� �������� � �����-������ */
		MainRender.TextureUpload(TextureLink);
	}

	/* ������ �� �������� */
	newTexture *tex=TextureLink;

	/* ������ �� �������� � �������� */
	/* newColor3ub *col=(newColor3ub *)&tex->Data[0]; */

	BYTE *col=&tex->Data[0];

	BYTE SpaceWidth=0;

	/* ���� �� ���� ������, ��� ������� ����� x+y*16 */
	for (int y=0;y<16;y++)

		for (int x=0;x<16;x++)
		{
			/* Le �������:
			1.	��������� ������� 16�16, � ������� ������ ���� �����.
			2.	���� ������ ������������ ����� ���������� ����� ���� ��������� ������, ������� ����� � ����� ������.
			3.	�������� FTrim - ������� ��������� ����� �������. */
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
	
	/* ����������� ������� */
	return TRUE;
}

/* ������� ������ ������ �� ����� */
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
