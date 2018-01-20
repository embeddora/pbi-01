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

#include "Mesh.h"

#include "Types.h"

#include "File.h"

#include "Console.h"

#include "Render.h"

#include "AseFile.h"

newManager<newMesh> g_MeshMan;

Meshs &g_Mesh = g_MeshMan.Item;

typedef std::map<DWORD, UINT> newVertexSmooth;

typedef std::vector<newVertexSmooth> newVertexPerFace;

typedef newVertexSmooth::iterator newVertexSmooth_it;

typedef newVertexPerFace::iterator newVertexPerFace_it;

newMesh::newMesh(void)
{
	isLoaded = FALSE;

	PreComputed = FALSE;
}

newMesh::~newMesh(void)
{
}

newResult newMesh::LoadFromFile(const newString &FileName)
{
	if (g_Mesh.find(FileName)!=g_Mesh.end())
	{
		if (isLoaded) return GenError(NEW_EXIST,FileName.c_str());
	}
	
	newFile file;

	newResult res = file.Open(FileName);

	if (res != NEW_OK)
		
		return res;

	newString sbuf;

	sbuf = file.ReadString(3);

	if (!(sbuf == NEW_MESH_FILEID) )
		
		return GenError(NEW_UNKNOW_FILE_FORMAT, FileName.c_str());

//+++	__int16 tok;
	unsigned short tok;//+++
	
	file.Read(tok);

	if (tok != NEW_MESH_VERSION)
		
		return GenError(NEW_UNKNOW_FILE_FORMAT, FileName.c_str());

	Name = file.ReadString();

	UINT size;

	file.Read(size);

	Vertex.resize(size);

	file.Read(size);

	Face.resize(size);

	UINT i;

	newVertex *v = &Vertex[0];

	for (i=0; i<Vertex.size();i++)
	{
		file.Read( v->Coord );
		
		v++;
	}
	
	newFace *f = &Face[0];

	for (i=0; i<Face.size();i++)
	{
		file.Read(f->A);

		file.Read(f->B);

		file.Read(f->C);

		file.Read(f->MeshSmooth);

		file.Read(f->SubMtlID);

		f->Link[0]=-1;f->Link[1]=-1;f->Link[2]=-1;

		f++;
	}
	

	file.Read(tok);

	if (tok == NEW_MESH_TVERTEX) 
	{
		file.Read(size);

		TVertex.resize(size);

		newTVertex *v = &TVertex[0];

		for (i=0; i<TVertex.size();i++)
		{
			file.Read( v->UV );

			v++;
		}
		

		file.Read(size);

		TVFace.resize(size);

		newTVFace *tvf = &TVFace[0];

		newFace *f = &Face[0];

		for (i=0; i<TVFace.size();i++)
		{
			file.Read(tvf->A);

			file.Read(tvf->B);

			file.Read(tvf->C);

			f->TV[0] = tvf->A;

			f->TV[1] = tvf->B;
			
			f->TV[2] = tvf->C;

			tvf++;f++;
		}
	}

	/* Finiding neigbors of each triangle */
	unsigned int FA[3],GA[3];

	unsigned int p1i, p2i, p1j, p2j;

	unsigned int P1i, P2i, P1j, P2j;

	unsigned int ki,kj;

	f = &Face[0];

	for (i=0; i<Face.size()-1;i++)
	{
		FA[0] = f->A;

		FA[1] = f->B;

		FA[2] = f->C;

		newFace *g = &Face[i+1];

		for (UINT j=i+1; j<Face.size();j++)
		{
			if (f != g)
			{
				GA[0]=g->A;

				GA[1]=g->B;

				GA[2]=g->C;

				for(ki=0;ki<3;ki++)

					if (f->Link[ki]==-1)
					{
						for(kj=0;kj<3;kj++)
						{
							p1i=ki;

							p1j=kj;

							p2i=(ki+1)%3;

							p2j=(kj+1)%3;
							
							p1i=FA[p1i];

							p2i=FA[p2i];

							p1j=GA[p1j];

							p2j=GA[p2j];

							P1i=((p1i+p2i)-abs(p1i-p2i))/2;

							P2i=((p1i+p2i)+abs(p1i-p2i))/2;

							P1j=((p1j+p2j)-abs(p1j-p2j))/2;

							P2j=((p1j+p2j)+abs(p1j-p2j))/2;
							
							/* if (...) then they are neighbors */
							if((P1i==P1j) && (P2i==P2j))
							{
								f->Link[ki] = j;	  

								g->Link[kj] = i;	  
							}
						}
					}
			}
			g++;
		}
		f++;
	}

	newVertexPerFace VertexPerFace;

	VertexPerFace.resize(Vertex.size());

	size_t n = 0;

	f = &Face[0];

	for (n=0; n<Face.size();n++)
	{
		newVector3f P1 = Vertex[f->B].Coord - Vertex[f->A].Coord;

		newVector3f P2 = Vertex[f->C].Coord - Vertex[f->A].Coord;

		f->Normal = P1 ^ P2;

		f->Normal.Normalize();

		UINT *ver[3] = {&f->A, &f->B, &f->C};

		for (int d = 0; d<3; d++)
		{
			if (VertexPerFace[*ver[d]].size()>0)
			{
				
				DWORD mmm = 0, msafe = 0, mmax = 0;
				
				for (newVertexSmooth_it it = VertexPerFace[*ver[d]].begin(); it != VertexPerFace[*ver[d]].end(); it++)
				{
					DWORD ms = (*it).first;

					mmm = mmm |(ms & f->MeshSmooth);

					msafe = ms;

					if (mmm)
						
						break;
				}
				if (!(mmm))
				{
					newVertex temp = Vertex[*ver[d]];

					Vertex.push_back(temp);

					VertexPerFace[*ver[d]][f->MeshSmooth] = (UINT)Vertex.size()-1;

					msafe = f->MeshSmooth;
				}
				else
				{
					if (VertexPerFace[*ver[d]].find(f->MeshSmooth) == VertexPerFace[*ver[d]].end())
					{
						VertexPerFace[*ver[d]][f->MeshSmooth] = *ver[d];
					}
				}
				*ver[d] = VertexPerFace[*ver[d]][msafe];
			}
			else
			{
				VertexPerFace[*ver[d]][f->MeshSmooth] = *ver[d];
			}
		}
		f++;
	}

	v = &Vertex[0];

	UINT vsi = Vertex.size();

	for (UINT vi = 0; vi<Vertex.size();vi++)
	{
		newVector3f PreNormal(0,0,0);

		f = &Face[0];

		for (n=0; n<Face.size();n++)
		{
			UINT *ver[3] = {&f->A, &f->B, &f->C};

			for (int d = 0; d<3; d++)
			{
				if (vi == (*ver[d]) )
				{
					PreNormal += f->Normal;
				}
			}
			f++;
		}

		PreNormal.Normalize();

		v->Normal = PreNormal;

		v++;
	}

	isLoaded = TRUE;
	
	return NEW_OK;
}

