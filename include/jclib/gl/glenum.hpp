#pragma once
#ifndef JCLIB_OPENGL_GLENUM_HPP
#define JCLIB_OPENGL_GLENUM_HPP

#include "gl/glsym.h"

#define _JCLIB_OPENGL_GLENUM_

namespace jc::gl
{
	/**
	 * @brief Enumeration of (supported) OpenGL object types
	*/
	enum class object_type : GLenum
	{
		program = GL_PROGRAM,
		shader = GL_SHADER,
		vao = GL_VERTEX_ARRAY,
		vbo = GL_BUFFER,
		program_pipeline = GL_PROGRAM_PIPELINE,
		texture = GL_TEXTURE,
	};

	/**
	 * @brief Enumeration of internal data formats
	*/
	enum class internal_format : GLenum
	{
		r8 = GL_R8,
		r16 = GL_R16,
		r16f = GL_R16F 	,
		r32f = GL_R32F 	,
		r8i = GL_R8I,
		r16i = GL_R16I 	,
		r32i = GL_R32I 	,
		r8ui = GL_R8UI 	,
		r16ui = GL_R16UI 	,
		r32ui = GL_R32UI 	,
		rg8 = GL_RG8,
		rg16 = GL_RG16 	,
		rg16f = GL_RG16F 	,
		rg32f = GL_RG32F 	,
		rg8i = GL_RG8I 	,
		rg16i = GL_RG16I 	,
		rg32i = GL_RG32I 	,
		rg8ui = GL_RG8UI 	,
		rg16ui = GL_RG16UI ,

		rgb4 = GL_RGB4,
		rgb5 = GL_RGB5,

		rgb8 = GL_RGB8,
		rgb8i = GL_RGB8I,
		rgb8ui = GL_RGB8UI,
		rgb8_snorm = GL_RGB8_SNORM,

		rgb16 = GL_RGB16,
		rgb16f = GL_RGB16F,
		rgb16i = GL_RGB16I,
		rgb16ui = GL_RGB16UI,
		rgb16_snorm = GL_RGB16_SNORM,

		rg32ui = GL_RG32UI ,
		rgb32f = GL_RGB32F ,
		rgb32i = GL_RGB32I ,
		rgb32ui = GL_RGB32UI ,
		rgba8 = GL_RGBA8 	,
		rgba16 = GL_RGBA16 ,
		rgba16f = GL_RGBA16F ,
		rgba32f = GL_RGBA32F ,
		rgba8i = GL_RGBA8I ,
		rgba16i = GL_RGBA16I ,
		rgba32i = GL_RGBA32I ,
		rgba8ui = GL_RGBA8UI ,
		rgba16ui = GL_RGBA16UI,
	};

	/**
	 * @brief Base OpenGL data formats
	*/
	enum class format : GLenum
	{
		red = GL_RED,
		rg = GL_RG,
		rgb = GL_RGB,
		bgr = GL_BGR,
		rgba = GL_RGBA,
		bgra = GL_BGRA,
		red_integer = GL_RED_INTEGER,
		rg_integer = GL_RG_INTEGER,
		rgb_integer = GL_RGB_INTEGER,
		bgr_integer = GL_BGR_INTEGER,
		rgba_integer = GL_RGBA_INTEGER,
		bgra_integer = GL_BGRA_INTEGER,
		stencil_index = GL_STENCIL_INDEX,
		depth_component = GL_DEPTH_COMPONENT,
		depth_stencil = GL_DEPTH_STENCIL,
	};

	enum class pixel_typecode : GLenum
	{
		gl_unsigned_byte = GL_UNSIGNED_BYTE,
		gl_byte = GL_BYTE,
		gl_unsigned_short = GL_UNSIGNED_SHORT,
		gl_short = GL_SHORT,
		gl_unsigned_int = GL_UNSIGNED_INT,
		gl_int = GL_INT,
		gl_float = GL_FLOAT,
		gl_unsigned_byte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
		gl_unsigned_byte_2_3_3_rev = GL_UNSIGNED_BYTE_2_3_3_REV,
		gl_unsigned_short_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
		gl_unsigned_short_5_6_5_rev = GL_UNSIGNED_SHORT_5_6_5_REV,
		gl_unsigned_short_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
		gl_unsigned_short_4_4_4_4_rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
		gl_unsigned_short_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
		gl_unsigned_short_1_5_5_5_rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
		gl_unsigned_int_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
		gl_unsigned_int_8_8_8_8_rev = GL_UNSIGNED_INT_8_8_8_8_REV,
		gl_unsigned_int_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
		gl_unsigned_int_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
	};

};

#endif