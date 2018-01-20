#ifndef __ASEFILE_H__
#define __ASEFILE_H__

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

#define LEXEMA	0x20

typedef struct 
{
	int	i3dsMaxAsciiExport;

	char	chAsciiExp[LEXEMA];

	char	chVersion[LEXEMA];

	int	iVer;

	int	iSubVer;

	char	chWeekDay[LEXEMA];

	char	chMonth[LEXEMA];

	int	iDate;

	int	iHours;

	int	iMinutes;

	int	iSeconds;

	int	iYear;

} AseFileType, *pAseFileType;

typedef struct
{
	char	chFileName[LEXEMA];

	int	iFirstFrame;

	int	iLastFrame;

	int	iFrameSpeed;

	int	iTicksPerFrame;

	float	cfBackgroundStatic[3];

	float	cfAmbientStatic[3];	

} AseSceneType, *pAseSceneType;


typedef struct
{
	char	chNameStr[LEXEMA];

	int	iNameIdx;

	char	chClass[LEXEMA];

	int	iSubNo;

	float	fMapAmount;

	char	chBitmap[LEXEMA];

	char	chMapType[LEXEMA];

	float	fUvwOffetU;

	float	fUvwOffetV;

	float	fUvwTilingU;

	float	fUvwTilingV;

	float	fUvwAngle;

	float	fUvwBlur;

	float	fUvwBlurOffset;

	float	fUvwNouseAmt;

	float	fUvwNoiseSize;

	int	iUvwNoiseLevel;

	float	fUvwNoisePhase;

	char	chBitmapFilter[LEXEMA];

} MapDissfuseType, pMapDissfuseType;

typedef struct
{
	int	iNameIdx;

	char	chNameStr[LEXEMA];

	char	chClass[LEXEMA];

	float	cfAmbient[3];

	float	cfDiffuse[3];

	float 	cfSpecular[3];

	float	fShine;

	float	fShineStrength;

	float	fTransperency;

	float	fWireSize;

	char	chShading[LEXEMA];

	float	fXpFallOff;

	float	fSelfIllum;

	char	chFallOff[LEXEMA];

	char	chXpType[LEXEMA];

	pMapDissfuseType pMapDissfuse;

} MaterialType, pMaterialType;

typedef struct 
{
	int 	iCount;

	pMaterialType	Material;

} MaterialListType, pMaterialListType;

typedef struct
{
	char 	chNodeNameAgain[LEXEMA];

	int	ciInheritPos[3];

	int	ciInheritRot[3];

	int	ciInheritScl[3];

	float	cfRow0[3];

	float	cfRow1[3];

	float	cfRow2[3];

	float	cfRow3[3];

	float	cfPos[3];

	float	cfRotAxis[3];

	float	fRotAngle;

	float	cfScale[3];

	float	cfScaleAxis[3];

	float	fScaleAxisAng;


} NodeTmType,  pNodeTmType;

typedef struct 
{
	int	iIdx;

	float	cfCords[3];

} VertexType, pVertexType, TvertexType, pTvertexType;

typedef struct
{
	int	iIdx;

	int	iA;

	int	iB;

	int	iC;

	int	iAB;

	int	iBC;

	int	iCA;

	int	iSmoothing;

	int	iMtLid;

} FaceType, pFaceType;

typedef struct
{
	int	iIdx;

	int	iFirst;

	int	iSecond;

	int	iThird;

} TfaceType, pTfaceType;


typedef struct
{
	int	iTimeValue;

	int	iNumVertex;

	int	iNumFaces;

	pVertexType	pVertex;

	pFaceType	pFace;

	int	iNumTvertex;

	pTvertexType	pTvertex;

	int	iNumTfaces;

	pTfaceType	pTface;

} MeshType, pMeshType;

typedef struct
{
	char 	chNodeName[LEXEMA];

	pNodeTmType	pNodeTm;

	pMeshType	pMesh;

	int	iPropMotionBlur;

	int	iPropCastShadow;

	int	iProcpRecvShadow;

	int	iMaterialRef;

	/* In most cases we won't see it in scene editor, in ASE model - too */
	float	cfWireFrameColor[3];

} GeomObjectType, pGeomObjectType;

#endif /* ifndef-define include guard */
