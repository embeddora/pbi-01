#include "Light.h"

newLight::newLight(void)
{
	PObj = PhysicObjectMan.newNewItem();

	InfiniteLight = FALSE;
}

newLight::~newLight(void)
{
	/* PhysicObjectMan.Release(PObj); */
}

void newLight::SetAmbientColor(newVector4f col)
{
	AmbientColor = col;
}
newVector4f newLight::GetAmbientColor()
{
	return AmbientColor;
}

void newLight::SetDiffuseColor(newVector4f col)
{
	DiffuseColor = col;
}
newVector4f newLight::GetDiffuseColor()
{
	return DiffuseColor;
}

void newLight::SetSpecularColor(newVector4f col)
{
	SpecularColor = col;
}
newVector4f newLight::GetSpecularColor()
{
	return SpecularColor;
}
void newLight::SetInfinite(BOOL inf)
{
	InfiniteLight = inf;
}
BOOL newLight::GetInfinite()
{
	return InfiniteLight;
}