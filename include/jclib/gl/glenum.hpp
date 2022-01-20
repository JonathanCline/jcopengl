#pragma once
#ifndef JCLIB_OPENGL_GLENUM_HPP
#define JCLIB_OPENGL_GLENUM_HPP

#include "gllib.hpp"

#include <jclib/type_traits.h>

#define _JCLIB_OPENGL_GLENUM_

#pragma region BASIC
namespace jc::gl
{
	/**
	 * @brief Enumerates the OpenGL integral type codes (ie. GL_FLOAT as a "typecode" for the floating point type)
	*/
	enum class typecode : GLenum
	{
		gl_float = GL_FLOAT,
		gl_double = GL_DOUBLE,

		gl_int = GL_INT,
		gl_unsigned_int = GL_UNSIGNED_INT,

		gl_short = GL_SHORT,
		gl_unsigned_short = GL_UNSIGNED_SHORT,

		gl_byte = GL_BYTE,
		gl_unsigned_byte = GL_UNSIGNED_BYTE,


		gl_int_vec2 = GL_INT_VEC2,
		gl_int_vec3 = GL_INT_VEC3,
		gl_int_vec4 = GL_INT_VEC4,

		gl_unsigned_int_vec2 = GL_UNSIGNED_INT_VEC2,
		gl_unsigned_int_vec3 = GL_UNSIGNED_INT_VEC3,
		gl_unsigned_int_vec4 = GL_UNSIGNED_INT_VEC4,

		gl_float_vec2 = GL_FLOAT_VEC2,
		gl_float_vec3 = GL_FLOAT_VEC3,
		gl_float_vec4 = GL_FLOAT_VEC4,

		gl_double_vec2 = GL_DOUBLE_VEC2,
		gl_double_vec3 = GL_DOUBLE_VEC3,
		gl_double_vec4 = GL_DOUBLE_VEC4,

		gl_float_mat2 = GL_FLOAT_MAT2,
		gl_float_mat3x2 = GL_FLOAT_MAT3x2,
		gl_float_mat2x3 = GL_FLOAT_MAT2x3,
		gl_float_mat3 = GL_FLOAT_MAT3,
		gl_float_mat3x4 = GL_FLOAT_MAT3x4,
		gl_float_mat4x3 = GL_FLOAT_MAT4x3,
		gl_float_mat2x4 = GL_FLOAT_MAT2x4,
		gl_float_mat4x2 = GL_FLOAT_MAT4x2,
		gl_float_mat4 = GL_FLOAT_MAT4,

		gl_double_mat2 = GL_DOUBLE_MAT2,
		gl_double_mat3x2 = GL_DOUBLE_MAT3x2,
		gl_double_mat2x3 = GL_DOUBLE_MAT2x3,
		gl_double_mat3 = GL_DOUBLE_MAT3,
		gl_double_mat3x4 = GL_DOUBLE_MAT3x4,
		gl_double_mat4x3 = GL_DOUBLE_MAT4x3,
		gl_double_mat2x4 = GL_DOUBLE_MAT2x4,
		gl_double_mat4x2 = GL_DOUBLE_MAT4x2,
		gl_double_mat4 = GL_DOUBLE_MAT4,

		gl_sampler_1D = GL_SAMPLER_1D,
		gl_sampler_2D = GL_SAMPLER_2D,
		gl_sampler_3D = GL_SAMPLER_3D,
		gl_sampler_1D_array = GL_SAMPLER_1D_ARRAY,
		gl_sampler_2D_array = GL_SAMPLER_2D_ARRAY,
	};

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
	 * @brief Bit masks for the buffers in a framebuffer
	*/
	enum class buffer_bit : GLbitfield
	{
		color = GL_COLOR_BUFFER_BIT,
		depth = GL_DEPTH_BUFFER_BIT,
		stencil = GL_STENCIL_BUFFER_BIT
	};
	constexpr inline buffer_bit operator|(const buffer_bit& lhs, const buffer_bit& rhs) noexcept
	{
		return static_cast<buffer_bit>(jc::to_underlying(lhs) | jc::to_underlying(rhs));
	};



};
#pragma endregion

#pragma region VBO

namespace jc::gl
{
	/**
	 * @brief Usage types used to hint to the GPU how a buffer will be used.
	*/
	enum class vbo_usage : GLenum
	{
		static_draw = GL_STATIC_DRAW,
		static_copy = GL_STATIC_COPY,
		static_read = GL_STATIC_READ,

