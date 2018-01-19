#include "Material.h"

#include "File.h"

#include "Console.h"

#include "Render.h"

#include "AseFile.h"

 /* 'strlen()' declaration;  18-01-2018 */
#include <string.h>



/* Global array to store materials */
newManager<newMaterial> MaterialMan;

Materials &Material = MaterialMan.Item;

newMaterial::newMaterial(void)
{
	GenericMap = NULL;

	AmbientMap = NULL;

	DiffuseMap = NULL;

	SpecularMap = NULL;

	ShineMap = NULL;

	ShineStrengthMap = NULL;

	SelfIllumMap = NULL;

	OpacityMap = NULL;

	FilterColorMap = NULL;

	BumpMap = NULL;

	ReflectMap = NULL;

	RefractMap = NULL;

	EnvMap = NULL;
}

/* Material constructor */
newMaterial::newMaterial(const newString &_Name,const newString &_DiffuseMap)
{
	Name=_Name;

	DiffuseMap=&Texture[_DiffuseMap];
}

/* Material destructor */
newMaterial::~newMaterial(void)
{

}

void newMaterial::memcopy(void* __dest,  void* __src, int __n)
{
int i;
char *d, *s;

	d = (char *)__dest;
	s = (char *)__src;
	
	for (i=0; i<__n; i++)
	    d[i] = s[i];
	    
}

/* Texture of type 'type' creation with 3 component color */ 
newResult newMaterial::CreateEmptyTexture(newEnum type,int width,int height, newColor3ub &c)
{
	switch (type)
	{
		/* 1. NEW_TEXTURE_DIFFUSE - basic texture */
		case NEW_TEXTURE_DIFFUSE:

			/* Creating the texture and storing its index */
			DiffuseMap=TextureMan.newNewItem();

			/* Filling texture in */
			DiffuseMap->CreateEmpty(width,height,c);

			return NEW_OK;

			break;
	}

	return GenError(NEW_UNKNOW,"Unknown texture type in material.");
}

/* Texture of type 'type' creation with 4 component color */ 
newResult newMaterial::CreateEmptyTexture(newEnum type,int width,int height, newColor4ub &c)
{
	switch (type)
	{
		/* 1. NEW_TEXTURE_DIFFUSE - basic texture */
		case NEW_TEXTURE_DIFFUSE:

			/* Creating the texture and storing its index */
			DiffuseMap=TextureMan.newNewItem();

			/* Filling texture in */
			DiffuseMap->CreateEmpty4(width,height,c);

			return NEW_OK;

			break;
	}

	return GenError(NEW_UNKNOW,"Unknown texture type in material.");
}


/* Making image from file 'FileName' to be a texture of type 'type' */ 
newResult newMaterial::LoadTextureFromFile(newEnum type,const newString& FileName)
{
	switch (type)
	{
		/* 1. NEW_TEXTURE_DIFFUSE - basic texture */
		case NEW_TEXTURE_DIFFUSE:

			/* Checking if such texture is already uploaded */
			if (Texture.find(FileName)==Texture.end())
			{
				/* If not then creating the texture and storing its index */
				DiffuseMap=TextureMan.newNewItem();
				
				/* Uploading contents of file 'FileName' into texture. Returning a result of such upload */
				return DiffuseMap->LoadFromFile(FileName);
			}

			return GenError(NEW_EXIST,(char *)FileName.c_str());

			break;
	}

	return GenError(NEW_UNKNOW,"Unknown texture type in material.");
}

