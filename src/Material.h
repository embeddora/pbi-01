#ifndef __MATERIAL_H__
#define __MATERIAL_H__

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

#include "Texture.h"

#include "Math.h"

class newMaterial
{
public:

	newMaterial(void)
		;
	newMaterial( const newString &_Name,const newString &_DiffuseMap);

	~newMaterial(void);

	newString Name;

	void memcopy(void*, void*, int);

	newString BaseShader;

	UINT Ref;

	newVector3f Ambient;

	newVector3f Diffuse;

	newVector3f Specular;

	float Shine;

	float ShineStrange;

	float Transparency;
    
	newTexture *GenericMap;

	newTexture *AmbientMap;

	newTexture *DiffuseMap;

	newTexture *SpecularMap;

	newTexture *ShineMap;

	newTexture *ShineStrengthMap;

	newTexture *SelfIllumMap;

	newTexture *OpacityMap;

	newTexture *FilterColorMap;

	newTexture *BumpMap;

	newTexture *ReflectMap;

	newTexture *RefractMap;

	newTexture *EnvMap;

	float GenericAmount;

	float AmbientAmount;

	float DiffuseAmount;

	float SpecularAmount;

	float ShineAmount;

	float ShineStrengthAmount;

	float SelfIllumAmount;

	float OpacityAmount;

	float FilterColorAmount;

	float BumpAmount;

	float ReflectAmount;

	float RefractAmount;

	float EnvAmount;

	newResult LoadTextureFromFile(newEnum type,const newString& FileName);

	newResult CreateEmptyTexture(newEnum type,int width,int height, newColor3ub &c);

	newResult CreateEmptyTexture(newEnum type,int width,int height, newColor4ub &c);

	newResult LoadFromFile(const newString &FileName);

	newResult ObtainMaterialFromAseFile(FILE * fpIn, int iMatIdx);
	
	void Apply();

	void Upload();

private:

	int NumSubMat;



        
};

extern newManager<newMaterial> MaterialMan;

typedef newManager<newMaterial>::Items Materials;

extern Materials &Material;

#endif /* ifndef-define include guard */