		stream_draw = GL_STREAM_READ,
		stream_copy = GL_STREAM_COPY,
		stream_read = GL_STREAM_READ,

		dynamic_draw = GL_DYNAMIC_READ,
		dynamic_copy = GL_DYNAMIC_COPY,
		dynamic_read = GL_DYNAMIC_READ,
	};

	/**
	 * @brief Enumerates targets that a vbo can be bound to
	*/
	enum class vbo_target : GLenum
	{
#if defined(GL_ARRAY_BUFFER)
		array = GL_ARRAY_BUFFER,
#endif
#if defined(GL_ATOMIC_COUNTER_BUFFER)
		atomic_counter = GL_ATOMIC_COUNTER_BUFFER,
#endif
#if defined(GL_COPY_READ_BUFFER)
		copy_read = GL_COPY_READ_BUFFER,
#endif
#if defined(GL_COPY_WRITE_BUFFER)
		copy_write = GL_COPY_WRITE_BUFFER,
#endif
#if defined(GL_DISPATCH_INDIRECT_BUFFER)
		dispatch_indirect = GL_DISPATCH_INDIRECT_BUFFER,
#endif
#if defined(GL_DRAW_INDIRECT_BUFFER)
		draw_indirect = GL_DRAW_INDIRECT_BUFFER,
#endif
#if defined(GL_ELEMENT_ARRAY_BUFFER)
		element_array = GL_ELEMENT_ARRAY_BUFFER,
#endif
#if defined(GL_PIXEL_PACK_BUFFER)
		pixel_pack = GL_PIXEL_PACK_BUFFER,
#endif
#if defined(GL_PIXEL_UNPACK_BUFFER)
		pixel_unpack = GL_PIXEL_UNPACK_BUFFER,
#endif
#if defined(GL_QUERY_BUFFER)
		query = GL_QUERY_BUFFER,
#endif
#if defined(GL_SHADER_STORAGE_BUFFER)
		shader_storage = GL_SHADER_STORAGE_BUFFER,
#endif
#if defined(GL_TEXTURE_BUFFER)
		texture = GL_TEXTURE_BUFFER,
#endif
#if defined(GL_TRANSFORM_FEEDBACK_BUFFER)
		transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER,
#endif
#if defined(GL_UNIFORM_BUFFER)
		uniform = GL_UNIFORM_BUFFER,
#endif
	};

	/**
	 * @brief Parameters that can be queried and possible set for a vbo
	*/
	enum class vbo_parameter : GLenum
	{
	#if defined(GL_BUFFER_ACCESS)
		access = GL_BUFFER_ACCESS,
	#endif
	#if defined(GL_BUFFER_ACCESS_FLAGS)
		access_flags = GL_BUFFER_ACCESS_FLAGS,
	#endif
	#if defined(GL_BUFFER_IMMUTABLE_STORAGE)
		immutable_storage = GL_BUFFER_IMMUTABLE_STORAGE,
	#endif
	#if defined(GL_BUFFER_MAPPED)
		mapped = GL_BUFFER_MAPPED,
	#endif
	#if defined(GL_BUFFER_MAP_LENGTH)
		map_length = GL_BUFFER_MAP_LENGTH,
	#endif
	#if defined(GL_BUFFER_MAP_OFFSET)
		map_offset = GL_BUFFER_MAP_OFFSET,
	#endif
	#if defined(GL_BUFFER_SIZE)
		size = GL_BUFFER_SIZE,
	#endif
	#if defined(GL_BUFFER_STORAGE_FLAGS)
		storage_flags = GL_BUFFER_STORAGE_FLAGS,
	#endif
	#if defined(GL_BUFFER_USAGE)
		usage = GL_BUFFER_USAGE,
	#endif
	};

}

#pragma endregion

