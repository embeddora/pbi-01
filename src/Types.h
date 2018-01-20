#ifndef __TYPES_H__
#define __TYPES_H__

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

#include <stdio.h>

#include <stdlib.h>

#include <string>

#include <vector>

#include <list>

#include <map>

#include <hash_map>

#include "Errors.h"


/* Pi */
#define NEW_PI			3.14159265358979323846f

/* Transformation:    */
#define NEW_DEGTORAD(a)		( (a) * ( NEW_PI / 180.0f ) )

/* Transformation:    */
#define	NEW_RADTODEG(x)		( (x) * ( 180.0f / NEW_PI ) )

/* Power 2 of munber 'x' */
#define NEW_SQR(x)		((x) * (x))

/* Bigger number selection */
#define NEW_MAX(a,b)		((a < b) ? (b) : (a))

/* Smaller number selection */
#define NEW_MIN(a,b)		((a < b) ? (a) : (b))

/* Epsilon for calculations */
const float	NEW_EPSILON	= 0.0001f;

/* Zero-degeneration of small number */
#define NEW_ZERO_CLAMP(x)	( (NEW_EPSILON > fabs(x))?0.0f:(x) )

/* Equality (or equity?) of float numbers */
#define NEW_FLOAT_EQ(x,v)	( ((v) - NEW_EPSILON) < (x) && (x) < ((v) + NEW_EPSILON) )		

/* Truncation of 'value' once it exceeds limits */
#define NEW_LIMIT_RANGE(low, value, high) { if (value < low) value = low; else if(value > high)	value = high; }

/* I can't explain what it does */
#define NEW_CUT_RANGE(low, value, high) { if (value < low) value = value - low + high;	else if(value >= high) value -= high; }

/* Amount of tics per second */
#define NEW_TIC_SECOND 10000


/* Native types definition */
typedef unsigned int UINT;
typedef unsigned char BYTE;
//+++typedef unsigned __int64 UINT64;
#define  __int64
typedef unsigned long UINT64;
typedef int BOOL;
typedef unsigned long DWORD;

#define FALSE 0
#define TRUE  1


/* Color structure (3 bytes) */
struct newColor3ub
{
	union {
		struct //++RGB
		{
			BYTE r,g,b;
		} RGBColor;
		BYTE Color[3];
	} ;
	newColor3ub()
	{
		RGBColor.r=0;
		RGBColor.g=0;
		RGBColor.b=0;
	}
	newColor3ub(BYTE r,BYTE g,BYTE b)
	{
		RGBColor.r=r;
		RGBColor.g=g;
		RGBColor.b=b;
	}
	BOOL operator ==(const newColor3ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return FALSE;
		if (RGBColor.g!=c.RGBColor.g) return FALSE;
		if (RGBColor.b!=c.RGBColor.b) return FALSE;
		return TRUE;

	}
	BOOL operator !=(const newColor3ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return TRUE;
		if (RGBColor.g!=c.RGBColor.g) return TRUE;
		if (RGBColor.b!=c.RGBColor.b) return TRUE;
		return FALSE;

	}
};

/* Color structure (4 bytes) */
struct newColor4ub
{
	union {
		struct //+++RGB
		{
			BYTE r,g,b,a;
		} RGBColor;
		BYTE Color[4];
	};
	newColor4ub()
	{
		RGBColor.r=0;
		RGBColor.g=0;
		RGBColor.b=0;
		RGBColor.a=255;
	}

	newColor4ub(BYTE r,BYTE g,BYTE b,BYTE a)
	{
		RGBColor.r=r;
		RGBColor.g=g;
		RGBColor.b=b;
		RGBColor.a=a;
	}
	BOOL operator ==(const newColor4ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return FALSE;
		if (RGBColor.g!=c.RGBColor.g) return FALSE;
		if (RGBColor.b!=c.RGBColor.b) return FALSE;
		if (RGBColor.a!=c.RGBColor.a) return FALSE;
		return TRUE;

	}
	BOOL operator !=(const newColor4ub c)
	{
		if (RGBColor.r!=c.RGBColor.r) return TRUE;
		if (RGBColor.g!=c.RGBColor.g) return TRUE;
		if (RGBColor.b!=c.RGBColor.b) return TRUE;
		if (RGBColor.a!=c.RGBColor.a) return TRUE;
		return FALSE;

	}
};

