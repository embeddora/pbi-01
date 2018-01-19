#include "Types.h" 

#include "Engine.h"

#include "Font.h"

#include "GL/gl.h"

#include "GL/glu.h"

#include <math.h>

#include "Math.h"

//++#include "Sound.h" 


#include <cstdio>


float fFPS = 0;

const GLfloat boxVertexes[] =
{
	/*x,y,z*/
	-1, -1, -1,
	1, -1, -1,
	1,  1, -1,
	-1,  1, -1,
	-1, -1,  1,
	1, -1,  1,
	1,  1,  1,
	-1,  1,  1,
};

const float boxUVs[] =
{
	0, 0,
	0, 1,
	1, 1,
	1, 0
};

const float boxNormals[] =
{
	0,  0, -1,
	0, 0,  1,
	1,  0,  0,
	-1,  0, 0,
	0,  -1,  0,
	0,  1, 0
};
const int boxQuads[] =
{
	1, 0, 3, 2,
	4, 5, 6, 7,
	1, 2, 6, 5,
	0, 4, 7, 3,
	1, 5, 4, 0,
	3, 7, 6, 2
};

newMaterial *CurMat[3];

newTexture *CurTex;

newString MyName;

newPhysicObject mypobj;

UINT CubeDS;

void DrawCube();

void CreateDSCube();

float CubeSpeed=3;

float angle=0;



#if 0
newVector3f Ax(1,2,3);
newQuaternionf Rot(0,newVector3f(0,1,0));
newQuaternionf CubeAccel(CubeSpeed,Ax);
#else
/* printf("[error] in '%s' initialization 'newVector3f Ax(1,2,3);' not done \n", __func__);
printf("[error] in '%s' initialization 'newQuaternionf Rot(0,newVector3f(0,1,0))' not done \n", __func__);
printf("[error] in '%s' initialization 'newQuaternionf CubeAccel(CubeSpeed,Ax);' not done \n", __func__);
*/
#endif /* (0) */


UINT RenderPath=1;

BOOL IsEnableBlend=TRUE;

UINT SkyDS;

newTexture *SkyTex[6];

void CreateSky(const newString &filename);

void CreateDLSky();

void RenderSky();

newScene myScene;

/*
CSoundManager mSoundManager;

CCustomSound* pSoundFirst = NULL;
CCustomSound* pSoundSecond = NULL;
CCustomSound* pSoundThird = NULL;
CCustomSound* pSound = NULL;
*/
newLight *myLight;

void InitSettings()
{

	MainRender.CurrentCamera=newNewCamera("My Camera");

	MainRender.CurrentCamera->AttachTo(PhysicObjectMan.newNewItem("Camera0"));

	MainRender.CurrentCamera->SetCameraStyle(NEW_CAMERASTYLE_FIRST_PERSON);

	MainSettings.CONSOLE.STATUS=NEW_CONSOLE_FULLSCREEN;

	MyName="XXX_new";


	MainConsole.AddCMD("m_Sensitivity",NEW_VARIABLE_FLOAT,&MainSettings.Mouse.Sensitivity);

	MainConsole.AddCMD("m_Pitch",NEW_VARIABLE_FLOAT,&MainSettings.Mouse.Pitch);
	MainConsole.AddCMD("m_Yaw",NEW_VARIABLE_FLOAT,&MainSettings.Mouse.Yaw);

	MainSettings.LoadFromFile("Cfg\\default.cfg");

	myScene.LoadSceneFromAseFile("Scene\\LesNazis.ASE ");
/*
	mSoundManager.Init();	



	pSoundFirst = mSoundManager.Precache_Stream("Sound\\Bike.wav", true);
	pSoundSecond = mSoundManager.Precache_Stream("Sound\\Helicopter.wav", true);
	pSoundThird = mSoundManager.Precache_Stream("Sound\\Jeep.wav", true);

	mSoundManager.Volume(pSoundFirst, 5500);
	mSoundManager.Volume(pSoundSecond, 5500);
	mSoundManager.Volume(pSoundThird, 5500);

	mSoundManager.Play(pSoundFirst);
	mSoundManager.Play(pSoundSecond);
	mSoundManager.Play(pSoundThird);

	mSoundManager.Play(pSoundFirst);
*/

}