#pragma region VAO
namespace jc::gl
{
	/**
	 * @brief Primitives that can be drawn using vertex data
	*/
	enum class primitive : GLenum
	{
#if defined(GL_POINTS)
		points = GL_POINTS,
#endif
#if defined(GL_LINE_STRIP)
		line_strip = GL_LINE_STRIP,
#endif
#if defined(GL_LINE_LOOP)
		line_loop = GL_LINE_LOOP,
#endif
#if defined(GL_LINES)
		lines = GL_LINES,
#endif
#if defined(GL_LINE_STRIP_ADJACENCY)
		line_strip_adjacency = GL_LINE_STRIP_ADJACENCY,
#endif
#if defined(GL_LINES_ADJACENCY)
		lines_adjacency = GL_LINES_ADJACENCY,
#endif
#if defined(GL_TRIANGLE_STRIP)
		triangle_strip = GL_TRIANGLE_STRIP,
#endif
#if defined(GL_TRIANGLE_FAN)
		triangle_fan = GL_TRIANGLE_FAN,
#endif
#if defined(GL_TRIANGLES)
		triangles = GL_TRIANGLES,
#endif
#if defined(GL_TRIANGLE_STRIP_ADJACENCY)
		triangle_strip_adjacency = GL_TRIANGLE_STRIP_ADJACENCY,
#endif
#if defined(GL_TRIANGLES_ADJACENCY)
		triangles_adjacency = GL_TRIANGLES_ADJACENCY,
#endif
#if defined(GL_PATCHES)
		patches = GL_PATCHES,
#endif
	};

};
#pragma endregion

#pragma region SHADER
namespace jc::gl
{
	/**
	 * @brief Shader type values for OpenGL shader objects
	*/
	enum class shader_type : GLenum
	{
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
		geometry = GL_GEOMETRY_SHADER,
		compute = GL_COMPUTE_SHADER,
		tesselation_control = GL_TESS_CONTROL_SHADER,
		tesselation_evaluation = GL_TESS_EVALUATION_SHADER,
	};

	/**
	 * @brief Parameters that can be queried and possible set for a shader object
	*/
	enum class shader_parameter : GLenum
	{
		info_log_length = GL_INFO_LOG_LENGTH,
		compile_status = GL_COMPILE_STATUS,
		type = GL_SHADER_TYPE,
		source_length = GL_SHADER_SOURCE_LENGTH,
		delete_status = GL_DELETE_STATUS,
	};

	/**
	 * @brief Bit flags for denoting shader stages in a program pipeline
	*/
	enum class shader_stage_bit : GLbitfield
	{
		vertex = GL_VERTEX_SHADER_BIT,
		fragment = GL_FRAGMENT_SHADER_BIT,
		geometry = GL_GEOMETRY_SHADER_BIT,
		all = GL_ALL_SHADER_BITS,
	};
	constexpr shader_stage_bit operator|(shader_stage_bit lhs, shader_stage_bit rhs)
	{
		return static_cast<shader_stage_bit>(jc::to_underlying(lhs) | jc::to_underlying(rhs));
	};
	constexpr shader_stage_bit& operator|=(shader_stage_bit& lhs, shader_stage_bit rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	};

};
#pragma endregion

#pragma region PROGRAM
namespace jc::gl
{
	/**
	 * @brief The type that a program resource may be.
	*/
	enum class resource_type
	{
#if defined(GL_UNIFORM)
		uniform = GL_UNIFORM,
#endif
#if defined(GL_PROGRAM_INPUT)
		program_input = GL_PROGRAM_INPUT,
#endif
#if defined(GL_PROGRAM_OUTPUT)
		program_output = GL_PROGRAM_OUTPUT,
#endif
#if defined(GL_VERTEX_SUBROUTINE_UNIFORM)
		vertex_subroutine_uniform = GL_VERTEX_SUBROUTINE_UNIFORM,
#endif
#if defined(GL_TESS_CONTROL_SUBROUTINE_UNIFORM)
		tess_control_subroutine_uniform = GL_TESS_CONTROL_SUBROUTINE_UNIFORM,
#endif
#if defined(GL_TESS_EVALUATION_SUBROUTINE_UNIFORM)
		tess_evaluation_subroutine_uniform = GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
#endif
#if defined(GL_GEOMETRY_SUBROUTINE_UNIFORM)
		geometry_subroutine_uniform = GL_GEOMETRY_SUBROUTINE_UNIFORM,
#endif
#if defined(GL_FRAGMENT_SUBROUTINE_UNIFORM)
		fragment_subroutine_uniform = GL_FRAGMENT_SUBROUTINE_UNIFORM,
#endif
#if defined(GL_COMPUTE_SUBROUTINE_UNIFORM)
		compute_subroutine_uniform = GL_COMPUTE_SUBROUTINE_UNIFORM,
#endif
#if defined(GL_TRANSFORM_FEEDBACK_BUFFER)
		transform_feedback_buffer = GL_TRANSFORM_FEEDBACK_BUFFER,
#endif
#if defined(GL_UNIFORM_BLOCK)
		uniform_block = GL_UNIFORM_BLOCK,
#endif
	};