newResult newMesh::LoadMeshFromAseFile(FILE * fpIn, char * MeshName)
{
MeshType Mesh;
FaceType AseFace;
int i;

	if (g_Mesh.find(MeshName)!=g_Mesh.end())
	{
		if (isLoaded) return GenError(NEW_EXIST, MeshName);
	}

	fscanf (fpIn, "\t*MESH {\n\t\t*TIMEVALUE %d\n", &(Mesh.iTimeValue));

	fscanf (fpIn, "\t\t*MESH_NUMVERTEX %d\n", &(Mesh.iNumVertex));

	Vertex.resize(Mesh.iNumVertex);

	fscanf (fpIn, "\t\t*MESH_NUMFACES %d\n", &(Mesh.iNumFaces));

	Face.resize(Mesh.iNumFaces);

	fscanf (fpIn, "\t\t*MESH_VERTEX_LIST {\n");

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t*MESH {\n\t\t*TIMEVALUE %d\n", Mesh.iTimeValue);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_NUMVERTEX %d\n", Mesh.iNumVertex);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_NUMFACES %d\n", Mesh.iNumFaces);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_VERTEX_LIST {\n");

	newVertex *v = &Vertex[0];

	for (i=0; i<Vertex.size();i++)
	{
	VertexType AseVertex;

		fscanf (fpIn, "\t\t\t*MESH_VERTEX\t%d\t%f\t%f\t%f\n", &(AseVertex.iIdx), &(AseVertex.cfCords[0]), &(AseVertex.cfCords[1]), &(AseVertex.cfCords[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t\t*MESH_VERTEX\t%d\t%f\t%f\t%f\n", (AseVertex.iIdx), (AseVertex.cfCords[0]), (AseVertex.cfCords[1]), (AseVertex.cfCords[2]) );

		v->Coord.Set((AseVertex.cfCords[0]), (AseVertex.cfCords[1]), (AseVertex.cfCords[2]));
		
		//file.Read( v->Coord );
		
		v++; /* put it into for (...!!!!) */
	}
	
	fscanf (fpIn, "\t\t}\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t}\n");

	/* of mesh vertex list */

	/* start of mesh face list */

	fscanf (fpIn, "\t\t*MESH_FACE_LIST {\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_FACE_LIST {\n");

	newFace *f = &Face[0];

	for (i=0; i<Face.size();i++)
	{
		fscanf (fpIn, "\t\t\t*MESH_FACE\t%d:\tA:\t%d\tB:\t%d\tC:\t%d\tAB:\t%d\tBC:\t%d\tCA:\t%d\t*MESH_SMOOTHING %d\t*MESH_MTLID %d\n", &(AseFace.iIdx), &(AseFace.iA), &(AseFace.iB), &(AseFace.iC), &(AseFace.iAB), &(AseFace.iBC), &(AseFace.iCA), &(AseFace.iSmoothing), &(AseFace.iMtLid) );

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t\t*MESH_FACE\t%d:\tA:\t%d\tB:\t%d\tC:\t%d\tAB:\t%d\tBC:\t%d\tCA:\t%d\t*MESH_SMOOTHING %d\t*MESH_MTLID %d\n", (AseFace.iIdx), (AseFace.iA), (AseFace.iB), (AseFace.iC), (AseFace.iAB), (AseFace.iBC), (AseFace.iCA), (AseFace.iSmoothing), (AseFace.iMtLid) );

		f->A = AseFace.iA;

		f->B = AseFace.iB;

		f->C = AseFace.iC;

		f->MeshSmooth = AseFace.iSmoothing;

		f->SubMtlID = AseFace.iMtLid;

		f->Link[0]=-1;f->Link[1]=-1;f->Link[2]=-1;

		f++; /* put it up into for (. . .) */
	}
	fscanf (fpIn, "\t\t}\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t}\n");

	/* end of mesh face list */


	{ /* start of mesh TVERTS amount + list */

		fscanf (fpIn, "\t\t*MESH_NUMTVERTEX %d\n", &(Mesh.iNumTvertex));


		TVertex.resize(Mesh.iNumTvertex);

		newTVertex *v = &TVertex[0];

		fscanf (fpIn, "\t\t*MESH_TVERTLIST {\n");

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_NUMTVERTEX %d\n", Mesh.iNumTvertex);
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_TVERTLIST {\n");		

		for (i=0; i<TVertex.size();i++)
		{		
		TvertexType Tvertex;

			fscanf (fpIn, "\t\t\t*MESH_TVERT %d\t%f\t%f\t%f\n", &(Tvertex.iIdx), &(Tvertex.cfCords[0]), &(Tvertex.cfCords[1]), &(Tvertex.cfCords[2]) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t\t*MESH_TVERT %d\t%f\t%f\t%f\n", (Tvertex.iIdx), (Tvertex.cfCords[0]), (Tvertex.cfCords[1]), (Tvertex.cfCords[2]) );

			/* OLD: //	file.Read( y->UV ); 	y->UV.Set(0,0,0); */
			// OLD but my: v->UV.Set(vectTemp.X, vectTemp.Y/*, vectTemp.Z*/); 
			v->UV.Set(Tvertex.cfCords[0], Tvertex.cfCords[1]); 

			v++; /* put it up .... */
		}
		fscanf (fpIn, "\t\t}\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t}\n");
	} /* end of mesh TVERTS list */


	{ /* start of mesh TFACES list */

		fscanf (fpIn, "\t\t*MESH_NUMTVFACES %d\n", &(Mesh.iNumTfaces));
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_NUMTVFACES %d\n", Mesh.iNumTfaces);

		TVFace.resize(Mesh.iNumTfaces);

		newTVFace *tvf = &TVFace[0];

		newFace *f = &Face[0];

		fscanf (fpIn, "\t\t*MESH_TFACELIST {\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t*MESH_TFACELIST {\n");		

		for (i=0; i<TVFace.size();i++)
		{
		TfaceType Tface;

			fscanf (fpIn, "\t\t\t*MESH_TFACE %d\t%d\t%d\t%d\n", &Tface.iIdx, &(Tface.iFirst), &(Tface.iSecond), &(Tface.iThird) );
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "\t\t\t*MESH_TFACE %d\t%d\t%d\t%d\n", Tface.iIdx, (Tface.iFirst), (Tface.iSecond), (Tface.iThird) ); 

			tvf->A = Tface.iFirst; tvf->B = Tface.iSecond; tvf->C = Tface.iThird;

			f->TV[0] = tvf->A; f->TV[1] = tvf->B; f->TV[2] = tvf->C;		

			tvf++;f++;		/* put them up */
		}
		fscanf (fpIn, "\t\t}\n");
MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL,"\t\t}\n");

	} /* end of mesh TFACES list */

	/* Finding neighbors of each triangle */
	unsigned int FA[3],GA[3];

	unsigned int p1i, p2i, p1j, p2j;

	unsigned int P1i, P2i, P1j, P2j;

	unsigned int ki,kj;

	f = &Face[0];

	for (i=0; i<Face.size()-1;i++)
	{
		FA[0] = f->A;

		FA[1] = f->B;

		FA[2] = f->C;

		newFace *g = &Face[i+1];

		for (UINT j=i+1; j<Face.size();j++)
		{
			if (f != g)
			{
				GA[0]=g->A;

				GA[1]=g->B;

				GA[2]=g->C;

				for(ki=0;ki<3;ki++)

					if (f->Link[ki]==-1)
					{
						for(kj=0;kj<3;kj++)
						{
							p1i=ki;

							p1j=kj;

							p2i=(ki+1)%3;

							p2j=(kj+1)%3;
							
							p1i=FA[p1i];

							p2i=FA[p2i];

							p1j=GA[p1j];

							p2j=GA[p2j];

							P1i=((p1i+p2i)-abs(p1i-p2i))/2;

							P2i=((p1i+p2i)+abs(p1i-p2i))/2;

							P1j=((p1j+p2j)-abs(p1j-p2j))/2;

							P2j=((p1j+p2j)+abs(p1j-p2j))/2;
							
							/* if ((...) && (...)) then they are neighbors */
							if((P1i==P1j) && (P2i==P2j))
							{
								f->Link[ki] = j;	  

								g->Link[kj] = i;	  
							}
						}
					}
			}
			g++;
		}
		f++;
	}

	newVertexPerFace VertexPerFace;

	VertexPerFace.resize(Vertex.size());

	size_t n = 0;

	f = &Face[0];

	for (n=0; n<Face.size();n++)
	{
		newVector3f P1 = Vertex[f->B].Coord - Vertex[f->A].Coord;

		newVector3f P2 = Vertex[f->C].Coord - Vertex[f->A].Coord;

		f->Normal = P1 ^ P2;

		f->Normal.Normalize();

		UINT *ver[3] = {&f->A, &f->B, &f->C};

		for (int d = 0; d<3; d++)
		{
			if (VertexPerFace[*ver[d]].size()>0)
			{
				
				DWORD mmm = 0, msafe = 0, mmax = 0;
				
				for (newVertexSmooth_it it = VertexPerFace[*ver[d]].begin(); it != VertexPerFace[*ver[d]].end(); it++)
				{
					DWORD ms = (*it).first;

					mmm = mmm |(ms & f->MeshSmooth);

					msafe = ms;

					if (mmm)
						
						break;
				}
				if (!(mmm))
				{
					newVertex temp = Vertex[*ver[d]];

					Vertex.push_back(temp);

					VertexPerFace[*ver[d]][f->MeshSmooth] = (UINT)Vertex.size()-1;

					msafe = f->MeshSmooth;
				}
				else
				{
					if (VertexPerFace[*ver[d]].find(f->MeshSmooth) == VertexPerFace[*ver[d]].end())
					{
						VertexPerFace[*ver[d]][f->MeshSmooth] = *ver[d];
					}
				}
				*ver[d] = VertexPerFace[*ver[d]][msafe];
			}
			else
			{
				VertexPerFace[*ver[d]][f->MeshSmooth] = *ver[d];
			}
		}
		f++;
	}

	v = &Vertex[0];

	UINT vsi = Vertex.size();

	for (UINT vi = 0; vi<Vertex.size();vi++)
	{
		newVector3f PreNormal(0,0,0);

		f = &Face[0];

		for (n=0; n<Face.size();n++)
		{
			UINT *ver[3] = {&f->A, &f->B, &f->C};

			for (int d = 0; d<3; d++)
			{
				if (vi == (*ver[d]) )
				{
					PreNormal += f->Normal;
				}
			}
			f++;
		}

		PreNormal.Normalize();

		v->Normal = PreNormal;

		v++;
	}

	isLoaded = TRUE;
	
	return NEW_OK;
}


void newMesh::PreCompute()
{
	if (PreComputed)
		
		return;

	/*RObj.Construct(*this); */

	/*MainConsole.Add(0,"%s",Name.c_str()); */

	PreComputed = TRUE;
}

void newMesh::Render()
{
	/*	RObj.Render(); */
}

