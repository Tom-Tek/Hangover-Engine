#pragma once

#include "Entity.h"

#include <EASTL/vector.h>

class Scene
{
public:
	eastl::vector<Entity*> entityTree;
	
};