	/**
	 * @brief Parameters that can be queried and possibly set for a program object.
	*/
	enum class program_parameter : GLenum
	{
		active_attributes = GL_ACTIVE_ATTRIBUTES,
		active_attribute_max_length = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
		active_uniforms = GL_ACTIVE_UNIFORMS,
		active_uniform_blocks = GL_ACTIVE_UNIFORM_BLOCKS,
		active_uniform_block_max_name_length = GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
		active_uniform_max_length = GL_ACTIVE_UNIFORM_MAX_LENGTH,
		attached_shaders = GL_ATTACHED_SHADERS,
		binary_length = GL_PROGRAM_BINARY_LENGTH,
		delete_status = GL_DELETE_STATUS,
		info_log_length = GL_INFO_LOG_LENGTH,
		link_status = GL_LINK_STATUS,
		program_binary_retrievable_hint = GL_PROGRAM_BINARY_RETRIEVABLE_HINT,
		transform_feedback_buffer_mode = GL_TRANSFORM_FEEDBACK_BUFFER_MODE,
		transform_feedback_varyings = GL_TRANSFORM_FEEDBACK_VARYINGS,
		transform_feedback_varying_max_length = GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
		validate_status = GL_VALIDATE_STATUS,
	};

	/**
	 * @brief Interfaces of a program that can be queried.
	*/
	enum class program_interface : GLenum
	{
		active_resources = GL_ACTIVE_RESOURCES,
		max_name_length = GL_MAX_NAME_LENGTH,
		max_num_active_variables = GL_MAX_NUM_ACTIVE_VARIABLES,
		max_num_compatible_subroutines = GL_MAX_NUM_COMPATIBLE_SUBROUTINES,
	};

	/**
	 * @brief Parameters that can be queried for a program resource.
	*/
	enum class resource_parameter : GLenum
	{
		name_length = GL_NAME_LENGTH,
		type = GL_TYPE,
		array_size = GL_ARRAY_SIZE,
		offset = GL_OFFSET,
		block_index = GL_BLOCK_INDEX,
		array_stride = GL_ARRAY_STRIDE,
		matrix_stride = GL_MATRIX_STRIDE,
		is_row_major = GL_IS_ROW_MAJOR,
		atomic_counter_buffer_index = GL_ATOMIC_COUNTER_BUFFER_INDEX,
		texture_buffer = GL_TEXTURE_BUFFER,
		buffer_binding = GL_BUFFER_BINDING,
		buffer_data_size = GL_BUFFER_DATA_SIZE,
		num_active_variables = GL_NUM_ACTIVE_VARIABLES,
		active_variables = GL_ACTIVE_VARIABLES,
		referenced_by_vertex_shader = GL_REFERENCED_BY_VERTEX_SHADER,
		referenced_by_tess_control_shader = GL_REFERENCED_BY_TESS_CONTROL_SHADER,
		referenced_by_tess_evaluation_shader = GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
		referenced_by_geometry_shader = GL_REFERENCED_BY_GEOMETRY_SHADER,
		referenced_by_fragment_shader = GL_REFERENCED_BY_FRAGMENT_SHADER,
		referenced_by_compute_shader = GL_REFERENCED_BY_COMPUTE_SHADER,
		num_compatible_subroutines = GL_NUM_COMPATIBLE_SUBROUTINES,
		compatible_subroutines = GL_COMPATIBLE_SUBROUTINES,
		top_level_array_size = GL_TOP_LEVEL_ARRAY_SIZE,
		top_level_array_stride = GL_TOP_LEVEL_ARRAY_STRIDE,
		location = GL_LOCATION,
		location_index = GL_LOCATION_INDEX,
		is_per_patch = GL_IS_PER_PATCH,
		location_component = GL_LOCATION_COMPONENT,
		transform_feedback_buffer_index = GL_TRANSFORM_FEEDBACK_BUFFER_INDEX,
		transform_feedback_buffer_stride = GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE,
	};

};
#pragma endregion

