#ifndef __FONT_H__
#define __FONT_H__

#include "Types.h"

#include "Texture.h"

// ����� ������
class newFont
{
public:
	// �����������
	newFont(void)
	{
		Width=
		Height=1;
		Spacing=6;
		Color=newColor4ub(255,255,255,255);
		Spacing=0;
	}
	~newFont(void);
	// �������� ������
	newString Name;
	// ������
	float Width;
	// ������
	float Height;
	// ��������
	newTexture *TextureLink;
	// ������������
	newEnum BlendFunc;
	// �������
	UINT Base;
	// ��������� ����� �������
	UINT Spacing;
	// ��������� �� �������� ����
	BYTE FTrim[256][2];
	// ����
	newColor4ub Color;
	// �������� �������� � ���������� �������� ����� �������
	// Zero - ��� ��������� ��������. ���� ��� ����, ���� ���� �������.
	BOOL LoadTextureAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);
};

// ���������� ������ �������
typedef std::map<newString,newFont> Fonts;

extern Fonts Fontchen;

// ����� �����
newFont *newNewFont(const newString &Name);
newFont *newDeleteFont(const newString &Name);
// ����� ������
BOOL newSelectFont(const newString &Name);

BOOL newLoadFontAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);


#endif /* ifndef-define include guard */
