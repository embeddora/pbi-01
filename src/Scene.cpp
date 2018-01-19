#include "Scene.h"

#include "Console.h"

#include "Render.h"

#include "AseFile.h"

namespace std { using namespace __gnu_cxx; }

newScene::newScene(void)
{

}

newScene::~newScene(void)
{

}

newResult newScene::LoadFromFile(newString FileName)
{
	MainConsole.Add(0,"Загрузка сцены: %s", FileName.c_str());

	newFile sfile;

	newResult res;

	if (NEW_OK != (res = sfile.Open(FileName)))
		
		return res;

	newString sbuf;

	sbuf = sfile.ReadString(3);

	if (!(sbuf == NEW_SCENE_FILEID) )
		
		return GenError(NEW_UNKNOW_FILE_FORMAT, FileName.c_str());

	__int16 tok;

	int ibuf, count;

	/* UINT uibuf; */

	sfile.Read(tok);

	if (tok != NEW_SCENE_VERSION)
		
		return GenError(NEW_UNKNOW_FILE_FORMAT, FileName.c_str());

	sfile.Read(tok);

	while (tok!=NEW_SCENE_END)
	{
		switch(tok)
		{

		case NEW_STATICBGCOLOR:

			sfile.Read(StaticBGColor/* newVector3f */);

			break;

		case NEW_STATICAMBIENT:

			sfile.Read(StaticAmbient /* newVector3f */);

			break;

		case NEW_MATERIAL_LIST:

			sfile.Read(count);

			while (count)
			{
				/* Загрузка материала */
				sfile.Read(ibuf);

				sbuf = sfile.ReadString();

				LoadMaterial(sbuf, ibuf);

				count--;
			}

			break;

		case NEW_ID_MESH:

			sfile.Read(tok);

			if (NEW_NODE_NAME == tok)				

				sbuf = sfile.ReadString();

			LoadObject3D(sbuf, sfile);

			break;

		default:
			;
		}
		sfile.Read(tok);
	}

	MainConsole.Add(0,"Сцена: ОК");

	return NEW_OK;
}