#pragma region TEXTURE
namespace jc::gl
{
	/**
	 * @brief Targets that a texture can use or bind to
	*/
	enum class texture_target : GLenum
	{
		tex1D = GL_TEXTURE_1D,
		array1D = GL_TEXTURE_1D_ARRAY,
		tex2D = GL_TEXTURE_2D,
		array2D = GL_TEXTURE_2D_ARRAY,
		tex3D = GL_TEXTURE_3D,
		rectangle = GL_TEXTURE_RECTANGLE,
		cube_map = GL_TEXTURE_CUBE_MAP,
		cube_map_array = GL_TEXTURE_CUBE_MAP_ARRAY,
		buffer = GL_TEXTURE_BUFFER,
		multisample = GL_TEXTURE_2D_MULTISAMPLE,
		mutisample_array = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	/**
	 * @brief Enumeration of internal data formats
	*/
	enum class internal_format : GLenum
	{
		r8 = GL_R8,
		r16 = GL_R16,
		r16f = GL_R16F,
		r32f = GL_R32F,
		r8i = GL_R8I,
		r16i = GL_R16I,
		r32i = GL_R32I,
		r8ui = GL_R8UI,
		r16ui = GL_R16UI,
		r32ui = GL_R32UI,
		rg8 = GL_RG8,
		rg16 = GL_RG16,
		rg16f = GL_RG16F,
		rg32f = GL_RG32F,
		rg8i = GL_RG8I,
		rg16i = GL_RG16I,
		rg32i = GL_RG32I,
		rg8ui = GL_RG8UI,
		rg16ui = GL_RG16UI,

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

		rg32ui = GL_RG32UI,
		rgb32f = GL_RGB32F,
		rgb32i = GL_RGB32I,
		rgb32ui = GL_RGB32UI,
		rgba8 = GL_RGBA8,
		rgba16 = GL_RGBA16,
		rgba16f = GL_RGBA16F,
		rgba32f = GL_RGBA32F,
		rgba8i = GL_RGBA8I,
		rgba16i = GL_RGBA16I,
		rgba32i = GL_RGBA32I,
		rgba8ui = GL_RGBA8UI,
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

	/**
	 * @brief Typecodes that can be used for pixel data types
	*/
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

	/**
	 * 	@brief Parameters that can be queried and possible set for a texture object
	 */
	enum class texture_parameter : GLenum
	{
		depth_stencil_mode = GL_DEPTH_STENCIL_TEXTURE_MODE,
		image_format_compatibility_type = GL_IMAGE_FORMAT_COMPATIBILITY_TYPE,
		base_level = GL_TEXTURE_BASE_LEVEL,
		border_color = GL_TEXTURE_BORDER_COLOR,
		compare_mode = GL_TEXTURE_COMPARE_MODE,
		compare_func = GL_TEXTURE_COMPARE_FUNC,
		immutable_format = GL_TEXTURE_IMMUTABLE_FORMAT,
		immutable_levels = GL_TEXTURE_IMMUTABLE_LEVELS,
		lod_bias = GL_TEXTURE_LOD_BIAS,
		mag_filter = GL_TEXTURE_MAG_FILTER,
		max_level = GL_TEXTURE_MAX_LEVEL,
		max_lod = GL_TEXTURE_MAX_LOD,
		min_filter = GL_TEXTURE_MIN_FILTER,
		min_lod = GL_TEXTURE_MIN_LOD,
		swizzle_r = GL_TEXTURE_SWIZZLE_R,
		swizzle_g = GL_TEXTURE_SWIZZLE_G,
		swizzle_b = GL_TEXTURE_SWIZZLE_B,
		swizzle_a = GL_TEXTURE_SWIZZLE_A,
		swizzle_rgba = GL_TEXTURE_SWIZZLE_RGBA,
		target = GL_TEXTURE_TARGET,
		view_min_layer = GL_TEXTURE_VIEW_MIN_LAYER,
		view_min_level = GL_TEXTURE_VIEW_MIN_LEVEL,
		view_num_layers = GL_TEXTURE_VIEW_NUM_LAYERS,
		view_num_levels = GL_TEXTURE_VIEW_NUM_LEVELS,
		wrap_s = GL_TEXTURE_WRAP_S,
		wrap_t = GL_TEXTURE_WRAP_T,
		wrap_r = GL_TEXTURE_WRAP_R
	};
};
#pragma endregion

#endif