/* Own string type definition */
typedef std::string newString;

/* Amount of words in string. 'Space' characters divide a string into words. */
UINT newStrWordCount(const newString &src);

/* Search and out in 'dst' n-th word of string 'src' */
BOOL newGetStrWord(UINT n, const newString &src, newString &dst);

/* String-to-byte covertation */
BYTE newStrToBYTE(const newString &value);

/* String-to-integer covertation */
int newStrToInt(const newString &value);

/* String-to-unsigned-integer covertation */
UINT newStrToUINT(const newString &value);

/* String-to-float covertation */
float newStrToFloat(const newString &value);

/* Number-to-string covertation */
newString newIntToStr(int i);

/* The following are the operations with strings: */

newString &newStrMakeUp(newString &res,const newString &src);

void* memcpy(void* __dest, const void* __src, int __n);

void newStrMakeUp(newString &res);

int newStrStr(const newString &sub,const newString &main);

BOOL newReadStr(FILE *file,newString &res);

typedef std::list<newString> newStringList;

/* Output level which admits only very important messages */
#define NEW_SYSMESS 1

/* Output level which admits every message */
#define NEW_MAX_MESSAGE_LEVEL 10

/* Output level which allows many messages but ASE-debug stuff */
#define NEW_ROBUST_MESSAGE_LEVEL 5

/* Output level which allows ASE-debug stuff */
#define NEW_ASEDBG_MESSAGE_LEVEL 6


/* All used name'd constants */
enum newEnum {

	NEW_NONE,

	NEW_TEXTURE_BORDER_REPEAT,

	NEW_TEXTURE_BORDER_CLAMP,

	NEW_TEXTURE_COMBINE_REPLACE,

	NEW_TEXTURE_COMBINE_ADD,

	NEW_TEXTURE_COMBINE_MODULATE,

	NEW_TEXTURE_DIFFUSE,

	NEW_TEXTURE_FONT,

	NEW_RENDERMODE_3D,

	NEW_RENDERMODE_2D,
	NEW_RENDERMODE_PARTICLE,					
	NEW_RENDERMODE_BILLBOARD,

	NEW_RENDERMODE_RESTORE_LAST,
	NEW_VARIABLE_BYTE,
	NEW_VARIABLE_INT,
	NEW_VARIABLE_UINT,
	NEW_VARIABLE_FLOAT,
	NEW_VARIABLE_BOOL,
	NEW_VARIABLE_STRING,
	NEW_CONSOLE_COMMAND,
	NEW_CONSOLE_FULLSCREEN,
	NEW_CONSOLE_FADEOUT,
	NEW_CONSOLE_FADEIN,
	NEW_CONSOLE_NOTVISIBLE,
	NEW_CONSOLE_VISIBLE,

	NEW_OBJECTTYPE_NONE,
	NEW_OBJECTTYPE_PHYSICOBJECT,
	NEW_OBJECTTYPE_CAMERA,
	NEW_OBJECTTYPE_MESH,
	NEW_OBJECTTYPE_OBJECT3D,
	NEW_OBJECTTYPE_SCENE,


	/* Camera type 'from first person' */
	NEW_CAMERASTYLE_FIRST_PERSON,
	/* Camera type 'from third person' */
	NEW_CAMERASTYLE_THIRD_PERSON,
	/* Camera type 'for strategies and suches' */
	NEW_CAMERASTYLE_STRATEGY,
	/* Camera movemet forward */
	NEW_CAMERA_FORWARD,
	/* Camera movemet back */
	NEW_CAMERA_BACK,
	/* Camera movemet to left */
	NEW_CAMERA_LEFT,
	/* Camera movemet to right */
	NEW_CAMERA_RIGHT,
	/* Camera movemet to upside */
	NEW_CAMERA_UP,
	/* Camera movemet downstairs */
	NEW_CAMERA_DOWN,

