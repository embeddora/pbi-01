#include "Lightsystem.h"

#include "Render.h"

#include "Console.h"

newLightSystem LightSystem;

newLightSystem::newLightSystem(void)
{
}

newLightSystem::~newLightSystem(void)
{
}

void newLightSystem::Init()
{
	MainRender.InitLights();

	MainConsole.Add(0,"Система освещения инициализирована.");
}

newLight * newLightSystem::AddLight(newString Name, newEnum ltype)
{
	newLight *result = NULL;

	LightMan.newNewPItem(Name);

	result = MainRender.CreateLight(ltype);

	result->Name = Name;

	LightMan.Item[Name] = result;

	return result;
}

newLight * newLightSystem::GetLight(newString Name)
{
	if (LightMan.Item.find(Name) != LightMan.Item.end())
	{
		return LightMan.Item[Name];
	}
	else return NULL;
}
void newLightSystem::DeleteLight(newString Name)
{
	if ( LightMan.Item.find(Name) != LightMan.Item.end())
	{
		LightMan.Item.erase(Name);
	}
}