BOOL Initialize (GL_Window* window, Keys* keys, BOOL first)
{
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);

	glAlphaFunc(GL_GREATER,0.0f);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_CULL_FACE);

	glEnable(GL_NORMALIZE);

	if (!first)
	{

		MainRender.RestoreAll();

		CreateDSCube();
		CreateDLSky();
		return TRUE;
	}

	MainRender.StartCalculateFPS(1);

	angle = 0;

	MainRender.TextMode2D(800,600);
	

	MainRender.CreateConsole();

	MainConsole.IsDrawing=TRUE;
	
	if (newLoadFontAndCheckSpace("Font/ComicS_Bi.tga","Comic Sans",0))
	{
		MainRender.CreateFont(MainRender.CurFont);
	}



	CreateSky("World/");


	MainSettings.CONSOLE.STATUS=NEW_CONSOLE_FADEOUT;


//+++	MainRender.CurrentCamera->Pobj->Position=newVector3f(-50,-100,50);
	MainRender.CurrentCamera->Pobj->Position=newVector3f(50, 0, 0);
	MainRender.CurrentCamera->Pobj->rotEuler.Yaw = 140;
	MainRender.CurrentCamera->Pobj->rotEuler.Pitch = 10;
	MainRender.ApplyCamera();


	LightSystem.Init();
	myLight = LightSystem.AddLight("Source0");
	myLight->PObj->Position.Set(-100,100,0);
	myLight->SetInfinite(TRUE);
	myLight->SetDiffuseColor(newVector4f(1,1,1,1));
	myLight->Enable();
	myLight->Apply();

	myScene.PreCompute();
	
	newMouseToCenter();
	
	return TRUE;
}

void Deinitialize (void)							
{

	MainConsole.Add(0,"die Ende der ganze Sache");
}

void Update (UINT64 &DeltaTime)
{

	angle = angle + (signed __int64)DeltaTime/(signed __int64)NEW_TIC_SECOND;

	MainRender.UpdateFPS(DeltaTime);

	MainRender.UpdateConsole(DeltaTime);

	MainInput.ProcessInput();

	// 'w' - 
	if (MainInput.keys.keyDown[87])	
	{
/*
mSoundManager.Play(pSoundFirst);
mSoundManager.Pause(pSoundSecond);
mSoundManager.Pause(pSoundThird);
*/
MainRender.CurrentCamera->Move(NEW_CAMERA_FORWARD, 1);
	}

	// 's' - 
	if (MainInput.keys.keyDown[83])
	{
/*
mSoundManager.Play(pSoundFirst);
mSoundManager.Pause(pSoundSecond);
mSoundManager.Pause(pSoundThird);
*/
MainRender.CurrentCamera->Move(NEW_CAMERA_BACK, 1);
	}

	// 'd' - 
	if (MainInput.keys.keyDown[65])
	{
/*
mSoundManager.Pause(pSoundFirst);
mSoundManager.Play(pSoundSecond);
mSoundManager.Pause(pSoundThird);
*/
MainRender.CurrentCamera->Move(NEW_CAMERA_LEFT, 1);
	}
	// 'a' - 
	if (MainInput.keys.keyDown[68])
	{
/*
mSoundManager.Pause(pSoundFirst);
mSoundManager.Play(pSoundSecond);
mSoundManager.Pause(pSoundThird);
*/
MainRender.CurrentCamera->Move(NEW_CAMERA_RIGHT, 1);
	}

	// 'b' - up
	if (MainInput.keys.keyDown[66])
	{
/*
mSoundManager.Pause(pSoundFirst);
mSoundManager.Pause(pSoundSecond);
mSoundManager.Play(pSoundThird);
*/
MainRender.CurrentCamera->Move(NEW_CAMERA_UP, 1);
	}

	// 'c' - down
	if (MainInput.keys.keyDown[67])
	{
/*
mSoundManager.Pause(pSoundFirst);
mSoundManager.Pause(pSoundSecond);
mSoundManager.Play(pSoundThird);
*/
MainRender.CurrentCamera->Move(NEW_CAMERA_DOWN, 1);
	}

	MainRender.CurrentCamera->ApplyTransform(DeltaTime);
}


