#pragma once

#include <GL/glew.h>

#include <mage3d_exported.h>

class Test
{
public:
	mage3d_EXPORT Test();
	virtual mage3d_EXPORT GLenum glewTest(int test);
};