newResult newScene::LoadSceneFromAseFile(newString FileName)
{
	MainConsole.Add(0,"MAZ: Загрузка сцены: %s", FileName.c_str());

	newFile sfile;

	newResult res;

	if ((res = sfile.Open(FileName)) != NEW_OK)
	{
MainConsole.Add(0,"MAZ: Загрузка сцены: %s", FileName.c_str());

		return res;
	}

{ /*  start of herr-mazukevich-inserion */

AseFileType cAseFile;

AseSceneType cAseScene;

//char cDumArr[DUM_ARRAY_LENGTH];

int iMatCount = 0, iMatCountRes;
int iDummy;/* remove! 15:23 18-01-2018 */

FILE * fpIn;

//char		*cpFirst	= &cDumArr[DUM_ARRAY_LENGTH/8*0],	*cpSecond	= &cDumArr[DUM_ARRAY_LENGTH/8*1],	*cpThird	= &cDumArr[DUM_ARRAY_LENGTH/8*2],	*cpFourth	= &cDumArr[DUM_ARRAY_LENGTH/8*3],	*cpFifth	= &cDumArr[DUM_ARRAY_LENGTH/8*4],	*cpSixth	= &cDumArr[DUM_ARRAY_LENGTH/8*5],	*cpSeventh	= &cDumArr[DUM_ARRAY_LENGTH/8*6],	*cpEigth	= &cDumArr[DUM_ARRAY_LENGTH/8*7];

int			iWhileRes;

	fpIn = sfile.GetFile();

	fscanf ( fpIn , "*3DSMAX_ASCIIEXPORT %d\n", &(cAseFile.i3dsMaxAsciiExport));

	fscanf (fpIn , "*COMMENT \"%s %s %2d,%02d - %s %s %02d %02d:%02d:%02d %4d\"\n",
		(char* /* remove! 15:23 18-01-2018 */) &(cAseFile.chAsciiExp), (char* /* remove! 15:23 18-01-2018 */)&(cAseFile.chVersion),
		(int*)&(cAseFile.iVer),			(int*)&(cAseFile.iSubVer),
		(char* /* remove! 15:23 18-01-2018 */)&(cAseFile.chWeekDay),		(char* /* remove! 15:23 18-01-2018 */)&(cAseFile.chMonth),
		(int*)&(cAseFile.iDate),		(int*)&(cAseFile.iHours),
		(int*)&(cAseFile.iSeconds),		(int*)&(cAseFile.iYear), (int*)&iDummy/* remove! 15:23 18-01-2018 */
	);

	fscanf (fpIn , "*SCENE {\n");


	fscanf (fpIn , "\t*SCENE_FILENAME %s\n", (char* /* dummy. remove! 15:23 18-01-2018 */) &(cAseScene.chFileName) );

	fscanf (fpIn , "\t*SCENE_FIRSTFRAME %d\n", &(cAseScene.iFirstFrame));

	fscanf (fpIn , "\t*SCENE_LASTFRAME %d\n", &(cAseScene.iLastFrame));

	fscanf (fpIn , "\t*SCENE_FRAMESPEED %d\n", &(cAseScene.iFrameSpeed));

	fscanf (fpIn , "\t*SCENE_TICKSPERFRAME  %d\n", &(cAseScene.iTicksPerFrame));

	fscanf (fpIn , "\t*SCENE_BACKGROUND_STATIC %f %f %f\n", &(cAseScene.cfBackgroundStatic[0]), &(cAseScene.cfBackgroundStatic[1]), &(cAseScene.cfBackgroundStatic[2]) );

	StaticBGColor.Set( cAseScene.cfBackgroundStatic[0], cAseScene.cfBackgroundStatic[1], cAseScene.cfBackgroundStatic[2] );

	fscanf (fpIn , "\t*SCENE_AMBIENT_STATIC %f %f %f\n}\n", &(cAseScene.cfAmbientStatic[0]), &(cAseScene.cfAmbientStatic[1]), &(cAseScene.cfAmbientStatic[2]) );

	StaticAmbient.Set( (cAseScene.cfAmbientStatic[0]), (cAseScene.cfAmbientStatic[1]), (cAseScene.cfAmbientStatic[2]) );

	fscanf (fpIn, "*MATERIAL_LIST {\n\t*MATERIAL_COUNT %d\n", &iMatCount);

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"*3DSMAX_ASCIIEXPORT  %d \n", cAseFile.i3dsMaxAsciiExport);
MainConsole.Add (NEW_ASEDBG_MESSAGE_LEVEL, "*COMMENT \"%s %s %2d,%02d - %s %s %02d %02d:%02d:%02d %4d\"\n", 		(cAseFile.chAsciiExp), 		(cAseFile.chVersion),		(cAseFile.iVer),		(cAseFile.iSubVer),		(cAseFile.chWeekDay),		(cAseFile.chMonth),		(cAseFile.iDate),		(cAseFile.iHours),		(cAseFile.iSeconds),		(cAseFile.iYear)	 );	
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "*SCENE {\n"); 
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_FILENAME %s\n", cAseScene.chFileName );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_FIRSTFRAME %d\n", cAseScene.iFirstFrame);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_LASTFRAME %d\n", cAseScene.iLastFrame);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_FRAMESPEED %d\n", cAseScene.iFrameSpeed);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_TICKSPERFRAME  %d\n", cAseScene.iTicksPerFrame);
MainConsole.Add (NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_BACKGROUND_STATIC %f %f %f\n", cAseScene.cfBackgroundStatic[0], cAseScene.cfBackgroundStatic[1], cAseScene.cfBackgroundStatic[2] );
MainConsole.Add (NEW_ASEDBG_MESSAGE_LEVEL, "\t*SCENE_AMBIENT_STATIC %f %f %f\n}\n",  (cAseScene.cfAmbientStatic[0]), (cAseScene.cfAmbientStatic[1]), (cAseScene.cfAmbientStatic[2]) ); 
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "*MATERIAL_LIST {\n\t*MATERIAL_COUNT %d\n", iMatCount);

	iMatCountRes = iMatCount;

	while (iMatCount--)

		LoadMaterialFromAseFile(fpIn);

	fscanf (fpIn, "}\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"}\n");

	/* while '*fpIn'  can be read */
	while ( !feof (fpIn) )
	{
		char cpNodeName[LEXEMA];	

		if (  !( 0 < ( iWhileRes = fscanf (fpIn, "*GEOMOBJECT {\n\t*NODE_NAME \"%s\n", cpNodeName) ) ) )
		{
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "%d =============== ALL ! \n", iWhileRes);

			break;
		}
		else
		{

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "*GEOMOBJECT {\n\t*NODE_NAME \"%s\n", cpNodeName ) ;

					LoadObject3dObjectFromAseFile(fpIn, cpNodeName, iMatCountRes);

		} /* else */

	} /* while ( !feof (fpIn) ) */
	
} /* herr-mazukevich-inserion */

	MainConsole.Add(0,"MAZ: Сцена: ОК");

	return NEW_OK;
}