void Draw (void)
{
	MainRender.RenderMode(NEW_RENDERMODE_3D);

	glClear(GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	MainRender.ApplyCamera();

	glEnable(GL_LIGHTING);
	myLight->Enable();
	//myLight->PObj->Position.Set(100*(float)cos(angle),100*(float)sin(angle),100*(float)sin(angle));
	//myLight->PObj->Position.Set(-100,100,0);
	myLight->Apply();


	RenderSky();	

	//glPushMatrix();
	//glScalef(0.05f, 0.05f, 0.05f);

    myScene.Render();
	//glPopMatrix();
	
	glFlush();
	
	MainRender.RenderMode(NEW_RENDERMODE_2D);
	
	if (newSelectFont("Lucida Console"))
	{

		MainRender.CurFont->Width=1;
		MainRender.CurFont->Height=1;


		if (MainRender.ISfps)
		if (MainSettings.Render.DrawFPS)
		{

			MainRender.CurFont->Color=newColor4ub(255,255,128,255);

			UINT n=0;

			newPrint2D(10,(n++)*16.0f+10,"Frames per second: %d", MainRender.fps);
			newPrint2D(10,(n++)*16.0f+10,"Triangles per frame: %d", MainRender.Tri_per_frame);
			newPrint2D(10,(n++)*16.0f+10,"Triangles per sec: %d", MainRender.Tri_per_second);
#if 0
			newPrint2D(10,(n++)*16.0f+10,"Quaternion: %f %f %f %f", Rot.X, Rot.Y, Rot.Z, Rot.W);
#else
std::printf("[error] in '%s' 'Rot' is not ready \n", __func__);
#endif /* (0) */

			newPrint2D(10,(n++)*16.0f+10,"Euler corners: X = %f, Y = %f", MainRender.CurrentCamera->Pobj->rotEuler.Pitch, MainRender.CurrentCamera->Pobj->rotEuler.Yaw);
			newPrint2D(10,(n++)*16.0f+10,"Camera position: X = %f, Y = %f, Z = %f", MainRender.CurrentCamera->Pobj->Position.X, MainRender.CurrentCamera->Pobj->Position.Y, MainRender.CurrentCamera->Pobj->Position.Z);
			newPrint2D(10,(n++)*16.0f+10,"Camera quaternion: %f %f %f %f", MainRender.CurrentCamera->Pobj->Rotation.X, MainRender.CurrentCamera->Pobj->Rotation.Y, MainRender.CurrentCamera->Pobj->Rotation.Z, MainRender.CurrentCamera->Pobj->Rotation.W);
			
		}
	}

	if (newSelectFont("Comic Sans"))
	{

		MainRender.CurFont->Width=1;
		MainRender.CurFont->Height=1;

		MainRender.CurFont->Color=newColor4ub(255,255,255,255);
		if (MainSettings.Render.DrawHelp)
		{

			float dg=350;
			newPrint2D(10,dg,"Hot keys:");
			newPrint2D(10,dg+=16,"'Esc'- Exit");
			newPrint2D(10,dg+=16,"'F1' - Toggle video modes (window/ full screen)");
			newPrint2D(10,dg+=16,"'F12'- Make s screeon shot");

			if (fabs(MainSettings.FOV-90)<0.001)
				MainRender.CurFont->Color=newColor4ub(0,255,0,255);
			else MainRender.CurFont->Color=newColor4ub(255,255,255,255);
				newPrint2D(10,dg+=16,"'4'  - FOV = 120 grads.");

			if (fabs(MainSettings.FOV-15)<0.001)
				MainRender.CurFont->Color=newColor4ub(0,255,0,255);
			else MainRender.CurFont->Color=newColor4ub(255,255,255,255);
				newPrint2D(10,dg+=16,"'5'  - FOV = 15 grads.");

			if (IsEnableBlend)
				MainRender.CurFont->Color=newColor4ub(0,255,0,255);
			else MainRender.CurFont->Color=newColor4ub(255,255,255,255);
				newPrint2D(10,dg+=16,"'6'  - Transparency disabled.");

			if (!IsEnableBlend)
				MainRender.CurFont->Color=newColor4ub(0,255,0,255);
			else
				MainRender.CurFont->Color=newColor4ub(255,255,255,255);

			newPrint2D(10,dg+=16,"'7'  - Transparency, if alfa channel preent.");
#if 0
			if (RenderPath==1)
				newPrint2D(10,dg+=16,"'R' - \D2\E5\EA\F3\F9\E8\E9 \F1\EF\EE\F1\EE\E1 \EE\F2\F0\E8\F1\EE\E2\EA\E8: glRotate \E8 \E4\E8\F1\EF\EB\E5\E9\ED\FB\E5 \F1\EF\E8\F1\EA\E8");

			if (RenderPath==2)
				newPrint2D(10,dg+=16,"'R' - \D2\E5\EA\F3\F9\E8\E9 \F1\EF\EE\F1\EE\E1 \EE\F2\F0\E8\F1\EE\E2\EA\E8: \EA\E2\E0\F2\E5\F0\ED\E8\EE\ED\FB \E8 glBegin//glEnd");
#else
			if (RenderPath==1)
				std::printf ("[warning] '%s' : unknown escape sequence ignored (RenderPath==1) \n", __func__);

			if (RenderPath==2)
				std::printf ("[warning] '%s' : unknown escape sequence ignored (RenderPath==2) \n", __func__);
#endif /* (0) */
			if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_VISIBLE)
				MainRender.CurFont->Color=newColor4ub(0,255,0,255);
			else
				MainRender.CurFont->Color=newColor4ub(255,255,255,255);

			newPrint2D(10,dg+=16,"'F' Show/hide FPS, triangles, and camera information.");

			newPrint2D(10,dg+=16,"'H' Show/hide help sceen.");

			newPrint2D(10,dg+=16,"'~' (tild)  - Cosole. To available commands list type 'help'.");
		}
		else
			newPrint2D(10,MainRender.FontDisplayHeight-20.0f,"Hot keys 's', 'w', 'd', 'a', 'c', 'b', and use the MOUSE; press 'H' for help.");
	}


	MainRender.DrawConsole();

	glFlush();
	
}


