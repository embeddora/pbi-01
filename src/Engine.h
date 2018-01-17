#ifndef __ENGINE_H__
#define __ENGINE_H__

/* Типы и константы */
#include "Types.h"

/* Рендер */
#include "Render.h"

/* Работа с окнами */
#include "Window.h"

/* Материалы */
#include "Material.h"

/* Консоль */
#include "Console.h"

#include "Camera.h"

#include "Scene.h"

#include "LightSystem.h"

/* Основной класс */
class Engine
{
public:
	Engine(void);

	~Engine(void);
};

#endif /* ifndef-define include guard */