	NEW_STORAGETYPE_DEFAULT,
	NEW_STORAGETYPE_FILE,
	NEW_STORAGETYPE_MEMORY,
	NEW_STORAGETYPE_PAKFILE,

	NEW_FILE_READ,
	NEW_FILE_WRITE,
	NEW_FILE_APPEND,

	NEW_OBJ3D_STATIC,
	NEW_OBJ3D_DYNAMIC,

	NEW_LIGHT_AUTODETECT,
	NEW_LIGHT_SIMPLE_OPENGL,
	NEW_LIGHT_LIGHTMAP_STATIC,
	NEW_LIGHT_PERPIXEL_STATIC,
	NEW_LIGHT_PERPIXEL_DYNAMIC,

	NEW_LIGHT_ATT_CONSTANT,
	NEW_LIGHT_ATT_LINEAR,
	NEW_LIGHT_ATT_QUADRATIC,
};

/* Base pbject */
class newBaseObject
{
public:
	newBaseObject(void) {Type=NEW_OBJECTTYPE_NONE;}
	/* Object type */
	newEnum Type;
	/* Name */
	newString Name;
};



typedef unsigned short __int16 ;//+++

/* Staff used for reading files prepared according to mike's file format */
#define NEW_SCENE_FILEID	"GSF"
#define NEW_MESH_FILEID		"GOF"
#define NEW_MATERIAL_FILEID	"GMF"
#define NEW_SCENE_VERSION		(__int16)0x0001 
#define NEW_MESH_VERSION		(__int16)0x0001 
#define NEW_MATERIAL_VERSION	(__int16)0x0001 

#define NEW_STATICBGCOLOR	(__int16)0x1001
#define NEW_STATICAMBIENT	(__int16)0x1002
#define NEW_PROP_MOTIONBLUR	(__int16)0x1003
#define NEW_PROP_CASTSHADOW	(__int16)0x1004
#define NEW_PROP_RECVSHADOW	(__int16)0x1005
#define NEW_NODE_NAME		(__int16)0x1006
#define NEW_NODE_PARENT		(__int16)0x1007
#define NEW_NODE_TM			(__int16)0x1008
#define NEW_SCENE_END		(__int16)0x1900

#define NEW_ID_NONE			(__int16)0x2000
#define NEW_ID_MESH			(__int16)0x2001
#define NEW_MESH_TVERTEX	(__int16)0x2002

#define NEW_MATERIAL_LIST	(__int16)0x3001
#define NEW_MATERIAL_REF	(__int16)0x3002
#define NEW_WIRECOLOR		(__int16)0x3003
#define NEW_MATERIAL		(__int16)0x3004
#define NEW_SUBMATERIAL		(__int16)0x3005
#define NEW_NUMSUBMTLS		(__int16)0x3006

#define NEW_AMBIENT			(__int16)0x4001
#define NEW_DIFFUSE			(__int16)0x4002
#define NEW_SPECULAR		(__int16)0x4003
#define NEW_SHINE			(__int16)0x4004
#define NEW_SHINE_STRENGTH	(__int16)0x4005
#define NEW_TRANSPARENCY	(__int16)0x4006
#define NEW_WIRESIZE		(__int16)0x4007

#define NEW_MAP_GENERIC			(__int16)0x5001
#define NEW_MAP_AMBIENT			(__int16)0x5002
#define NEW_MAP_DIFFUSE			(__int16)0x5003
#define NEW_MAP_SPECULAR		(__int16)0x5004
#define NEW_MAP_SHINE			(__int16)0x5005
#define NEW_MAP_SHINESTRENGTH	(__int16)0x5006
#define NEW_MAP_SELFILLUM		(__int16)0x5007
#define NEW_MAP_OPACITY			(__int16)0x5008
#define NEW_MAP_FILTERCOLOR		(__int16)0x5009
#define NEW_MAP_BUMP			(__int16)0x5010
#define NEW_MAP_REFLECT			(__int16)0x5011
#define NEW_MAP_REFRACT			(__int16)0x5012
#define NEW_ENVMAP				(__int16)0x5013

#define NEW_BITMAP				(__int16)0x5014


#define NEW_TWOSIDED		(__int16)0x5001

#endif /* ifndef-define include guard */