BOOL ProcessKeys(
   HWND hWnd,
   UINT message,
   /* WPARAM 19012018 6:48 */ DWORD wParam,
   /* LPARAM 19012018 6:48 */ long lParam 
)
{
	if (wParam==192)
	{
		if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEOUT)
			|| (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_NOTVISIBLE))
		{
			MainSettings.CONSOLE.STATUS=NEW_CONSOLE_FADEIN;
			return TRUE;
		}
		if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEIN)
			|| (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_VISIBLE))
		{
			MainSettings.CONSOLE.STATUS=NEW_CONSOLE_FADEOUT;
			return TRUE;
		}
	}


	if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_VISIBLE) || 
		(MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEIN) )
	{

		MainConsole.ProcessKey((DWORD)wParam);

		return TRUE;
	}

	if (wParam == 72) MainSettings.Render.DrawHelp = MainSettings.Render.DrawHelp? FALSE: TRUE;


	if (wParam == 70) MainSettings.Render.DrawFPS = MainSettings.Render.DrawFPS? FALSE: TRUE;

	if (wParam==27)
#if 0
		PostMessage (hWnd, WM_CLOSE, 0, 0);
#else
//		std::printf("[warning] '%s' PostMessage (hWnd, WM_CLOSE, 0, 0) \n", __func__);
//		std::printf("[warning]  PostMessage (hWnd, WM_CLOSE, 0, 0) \n");
		printf("[warning]  PostMessage (hWnd, WM_CLOSE, 0, 0) \n");
#endif /* (0) */

	if (wParam==112)