newResult newMaterial::LoadFromFile(const newString &FileName)
{
newFile file;

newResult res = file.Open(FileName);

	if (res != NEW_OK)
		
		return res;

	newString sbuf;

	sbuf = file.ReadString(3);

	if (!(sbuf == NEW_MATERIAL_FILEID) )
		
		return GenError(NEW_UNKNOW_FILE_FORMAT, FileName.c_str());

	__int16 tok;
	
	file.Read(tok);

	if (tok != NEW_MATERIAL_VERSION)
		
		return GenError(NEW_UNKNOW_FILE_FORMAT, FileName.c_str());

	file.Read(tok);

	if (tok != NEW_MATERIAL)
		
		return GenError(NEW_UNKNOW,"Some error with id tokken");

	BaseShader = file.ReadString();

	if ("Standard" == BaseShader)
	{
		file.Read(tok);

		if (tok == NEW_AMBIENT)
		{
			file.Read(Ambient);

			file.Read(tok);
		}
		if (tok== NEW_DIFFUSE)
		{
			file.Read(Diffuse);

			file.Read(tok);
		}
		if (tok== NEW_SPECULAR)
		{
			file.Read(Specular);

			file.Read(tok);
		}
		if (tok == NEW_SHINE)
		{
			file.Read(Shine);

			file.Read(tok);
		}
		if (tok == NEW_SHINE_STRENGTH)
		{
			file.Read(ShineStrange);

			file.Read(tok);
		}
		if (tok == NEW_TRANSPARENCY)
		{
			file.Read(Transparency);

			if (NEW_FLOAT_EQ(Transparency,0))
				
				Transparency = 1;

			file.Read(tok);
		}
	}
	else
	{
		file.Read(tok);

		if (tok == NEW_AMBIENT)
		{
			file.Read(Ambient);

			file.Read(tok);
		}
		if (tok== NEW_DIFFUSE)
		{
			file.Read(Diffuse);

			file.Read(tok);
		}
		if (tok== NEW_SPECULAR)
		{
			file.Read(Specular);

			file.Read(tok);
		}
		if (tok == NEW_SHINE)
		{
			file.Read(Shine);

			file.Read(tok);
		}
		if (tok == NEW_SHINE_STRENGTH)
		{
			file.Read(ShineStrange);

			file.Read(tok);
		}
		if (tok == NEW_TRANSPARENCY)
		{
			file.Read(Transparency);

			if (NEW_FLOAT_EQ(Transparency,0))
				
				Transparency = 1;

			file.Read(tok);
		}
	}

	if (tok == NEW_NUMSUBMTLS)
	{
		file.Read(NumSubMat);

		file.Read(tok);
	}
	if (tok == NEW_SUBMATERIAL)
	{
		file.Read(NumSubMat);

		file.Read(tok);
	}	

	newTexture **tex;

	float *amt;

	BOOL istexture = TRUE;

	while ((istexture)&&(!file.Feof()))
	{
		istexture = FALSE;

		if (tok == NEW_MAP_GENERIC)
		{
			tex = &GenericMap;

			amt = &GenericAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_AMBIENT)			
		{
			tex = &AmbientMap;

			amt = &AmbientAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_DIFFUSE)			
		{
			tex = &DiffuseMap;

			amt = &DiffuseAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_SPECULAR)		
		{
			tex = &SpecularMap;

			amt = &SpecularAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_SHINE)			
		{
			tex = &ShineMap;

			amt = &ShineAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_SHINESTRENGTH)	
		{
			tex = &ShineStrengthMap;

			amt = &ShineStrengthAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_SELFILLUM)		
		{
			tex = &SelfIllumMap;

			amt = &SelfIllumAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_OPACITY)			
		{
			tex = &OpacityMap;

			amt = &OpacityAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_FILTERCOLOR)	
		{
			tex = &FilterColorMap;

			amt = &FilterColorAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_BUMP)
		{
			tex = &BumpMap;

			amt = &BumpAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_REFLECT)			
		{
			tex = &ReflectMap;

			amt = &ReflectAmount;

			istexture = TRUE;
		}
		if (tok == NEW_MAP_REFRACT)
		{
			tex = &RefractMap;

			amt = &RefractAmount;

			istexture = TRUE;
		}
		if (tok == NEW_ENVMAP)
		{
			tex = &EnvMap;

			amt = &EnvAmount;

			istexture = TRUE;
		}
		if (istexture)
		{
			newString TexName = file.ReadString();

			sbuf = file.ReadString();

			/* if (sbuf == "Bitmap") tex->LoadFromFile(TexName); */
			int subNo;

			file.Read(subNo);

			file.Read(*amt);

			file.Read(tok);

			if (tok == NEW_BITMAP)
			{
				sbuf = newString("Data\\Textures\\")+ file.ReadString();

				(*tex) = TextureMan.newNewItem(sbuf);

				(*tex)->LoadFromFile(sbuf);
			}

			/* if (tok == NEW_UVGEN) */
			if (!file.Feof())
				
				file.Read(tok);
		}
	}

	Name = FileName;

	return NEW_OK;
}