newResult newScene::LoadMaterial(newString FileName, UINT mtlRef)
{
	MainConsole.Add(0,"Загрузка материала: %s", FileName.c_str());

	/* Загрузка материала */
	newMaterial *mtl = MaterialMan.newNewItem(FileName);

	mtl->Ref = mtlRef;

	MatList.push_back(mtl);

	newResult res = mtl->LoadFromFile(newString("Data\\Materials\\")+FileName+"."+NEW_MATERIAL_FILEID);

	if (res != NEW_OK)
		
		return res;

	MainConsole.Add(0,"Материал: ОК");

	return NEW_OK;
}

newResult newScene::LoadMaterialFromAseFile(FILE * fpIn)
{
int iMaterialId;
char cpSafePlace[LEXEMA*2];
MaterialType cMaterial;

	fscanf (fpIn, "\t*MATERIAL %d {\n", &iMaterialId );


	fscanf (fpIn, "\t\t*MATERIAL_NAME \"%d - %s\n", &(cMaterial.iNameIdx), (char* /* 15:23 18-01-2018 */)  &(cMaterial.chNameStr));

	sprintf (cpSafePlace, "%d - %s", cMaterial.iNameIdx, cMaterial.chNameStr);

	MainConsole.Add(0,"MAZ Загрузка материала: %s", cpSafePlace);

	/* Loading material */
	newMaterial *mtl = MaterialMan.newNewItem(cpSafePlace);

	mtl->Ref = iMaterialId;

	MatList.push_back(mtl);

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*MATERIAL %d {\n", iMaterialId);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MATERIAL_NAME \"%s\n", cpSafePlace);

	newResult res = mtl->ObtainMaterialFromAseFile(fpIn, iMaterialId);

	fscanf (fpIn, "\t}\n");

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t}\n");

	if (res != NEW_OK)
		
		return res;

	MainConsole.Add(0,"MAZ Материал: ОК");

	return NEW_OK;
}

newResult newScene::LoadObject3D(newString FileName, newFile &sfile)
{
	/*	try { */

	MainConsole.Add(0,"Загрузка объекта: %s", FileName.c_str());

	__int16 tok;

	int ibuf;

	/* Создание Object3D */

	/* static IDs=0; */
	
	newObject3D *obj = Object3DMan.newNewItem(FileName);

	ObjList.push_back(obj);

	obj->Type = NEW_OBJ3D_STATIC;
	
	/* Загрузка физического объекта */
	sfile.Read(tok);

	if (tok==NEW_NODE_TM)
	{
		newString sbuf;

		sfile.Read(tok);

		if (NEW_NODE_NAME == tok)
			
			sbuf = sfile.ReadString();

		obj->BornPhysicObject(sbuf);

		sfile.Read(obj->Pobj->Position);

		sfile.Read(obj->Pobj->Rotation);
	}
	
	/* Загрузка меша */
	newString nodename = FileName;

	int qq = nodename.length()-1;

	while ((qq>=0) && (nodename[qq--]!='_') )
	{
	}

	if (qq>0)
	{
		nodename = nodename.substr(0,qq+1);
	}

	nodename = newString("Data\\Objects\\") + nodename + "." + NEW_MESH_FILEID;

	obj->BornMesh(nodename);

	obj->Mesh->LoadFromFile(nodename);

	sfile.Read(tok);

	if (NEW_PROP_MOTIONBLUR == tok)
	{
		sfile.Read(ibuf);

		obj->MotionBlur = (BOOL)ibuf;
	}

	sfile.Read(tok);

	if (NEW_PROP_CASTSHADOW == tok)
	{
		sfile.Read(ibuf);

		obj->CastShadows = (BOOL)ibuf;
	}
	sfile.Read(tok);

	if (NEW_PROP_RECVSHADOW == tok)
	{
		sfile.Read(ibuf);

		obj->RecvShadows = (BOOL)ibuf;
	}

	sfile.Read(tok);

	if (NEW_MATERIAL_REF == tok)
	{
		sfile.Read(ibuf);

		obj->Mtl = MatList[ibuf];
	}

	MainConsole.Add(0,"Объект: ОК");

	/* } catch (...) {	MainConsole.Add(0,"Объект: Ошибка");	} */

	return NEW_OK;
}

