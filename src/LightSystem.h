#ifndef __LIGHTSYSTEM_H__
#define __LIGHTSYSTEM_H__

#include "Types.h"

#include "Light.h"

class newLightSystem
{
public:

	newLightSystem(void);

	~newLightSystem(void);

	void Init();

	newLight * AddLight(newString Name, newEnum ltype = NEW_LIGHT_AUTODETECT);

	newLight * GetLight(newString Name);

	void DeleteLight(newString Name);

	newManager<newLight *> LightMan;
};

extern newLightSystem LightSystem;
