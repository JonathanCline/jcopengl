#pragma once

/*
	Parameter query shorthand functions
*/

#include "gl.hpp"

namespace jc::gl
{
	inline GLint get_max_uniform_locations()
	{
		GLint _v{};
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &_v);
		return _v;
	};
};
