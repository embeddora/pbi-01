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

class newRender
{
private:

	std::list<newDisplayList> DList;

public:

	newRender(void);

	~newRender(void);

	BOOL SetupOpenGL(GL_Window *window);

	BOOL DestroyWindowGL (GL_Window* window);

	void Reshape(int width, int height);

	void RestoreAll();

	newString VendorName;

	newString RendererName;

	newString VersionName;

	newString Extensions;

	UINT Tri_per_frame;

	UINT Tri_per_second;

	UINT Tri_accum;

	UINT64 Frame_Num;

	UINT fps;

	newTimer FPSTimer;

	BOOL ISfps;

	UINT64 FPSTime;

	UINT64 LastFPSFrame;

	void StartCalculateFPS(float Time);

	void UpdateFPS(UINT64 &DeltaTime);

	UINT CurTexID;

	void TextureUpload(newTexture *tex);

	void TextureEnable(newTexture *tex);

	void TextureDisable();

	void TextureOperation(newEnum op);

	void TextureUpdate(newTexture *tex);

	newEnum CurrentRenderMode;

	void RenderMode(newEnum mode);

	newFont *CurFont;

	UINT FontDisplayWidth;

	UINT FontDisplayHeight;

	void TextMode2D(UINT Width, UINT Height)
	{
		FontDisplayWidth=Width;

		FontDisplayHeight=Height;

		ReshapeConsole();
	}

	BOOL CreateFont(newFont *font);

	void KillFont(newFont *font);

	void CreateConsole();

	void DrawConsole();

	UINT ConsoleDS;
	
	void ReshapeConsole();

	void UpdateConsole(UINT64 DeltaTime);

	newCamera *CurrentCamera;

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

class newGLRender : public newRender
{
public:

	newGLRender(void);

	~newGLRender(void);
};

extern newRender MainRender;

void newGetScreen(BYTE *Data);

void newPrint2D(float x,float y,const char *Text, ...);

#endif /* ifndef-define include guard */