newResult newScene::LoadObject3dObjectFromAseFile(FILE * fpIn, char * cpNodeName, int iMatCountRes)
{
NodeTmType NodeTm;

GeomObjectType GeomObject;

newVector3f RotVectAxis;

	fscanf (fpIn, "\t*NODE_TM {\n\t\t*NODE_NAME \"%s\n", (char* /* 15:26 18-01-2018 */)  &(NodeTm.chNodeNameAgain) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*NODE_TM {\n\t\t*NODE_NAME \"%s\n", NodeTm.chNodeNameAgain );

	/*	try { */
	MainConsole.Add(0,"MAZ Загрузка объекта: %s", NodeTm.chNodeNameAgain);

	/* Создание Object3D */
	newObject3D *obj = Object3DMan.newNewItem(NodeTm.chNodeNameAgain);

	ObjList.push_back(obj);

	obj->Type = NEW_OBJ3D_STATIC;
	
	/* Загрузка физического объекта */
	fscanf (fpIn, "\t\t*INHERIT_POS %d %d %d\n", &(NodeTm.ciInheritPos[0]),	&(NodeTm.ciInheritPos[1]), &(NodeTm.ciInheritPos[2]) );


	fscanf (fpIn, "\t\t*INHERIT_ROT %d %d %d\n", &(NodeTm.ciInheritRot[0]), &(NodeTm.ciInheritRot[1]), &(NodeTm.ciInheritRot[2]) );

	fscanf (fpIn, "\t\t*INHERIT_SCL %d %d %d\n", &(NodeTm.ciInheritScl[0]), &(NodeTm.ciInheritScl[1]), &(NodeTm.ciInheritScl[2]) );


	fscanf (fpIn, "\t\t*TM_ROW0 %f %f %f\n", &(NodeTm.cfRow0[0]), &(NodeTm.cfRow0[1]), &(NodeTm.cfRow0[2]) );

	fscanf (fpIn, "\t\t*TM_ROW1 %f %f %f\n",  &(NodeTm.cfRow1[0]), &(NodeTm.cfRow1[1]), &(NodeTm.cfRow1[2])  );

	fscanf (fpIn, "\t\t*TM_ROW2 %f %f %f\n",  &(NodeTm.cfRow2[0]), &(NodeTm.cfRow2[1]), &(NodeTm.cfRow2[2]) );

	fscanf (fpIn, "\t\t*TM_ROW3 %f %f %f\n",  &(NodeTm.cfRow3[0]), &(NodeTm.cfRow3[1]), &(NodeTm.cfRow3[2]) );

	fscanf (fpIn, "\t\t*TM_POS %f %f %f\n", &(NodeTm.cfPos[0]),  &(NodeTm.cfPos[1]),  &(NodeTm.cfPos[2]) );

	fscanf (fpIn, "\t\t*TM_ROTAXIS %f %f %f\n", &(NodeTm.cfRotAxis[0]), &(NodeTm.cfRotAxis[1]), &(NodeTm.cfRotAxis[2]) );

	fscanf (fpIn, "\t\t*TM_ROTANGLE %f\n", &(NodeTm.fRotAngle) );

	fscanf (fpIn, "\t\t*TM_SCALE %f %f %f\n", &(NodeTm.cfScale[0]), &(NodeTm.cfScale[1]), &(NodeTm.cfScale[2]) );

	fscanf (fpIn, "\t\t*TM_SCALEAXIS %f %f %f\n", &(NodeTm.cfScaleAxis[0]), &(NodeTm.cfScaleAxis[1]), &(NodeTm.cfScaleAxis[2]) );

	fscanf (fpIn, "\t\t*TM_SCALEAXISANG %f\n\t}\n", &(NodeTm.fScaleAxisAng));

	/* register node under nodename */
	obj->BornPhysicObject(NodeTm.chNodeNameAgain);

	obj->Pobj->Position.Set(NodeTm.ciInheritPos[0], NodeTm.ciInheritPos[1], NodeTm.ciInheritPos[2]);

//+++ ATTENTION: do I really need the following?	RotVectAxis.Set(vectTmRotAxis.X ,vectTmRotAxis.Y, vectTmRotAxis.Z);

	obj->Pobj->Rotation.FromAxisAngle(NodeTm.fRotAngle, RotVectAxis); 	

	obj->BornMesh(NodeTm.chNodeNameAgain);

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*INHERIT_POS %d %d %d\n", (NodeTm.ciInheritPos[0]), (NodeTm.ciInheritPos[1]), (NodeTm.ciInheritPos[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*INHERIT_ROT %d %d %d\n", (NodeTm.ciInheritRot[0]), (NodeTm.ciInheritRot[1]), (NodeTm.ciInheritRot[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*INHERIT_SCL %d %d %d\n",  (NodeTm.ciInheritScl[0]), (NodeTm.ciInheritScl[1]), (NodeTm.ciInheritScl[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_ROW0 %f %f %f\n",  (NodeTm.cfRow0[0]), (NodeTm.cfRow0[1]), (NodeTm.cfRow0[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_ROW1 %f %f %f\n", (NodeTm.cfRow1[0]), (NodeTm.cfRow1[1]), (NodeTm.cfRow1[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_ROW2 %f %f %f\n", (NodeTm.cfRow2[0]), (NodeTm.cfRow2[1]), (NodeTm.cfRow2[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_ROW3 %f %f %f\n", (NodeTm.cfRow3[0]), (NodeTm.cfRow3[1]), (NodeTm.cfRow3[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_POS %f %f %f\n", (NodeTm.cfPos[0]),  (NodeTm.cfPos[1]),  (NodeTm.cfPos[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_ROTAXIS %f %f %f\n",  (NodeTm.cfRotAxis[0]), (NodeTm.cfRotAxis[1]), (NodeTm.cfRotAxis[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_ROTANGLE %f\n", NodeTm.fRotAngle );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_SCALE %f %f %f\n", (NodeTm.cfScale[0]), (NodeTm.cfScale[1]), (NodeTm.cfScale[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_SCALEAXIS %f %f %f\n", (NodeTm.cfScaleAxis[0]), (NodeTm.cfScaleAxis[1]), (NodeTm.cfScaleAxis[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*TM_SCALEAXISANG %f\n\t}\n", NodeTm.fScaleAxisAng);

	/* Load mesh */
	obj->Mesh->LoadMeshFromAseFile(fpIn, NodeTm.chNodeNameAgain);
	
	fscanf (fpIn, "\t}\n");

	fscanf (fpIn, "\t*PROP_MOTIONBLUR %d\n", &(GeomObject.iPropMotionBlur));

	obj->MotionBlur = (BOOL)GeomObject.iPropMotionBlur;

	fscanf (fpIn, "\t*PROP_CASTSHADOW %d\n", &(GeomObject.iPropCastShadow));

	obj->CastShadows = (BOOL)GeomObject.iPropCastShadow;

	fscanf (fpIn, "\t*PROP_RECVSHADOW %d\n", &(GeomObject.iProcpRecvShadow));

	obj->RecvShadows = (BOOL)GeomObject.iProcpRecvShadow;


MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t}\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*PROP_MOTIONBLUR %d\n", GeomObject.iPropMotionBlur);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*PROP_CASTSHADOW %d\n", GeomObject.iPropCastShadow);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*PROP_RECVSHADOW %d\n", GeomObject.iProcpRecvShadow);

#define ROBUST_AMOUNT 200

	if ((iMatCountRes) && (iMatCountRes < ROBUST_AMOUNT) )
	{

		fscanf (fpIn, "\t*MATERIAL_REF %d\n}\n", &(GeomObject.iMaterialRef));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*MATERIAL_REF %d\n}\n", GeomObject.iMaterialRef);

		if (GeomObject.iMaterialRef < ROBUST_AMOUNT)

			obj->Mtl = MatList[GeomObject.iMaterialRef];
		else
			obj->Mtl = MatList[0];	
	}
	else /* if ((iMatCountRes) && (iMatCountRes .... */
	{

		fscanf (fpIn, "\t*WIREFRAME_COLOR %f %f %f\n}\n", &(GeomObject.cfWireFrameColor[0]), &(GeomObject.cfWireFrameColor[1]), &(GeomObject.cfWireFrameColor[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*WIREFRAME_COLOR %f %f %f\n}\n", (GeomObject.cfWireFrameColor[0]), (GeomObject.cfWireFrameColor[1]), (GeomObject.cfWireFrameColor[2]) );

		MainConsole.Add(0, "WARNING: check LoadObject3dObjectFromAseFile() - no materials!\n");

		return NEW_NOT_IMPLEMENTED;
	} /* if ((iMatCountRes) && (iMatCountRes .... */

	MainConsole.Add(0,"MAZ Объект: ОК");	

	return NEW_OK;
}


int ObjCompare(newObject3D *A,newObject3D *B)
{
	/* if (A->Mtl->Transparency < B->Mtl->Transparency) return -1; */

	/* if (A->Mtl < B->Mtl) return 1; */

	if (A->Mtl > B->Mtl)
		
		return -1;

	return 0;
}

int ObjCompareTansp(newObject3D *A,newObject3D *B)
{
#if ATTENTION_RAW_STUFF

#else
	if (A->Mtl->Transparency < B->Mtl->Transparency)
		
		return -1;

	/*	if (A->Mtl < B->Mtl) return 1; */

	/*	if (A->Mtl > B->Mtl) return -1; */
#endif /* (0) */

	return 0;
}

void newScene::SortObjects()
{
	for (std::list<newObject3D *>::iterator it = ObjList.begin(); it!=ObjList.end();it++)

		for (std::list<newObject3D *>::iterator to = it; to!=ObjList.end();to++)

			if (ObjCompare((*it),(*to))< 0 ) 

				std::swap(*it,*to);

	for (std::list<newObject3D *>::iterator it = ObjList.begin(); it!=ObjList.end();it++)

		for (std::list<newObject3D *>::iterator to = it; to!=ObjList.end();to++)

			if (ObjCompareTansp((*it),(*to))< 0 ) 

				std::swap(*it,*to);
}

void newScene::PreCompute()
{
	if (!ObjList.size())
		
		return;

	SortObjects();

	/* for (std::list<newObject3D *>::iterator it = ObjList.begin(); it != ObjList.end();it++) {		MainConsole.Add(0,"Name: %s, Material: %s",(*it)->Name.c_str(), (*it)->Mtl->Name.c_str()); 	} */

	UINT i=0;

	newMaterial *mtl = NULL;

	newDisplayList *dl = NULL;

	for (std::list<newObject3D *>::iterator it = ObjList.begin(); it != ObjList.end();it++)
	{
		(*it)->PreCompute();

		if (!mtl)
		{
			dl = MainRender.GetNewDL();

			dl->mtl = (*it)->Mtl;

			mtl = dl->mtl;

			/* MainConsole.Add(0,"Start List..."); */

			mtl = (*it)->Mtl;
		}
        if (mtl != (*it)->Mtl)
		{
			if (dl != NULL)
			{
				dl->Compile();

				/* MainConsole.Add(0,"End List..."); */
			}

			dl = MainRender.GetNewDL();

			dl->mtl = (*it)->Mtl;

			mtl = dl->mtl;

			/* MainConsole.Add(0,"Start List..."); */
		}
		dl->objlist.push_back(&(*it)->RObj);
	}
	if (dl != NULL)
	{
		dl->Compile();

		/* MainConsole.Add(0,"End List..."); */
	}

	/* MainConsole.Add(0,"Display Lists: %d", MainRender.GetDisplayListCount()); */

	for(std::vector<newMaterial *>::iterator itr= MatList.begin(); itr<MatList.end(); itr++)
		
		(*itr)->Upload();
	
}

void newScene::Render()
{
	MainRender.RenderDisplayLists();

		return;

	for (std::list<newObject3D *>::iterator it = ObjList.begin(); it != ObjList.end();it++)
	{
		if ((*it)->Type != NEW_OBJ3D_DYNAMIC)
			
			continue;

		(*it)->Mtl->Apply();

		(*it)->Render();
	}
}
