#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#define DEF_FILTER		10
#define DEF_TWO			2
#define DEF_DEF_NULL	0
#define DEF_MINUS_ONE	-1

#pragma once

#include "Types.h"

#include "Manager.h"

/* ����� �������� */
class newTexture
{
public:
	newTexture(UINT filter = DEF_FILTER,  UINT generate_mipmap = DEF_TWO)
	{
		Filter		=	DEF_FILTER;

		Width		=	DEF_DEF_NULL;

		Height		=	DEF_DEF_NULL;

		TextureID	=	DEF_MINUS_ONE;

		Bpp			=	DEF_DEF_NULL;

		Border_Type	=	NEW_TEXTURE_BORDER_REPEAT;

		Type		=	NEW_TEXTURE_DIFFUSE;

		isBRGA		=	FALSE;

		isLoaded	=	FALSE;

		GenerateMipMap = generate_mipmap;
	}

	~newTexture(void);

    /* ������� �������� */
	std::vector<BYTE> Data;

	/* ������ ���������� �������� */
	BYTE Filter;

	/* ��� �� ������� */
	BYTE Bpp;

	/* ��� �������� */
	newString Name;

	/* ������ �������� */
	UINT Width;

	/* ������ �������� */
	UINT Height;

	/* ������ �������� � ����� ������ */
	UINT TextureID;

	/* ��� ������ */
	newEnum Border_Type;

	/* ��� �������� */
	newEnum Type;

	/* ������ ������������������ ������� */
	BOOL isBRGA;

	/* �������� ���������? */
	BOOL isLoaded;

	/* ������������ �������: (0 - ���, 1 - ��, �� � ������, 2 - ��, �������������� ���������) */
	UINT GenerateMipMap;	

	/* ������ ��������, ����������� ������ � 3 ���������� */
	void CreateEmpty(int width,int height, newColor3ub &c);

	/* ������ ��������, ����������� ������ � 4 ���������� */
	void CreateEmpty(int width,int height, newColor4ub &c);

	/* ��������� �� ����� ������ � ����������� �� ���������� */
	newResult LoadFromFile(const newString &FileName);

	/* ��������� TGA ���� */
	newResult LoadTGAFile(const newString &FileName);

	/* ��������� JPG ���� */
	newResult LoadJPGFile(const newString &FileName);

	/* ��������� ����� ����� �� ����� */
	newResult LoadAlphaFromFile(const newString &FileName);

	/* �������� �������� */
	void Enable();

	/* ��������� �������� � �����������. */
	newResult Upload();
};

/* �������� ������� */
extern newManager<newTexture> TextureMan;

/* ����������� ��������� ����� */
typedef newManager<newTexture>::Items Textures;

/* ������ �� �������� ��������� ������� */
extern Textures &Texture;

/* ������ ������ � ���� JPG; ������������ ��� �������� ScreenShot'�� */
BOOL newSaveScreenToJPG();


#endif /* ifndef-define include guard */