#if 0
		PostMessage (hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
#else
//		std::printf("[warning] '%s' PostMessage (hWnd, WM_TOGGLEFULLSCREEN, 0, 0) \n", __func__);
		std::printf("[warning]  PostMessage (hWnd, WM_TOGGLEFULLSCREEN, 0, 0) \n");
#endif /* (0) */

#undef USE_LIBJPEG
#ifdef USE_LIBJPEG
	if (wParam==123) newSaveScreenToJPG();
#endif /* (0) */

	if (wParam==49) 
	{
		CurTex=CurMat[0]->DiffuseMap;
	}

	if (wParam==50) 
	{
		CurTex=CurMat[1]->DiffuseMap;
	}

	if (wParam==51)
	{
		CurTex=CurMat[2]->DiffuseMap;
	}

	if (wParam==52) MainConsole.ExecCMD("FOV 120");


	if (wParam==53) MainConsole.ExecCMD("FOV 15");

	if (wParam==54)
	{
		IsEnableBlend=TRUE;
	}

	if (wParam==55) 
	{
		IsEnableBlend=FALSE;
	}

	if (wParam==82) 
	{
		RenderPath=(RenderPath)%2+1;
	}


	return TRUE;
}

void DrawCube()
{
	glBegin(GL_QUADS);

	for(int i = 0; i < 6; i++ )
	{
		for (int j=0;j<4;j++)
		{
			glColor3fv(&boxVertexes[boxQuads[i*4+j]*3]);

			glTexCoord2fv(&boxUVs[j*2]);

			glVertex3fv(&boxVertexes[boxQuads[i*4+j]*3]);
		}
	}
	glEnd();
}

void CreateDSCube()
{
	CubeDS=glGenLists(1);

	glNewList(CubeDS,GL_COMPILE);

	DrawCube();

	glEndList();
}

void CreateSky(const newString &filename)
{

//++char *posname[]={"_0001","_0003","_0002","_0000","_0005","_0004"};
	const char * posname[]={"East","West","South","North","Hell","Heaven"};

int i;

	for (i=0;i<6;i++)
	{
		SkyTex[i]=TextureMan.newNewItem();

		SkyTex[i]->LoadFromFile(filename+newString(posname[i])+".jpg");

		SkyTex[i]->Border_Type = NEW_TEXTURE_BORDER_CLAMP;

		SkyTex[i]->Upload();
	}

	CreateDLSky();
}

void CreateDLSky()
{
	const float NEW_SKYBOX_SIZE=1.0f;

	const float SkyboxVertexes[] =
	{
		-NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE,
		 NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE,
		 NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE,
		-NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE,
		-NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE,
		 NEW_SKYBOX_SIZE, -NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE,
		 NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE,
		-NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE,  NEW_SKYBOX_SIZE,
	};

	const float SkyboxUVs[] =
	{

			0, 1,
			1, 1,
			1, 0,
			0, 0,

			0, 1,
			1, 1,
			1, 0,
			0, 0,

			0, 1,
			1, 1,
			1, 0,
			0, 0,

			0, 1,
			1, 1,
			1, 0,
			0, 0,

			0, 1,
			0, 0,
			1, 0,
			1, 1,

			1, 0,
			1, 1,
			0, 1,
			0, 0,

	};

	const int SkyboxQuads[] =
	{
		0, 1, 2, 3,
		5, 4, 7, 6,
		1, 5, 6, 2,
		4, 0, 3, 7,
		5, 1, 0, 4,
		7, 3, 2, 6
	};

	int i,j=0,v,c=0;

 	SkyDS=glGenLists(1);

	glNewList(SkyDS,GL_COMPILE);

	for( i = 0; i < 6; i++ )
	{
		SkyTex[i]->Enable();

		glBegin( GL_QUADS );
		for( j = 0; j < 4; j++ )
		{
			v = SkyboxQuads[(i*4)+j] * 3;

			glTexCoord2fv( &SkyboxUVs[i*8+j*2] );

			glVertex3fv( &SkyboxVertexes[v] );
		}
		glEnd();
	}

	glEndList();
}

void RenderSky()
{
	newVector3f temp=MainRender.CurrentCamera->Pobj->Position;

	glTranslatef(-temp.X, -temp.Y, -temp.Z);	

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	glDepthMask(0);

	glDisable(GL_LIGHTING);

	glColor3f(1,1,1);

	glCallList(SkyDS);

	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	glDepthMask(1);

	glEnable(GL_BLEND);

	glTranslatef(temp.X, temp.Y, temp.Z);

	MainRender.Tri_per_frame += 12;
}
