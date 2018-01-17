#include "Renderobject.h"

#include "Mesh.h"

#include "Render.h"

typedef std::map<UINT, UINT> newVertexTex;

typedef std::vector<newVertexTex> newVertexTexPerFace;

typedef newVertexTex::iterator newVertexTex_it;

typedef newVertexTexPerFace::iterator newVertexTexPerFace_it;

newRenderObject::newRenderObject(void)
{
}

newRenderObject::~newRenderObject(void)
{
}

void newRenderObject::Construct(const newMesh &m)
{
UINT i;

size_t wsize = m.Vertex.size();

	std::vector<newVertex> VertexTextured;

	VertexTextured = m.Vertex;

	std::vector<newFace> FaceTextured;

	FaceTextured = m.Face;

	newVertexTexPerFace VertexPerFace;

	VertexPerFace.resize(m.Vertex.size());

	size_t n = 0;

	newFace *f = &FaceTextured[0];

	for (n=0; n<FaceTextured.size();n++)
	{
	UINT *ver[3] = {&f->A, &f->B, &f->C};

		for (int d = 0; d<3; d++)
		{
			if (VertexPerFace[*ver[d]].size()>0)
			{
				if (VertexPerFace[*ver[d]].find(f->TV[d]) == VertexPerFace[*ver[d]].end())
				{
					newVertex temp = VertexTextured[*ver[d]];

					VertexTextured.push_back(temp);

					VertexPerFace[*ver[d]][f->TV[d]] = (UINT)VertexTextured.size()-1;
				}
				/*
				else
				{
					if (VertexPerFace[*ver[d]].find(f->TV[d]) == VertexPerFace[*ver[d]].end())
					{
						VertexPerFace[*ver[d]][f->TV[d]] = *ver[d];
					}
				}
				*/
				*ver[d] = VertexPerFace[*ver[d]][f->TV[d]];
			}
			else
			{
				VertexPerFace[*ver[d]][f->TV[d]] = *ver[d];
			}
		}
		f++;
	}


	size_t newsize = VertexTextured.size();

	UINT dif = newsize - wsize;
	
	RVertex.resize( VertexTextured.size() );

	RIndex.reserve( 3*FaceTextured.size() );

	UINT tvsize = m.TVertex.size();

	for(i=0;i<NEW_MAX(0, FaceTextured.size());i++)
	{
	newRenderVertex *rv;

	const newFace *f = &FaceTextured[i];

	const UINT *ver[]={&f->A, &f->B, &f->C};

		for (int j=0;j<3;j++)
		{
			rv = &RVertex[*ver[j]];

			rv->Coord = VertexTextured[*ver[j]].Coord;

			rv->Normal = VertexTextured[*ver[j]].Normal;

			rv->UV = m.TVertex[f->TV[j]].UV;

			RIndex.push_back(*ver[j]);
		}
		/*

		// A
		rv = &RVertex[f->A];
		rv->Coord = m.Vertex[f->A].Coord;
		rv->Normal = m.Vertex[f->A].Normal;
		rv->UV = m.TVertex[f->TV[0]].UV;
		RIndex.push_back(f->A);

		// B
		rv = &RVertex[f->B];
		rv->Coord = m.Vertex[f->B].Coord;
		rv->Normal = m.Vertex[f->B].Normal;
		rv->UV = m.TVertex[f->TV[1]].UV;
		RIndex.push_back(f->B);

		// C
		rv = &RVertex[f->C];
		rv->Coord = m.Vertex[f->C].Coord;
		rv->Normal = m.Vertex[f->C].Normal;
		rv->UV = m.TVertex[f->TV[2]].UV;
		RIndex.push_back(f->C);
		*/
	}

}
void newRenderObject::Render()
{
	MainRender.CallList(*this);

	MainRender.Tri_per_frame += (UINT)RIndex.size()/3;
}

void newRenderObject::TransformToStatic(newPhysicObject &po)
{
	for (std::vector<newRenderVertex>::iterator it = RVertex.begin(); it < RVertex.end(); it++)
	{
		newRenderVertex &v = *it;

		po.Rotation.Apply(v.Coord);

		v.Coord += po.Position;

		v.Color = newVector3f(1,1,1);

		po.Rotation.Apply(v.Normal);
	}
	MainRender.MakeList(*this);
}

void newRenderObject::DumpDraw()
{
	MainRender.DumpDraw(*this);
}