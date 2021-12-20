#pragma once

/*
	Parameter query shorthand functions
*/

#include "gl.hpp"
#include "glenum.hpp"

#include <jclib/concepts.h>

#include <array>

namespace jc::gl
{

	inline GLint get_max_uniform_locations()
	{
		GLint _v{};
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &_v);
		return _v;
	};

	/**
	 * @brief Gets the target a texture is assigned to
	 * 
	 * @param _texture Texture ID
	 * @return Texture target value
	*/
	inline texture_target get_texture_target(texture_id _id)
	{
		const auto _value = get<GLint>(_id, texture_parameter::target);
		return texture_target(_value);
	};

};