/* ... and the file already opened and pointed by non-ZERO pointer 'fpIn' */
newResult newMaterial::ObtainMaterialFromAseFile(FILE * fpIn, int iMaterialIdx)
{	
MaterialType cMaterial;

MapDissfuseType MapDissfuse;

char cpClass[LEXEMA];

int subNo;

float *amt;

newTexture **tex;

	fscanf (fpIn, "\t\t*MATERIAL_CLASS \"%s\n", (char* /* dummy. remove! 15:13 18-01-2018 */) &(cMaterial.chClass) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_CLASS \"%s\n", cMaterial.chClass );
	BaseShader = cpClass;

	fscanf (fpIn, "\t\t*MATERIAL_AMBIENT %f %f %f\n", &(cMaterial.cfAmbient[0]), &(cMaterial.cfAmbient[1]), &(cMaterial.cfAmbient[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_AMBIENT %f %f %f\n", cMaterial.cfAmbient[0], cMaterial.cfAmbient[1], cMaterial.cfAmbient[2]  );
	Ambient.Set(cMaterial.cfAmbient[0], cMaterial.cfAmbient[1], cMaterial.cfAmbient[2] );

	fscanf (fpIn, "\t\t*MATERIAL_DIFFUSE %f %f %f\n", &(cMaterial.cfDiffuse[0]), &(cMaterial.cfDiffuse[1]), &(cMaterial.cfDiffuse[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_DIFFUSE %f %f %f\n", (cMaterial.cfDiffuse[0]), (cMaterial.cfDiffuse[1]), (cMaterial.cfDiffuse[2]) );
	Diffuse.Set( cMaterial.cfDiffuse[0], cMaterial.cfDiffuse[1], cMaterial.cfDiffuse[2] );

	fscanf (fpIn, "\t\t*MATERIAL_SPECULAR %f %f %f\n", &(cMaterial.cfSpecular[0]), &(cMaterial.cfSpecular[1]), &(cMaterial.cfSpecular[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_SPECULAR %f %f %f\n", cMaterial.cfSpecular[0], cMaterial.cfSpecular[1], cMaterial.cfSpecular[2] );
	Specular.Set(cMaterial.cfSpecular[0], cMaterial.cfSpecular[1], cMaterial.cfSpecular[2]);

	fscanf (fpIn, "\t\t*MATERIAL_SHINE %f\n", &(cMaterial.fShine) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_SHINE %f\n", cMaterial.fShine);
	Shine = cMaterial.fShine;

	fscanf (fpIn, "\t\t*MATERIAL_SHINESTRENGTH %f\n", &(cMaterial.fShineStrength) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_SHINESTRENGTH %f\n", cMaterial.fShineStrength );
	ShineStrange = cMaterial.fShineStrength;

	fscanf (fpIn, "\t\t*MATERIAL_TRANSPARENCY %f\n", &(cMaterial.fTransperency) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_TRANSPARENCY %f\n", cMaterial.fTransperency);
	Transparency = cMaterial.fTransperency;	if (NEW_FLOAT_EQ(cMaterial.fTransperency,0)) Transparency = 1;

	fscanf (fpIn, "\t\t*MATERIAL_WIRESIZE %f\n", &(cMaterial.fWireSize) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_WIRESIZE %f\n", cMaterial.fWireSize);	
	// oops ! gibt's kein Feld "Wiresize" innerhalb des "GunMaterial::" Klasses.

	fscanf (fpIn, "\t\t*MATERIAL_SHADING %s\n", (char* /* dummy. remove! 15:13 18-01-2018 */)  &(cMaterial.chShading) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_SHADING %s\n", cMaterial.chShading);
	// oops ! gibt's kein Feld "Shading" innerhalb des "GunMaterial::" Klasses.

	fscanf (fpIn, "\t\t*MATERIAL_XP_FALLOFF %f\n", &(cMaterial.fXpFallOff));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_XP_FALLOFF %f\n", cMaterial.fXpFallOff);
	// oops ! gibt's kein float Feld "XpFalloff" innerhalb des "GunMaterial::" Klasses.

	fscanf (fpIn, "\t\t*MATERIAL_SELFILLUM %f\n", &(cMaterial.fSelfIllum));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_SELFILLUM %f\n", cMaterial.fSelfIllum);
	// oops ! gibt's kein _float_ feld "SelfIllum" innerhalb des "GunMaterial::" Klasses.	

	fscanf (fpIn, "\t\t*MATERIAL_FALLOFF %s\n",  (char* /* dummy. remove! 15:13 18-01-2018 */)  &(cMaterial.chFallOff));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_FALLOFF %s\n", cMaterial.chFallOff );
	// oops ! gibt's kein _string_ feld "cpFalloff" innerhalb des "GunMaterial::" Klasses.	

	fscanf (fpIn, "\t\t*MATERIAL_XP_TYPE %s\n",(char* /* dummy. remove! 15:13 18-01-2018 */)  &(cMaterial.chXpType) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MATERIAL_XP_TYPE %s\n", cMaterial.chXpType );
	// oops ! gibt's kein _string_ feld "cpXpType" innerhalb des "GunMaterial::" Klasses.	

	// starting with map . 

	fscanf(fpIn, "\t\t*MAP_DIFFUSE {\n\t\t\t*MAP_NAME \"%s #%d\"\n", (char* /* dummy. remove! 15:13 18-01-2018 */)  &(MapDissfuse.chNameStr), &(MapDissfuse.iSubNo));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t*MAP_DIFFUSE {\n\t\t\t*MAP_NAME \"%s #%d\"\n", MapDissfuse.chNameStr, MapDissfuse.iSubNo );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*MAP_CLASS \"%s\n",  (char* /* dummy. remove! 15:13 18-01-2018 */) &(MapDissfuse.chClass));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*MAP_CLASS \"%s\n", MapDissfuse.chClass);
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*MAP_SUBNO %d\n", &(MapDissfuse.iSubNo) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*MAP_SUBNO %d\n", MapDissfuse.iSubNo );
	subNo = MapDissfuse.iSubNo; // 'subNo' seems to be useless

	fscanf (fpIn, "\t\t\t*MAP_AMOUNT %f\n", &(MapDissfuse.fMapAmount) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*MAP_AMOUNT %f\n", MapDissfuse.fMapAmount);	
	amt = &DiffuseAmount; *amt = MapDissfuse.fMapAmount;

	fscanf (fpIn, "\t\t\t*BITMAP %s\n", (char* /* dummy. remove! 15:13 18-01-2018 */)  &(MapDissfuse.chBitmap) );
	{
	int _ln = strlen (MapDissfuse.chBitmap) ;

		memcopy (MapDissfuse.chBitmap, &((MapDissfuse.chBitmap)[1]), strlen(MapDissfuse.chBitmap) -2 );

		(MapDissfuse.chBitmap)[_ln - 1 - 1] = '\0';

		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*BITMAP %s\n", MapDissfuse.chBitmap);
	}

	if ('\0' != (MapDissfuse.chBitmap)[0])
	{								
	char cpTxtFname[LEXEMA];

	tex = &DiffuseMap;	   
		   
		/* 'iMaterialIdx' comes as parameter. */
		sprintf(cpTxtFname, "%s%c", "MTL", iMaterialIdx+'0');

		(*tex) = TextureMan.newNewItem(cpTxtFname);

		/* i.e. texsture::loadfromfile */
		(*tex)->LoadFromFile(MapDissfuse.chBitmap);
	}

	fscanf (fpIn, "\t\t\t*MAP_TYPE %s\n", (char* /* dummy. remove! 15:13 18-01-2018 */) &(MapDissfuse.chMapType));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*MAP_TYPE %s\n", MapDissfuse.chMapType);
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_U_OFFSET %f\n", &(MapDissfuse.fUvwOffetU));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_U_OFFSET %f\n", MapDissfuse.fUvwOffetU);
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_V_OFFSET %f\n", &(MapDissfuse.fUvwOffetV) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_V_OFFSET %f\n", MapDissfuse.fUvwOffetV );
	// the same - so I ignore it 
	
	fscanf (fpIn, "\t\t\t*UVW_U_TILING %f\n", &(MapDissfuse.fUvwTilingU) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_U_TILING %f\n", MapDissfuse.fUvwTilingU );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_V_TILING %f\n", &(MapDissfuse.fUvwTilingV) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_V_TILING %f\n", MapDissfuse.fUvwTilingV );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_ANGLE %f\n", &(MapDissfuse.fUvwAngle) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_ANGLE %f\n", MapDissfuse.fUvwAngle );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_BLUR %f\n", &(MapDissfuse.fUvwBlur) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_BLUR %f\n", MapDissfuse.fUvwBlur );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_BLUR_OFFSET %f\n", &(MapDissfuse.fUvwBlurOffset) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_BLUR_OFFSET %f\n", MapDissfuse.fUvwBlurOffset );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_NOUSE_AMT %f\n", &(MapDissfuse.fUvwNouseAmt) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_NOUSE_AMT %f\n", MapDissfuse.fUvwNouseAmt );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_NOISE_SIZE %f\n", &(MapDissfuse.fUvwNoiseSize) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_NOISE_SIZE %f\n", MapDissfuse.fUvwNoiseSize );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_NOISE_LEVEL %d\n", &(MapDissfuse.iUvwNoiseLevel) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_NOISE_LEVEL %d\n", MapDissfuse.iUvwNoiseLevel );
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*UVW_NOISE_PHASE %f\n", &(MapDissfuse.fUvwNoisePhase) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*UVW_NOISE_PHASE %f\n", MapDissfuse.fUvwNoisePhase);
	// the same - so I ignore it 

	fscanf (fpIn, "\t\t\t*BITMAP_FILTER %s\n\t\t}\n", (char* /* dummy. remove! 15:13 18-01-2018 */)  &(MapDissfuse.chBitmap));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t\t*BITMAP_FILTER %s\n\t\t}\n", MapDissfuse.chBitmap);
	// the same - so I ignore it 

///////////////////////////////////////////////////////////////////////

	MainConsole.Add(0," Warning: current way of ASE-file unraveling _IGNORES_ THE _SUBMATERIALS_");

	return NEW_OK;
}


void newMaterial::Apply()
{
static newMaterial* CurMat = NULL;

	if (CurMat == this)
		
		return;

	if (DiffuseMap != NULL)

		DiffuseMap->Enable();
	else
	{
		if (GenericMap)
			
			GenericMap->Enable();

		else

			MainRender.TextureDisable();
	}
	newVector4f Col(Diffuse.X,Diffuse.Y,Diffuse.Z,Transparency);

	MainRender.SetColor(Col);

	MainRender.SetMaterial(this);

	CurMat = this;
}

void newMaterial::Upload()
{
	if (DiffuseMap!=NULL)

		if (DiffuseMap->TextureID)

			DiffuseMap->Upload();

	if (GenericMap!=NULL)

		if (GenericMap->TextureID)

			GenericMap->Upload();
}
