#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Types.h"

#include "PhysicObject.h"

class newLight: public newBaseObject
{
public:
	
	newLight(void);

	virtual ~newLight(void) = 0;

	virtual void Enable() = 0;

	virtual void Disable() = 0;

	virtual void Apply() = 0;

	void SetPosition(newVector3f pos);

	newVector3f GetPosition();

	void SetDiffuseColor(newVector4f col);

	newVector4f GetDiffuseColor();

	void SetAmbientColor(newVector4f col);

	newVector4f GetAmbientColor();

	void SetSpecularColor(newVector4f col);

	newVector4f GetSpecularColor();

	void SetInfinite(BOOL inf);

	BOOL GetInfinite();

	virtual void SetAttenuation(newEnum att_type, float att) = 0;
	
/* private: */
	newPhysicObject *PObj;

	newVector4f DiffuseColor;

	newVector4f AmbientColor;

	newVector4f SpecularColor;

	BOOL InfiniteLight;

	float Attenuation;
};

#endif /* ifndef-define include guard */
