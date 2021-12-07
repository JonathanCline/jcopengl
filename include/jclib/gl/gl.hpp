#pragma once
#ifndef JCLIB_OPENGL_GL_HPP
#define JCLIB_OPENGL_GL_HPP

#include "gl/glsym.h"
#include "glenum.hpp"

#include <jclib/type.h>
#include <jclib/optional.h>
#include <jclib/algorithm.h>

#define _JCLIB_OPENGL_GL_ 

#include <span>
#include <format>
#include <compare>
#include <iostream>
#include <charconv>

namespace jc::gl
{

	/**
	 * @brief Traits type for describing how OpenGL objects can be interacted with
	*/
	template <object_type Type>
	struct object_traits;



	template <object_type Type>
	struct object_id
	{
	public:
		using traits_type = object_traits<Type>;
		using value_type = typename traits_type::value_type;

	private:

		constexpr value_type& value() noexcept { return this->value_; };
		constexpr const value_type& value() const noexcept { return this->value_; };

	public:

		constexpr value_type get() const noexcept { return this->value(); };
		constexpr explicit operator value_type() const noexcept { return this->get(); };

		constexpr bool good() const noexcept
		{
			return this->get() != traits_type::null();
		};
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};

		friend constexpr inline auto operator<=>(const object_id& lhs, const object_id& rhs) noexcept = default;

		constexpr object_id() = default;
		constexpr explicit object_id(value_type _value) :
			value_{ _value }
		{
#if	JCLIB_DEBUG_V
			if (this->good())
			{
				if (!traits_type::check(this->get()))
				{
					JCLIB_ABORT();
				};
			};
#endif
		};

		constexpr object_id(jc::null_t) noexcept :
			value_{ traits_type::null() }
		{};
		constexpr object_id& operator=(jc::null_t) noexcept
		{
			this->value() = traits_type::null();
			return *this;
		};

	private:
		value_type value_;
	};



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

	template <>
	struct object_traits<object_type::shader>
	{
		using value_type = ::GLuint;

		static value_type create(shader_type _type)
		{
			return glCreateShader(jc::to_underlying(_type));
		};
		static void destroy(value_type _value)
		{
			glDeleteShader(_value);
		};
		static bool check(const value_type& _value)
		{
			return glIsShader(_value);
		};
		constexpr static value_type null()
		{
			return value_type{ 0 };
		};
	};
	using shader_traits = object_traits<object_type::shader>;

	template <>
	struct object_traits<object_type::program>
	{
		using value_type = GLuint;
		static value_type create()
		{
			return glCreateProgram();
		};
		static void destroy(value_type _value)
		{
			glDeleteProgram(_value);
			_value = 0;
		};
		static bool check(const value_type& _value)
		{
			return glIsProgram(_value);
		};
		constexpr static value_type null()
		{
			return value_type{ 0 };
		};

		static void bind(const value_type& _value)
		{
			glUseProgram(_value);
		};
		static bool is_bound(const value_type& _value)
		{
			GLint _active;
			glGetIntegerv(GL_CURRENT_PROGRAM, &_active);
			return static_cast<GLint>(_value) == _active;
		};
	};
	using program_traits = object_traits<object_type::program>;

	template <>
	struct object_traits<object_type::vao>
	{
		using value_type = GLuint;
		static value_type create()
		{
			value_type _value;
			glCreateVertexArrays(1, &_value);
			return _value;
		};
		static void destroy(value_type _value)
		{
			glDeleteVertexArrays(1, &_value);
			_value = 0;
		};
		static bool check(const value_type& _value)
		{
			return glIsVertexArray(_value);
		};
		constexpr static value_type null()
		{
			return value_type{ 0 };
		};

		static void bind(const value_type& _value)
		{
			glBindVertexArray(_value);
		};
		static bool is_bound(const value_type& _value)
		{
			GLint _active;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &_active);
			return static_cast<GLint>(_value) == _active;
		};
	};
	using vao_traits = object_traits<object_type::vao>;

	template <>
	struct object_traits<object_type::program_pipeline>
	{
		using value_type = GLuint;
		static value_type create()
		{
			value_type _out;
			glCreateProgramPipelines(1, &_out);
			return _out;
		};
		static void destroy(value_type _value)
		{
			glDeleteProgramPipelines(1, &_value);
			_value = 0;
		};
		static bool check(const value_type& _value)
		{
			return glIsProgramPipeline(_value);
		};
		constexpr static value_type null()
		{
			return value_type{ 0 };
		};

		static void bind(const value_type& _value)
		{
			glUseProgram(0);
			glBindProgramPipeline(_value);
		};
		static bool is_bound(const value_type& _value)
		{
			GLint _active;
			glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &_active);
			return static_cast<GLint>(_value) == _active;
		};
	};
	using program_pipeline_traits = object_traits<object_type::program_pipeline>;

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
	 * @brief Traits type for OpenGL texture objects
	*/
	template <>
	struct object_traits<object_type::texture>
	{
		using value_type = GLuint;

		/**
		 * @brief Creates a new texture
		 * @param _target Target to create texture with
		 * @return Vaild owning texture ID
		*/
		JCLIB_NODISCARD("owning ID") static value_type create(texture_target _target)
		{
			value_type _out;
			glCreateTextures(jc::to_underlying(_target), 1, &_out);
			return _out;
		};
		static void destroy(value_type _value)
		{
			glDeleteTextures(1, &_value);
		};
		static bool check(const value_type& _value)
		{
			return glIsTexture(_value);
		};
		constexpr static value_type null()
		{
			return value_type{ 0 };
		};

		static void bind(const value_type& _value, const texture_target& _target)
		{
			glBindTexture(jc::to_underlying(_target), _value);
		};
	};

	/**
	 * @brief Traits type for OpenGL texture objects
	*/
	using texture_traits = object_traits<object_type::texture>;


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


	template <>
	struct object_traits<object_type::vbo>
	{
		using value_type = GLuint;

		static value_type create()
		{
			value_type _value;
			glCreateBuffers(1, &_value);
			return _value;
		};
		static void destroy(value_type _value)
		{
			glDeleteBuffers(1, &_value);
			_value = 0;
		};
		static bool check(const value_type& _value)
		{
			return glIsBuffer(_value);
		};
		constexpr static value_type null()
		{
			return value_type{ 0 };
		};

		static void bind(const value_type& _value, vbo_target _target)
		{
			glBindBuffer(jc::to_underlying(_target), _value);
		};
	};
	using vbo_traits = object_traits<object_type::vbo>;


	/**
	 * @brief Invariant for storing OpenGL shader object IDs
	*/
	using shader_id = object_id<object_type::shader>;

	/**
	 * @brief Invariant for storing OpenGL program object IDs
	*/
	using program_id = object_id<object_type::program>;

	/**
	 * @brief Invariant for storing OpenGL vertex array object (VAO) IDs
	*/
	using vao_id = object_id<object_type::vao>;

	/**
	 * @brief Invariant for storing OpenGL vertex buffer object (VBO) IDs
	*/
	using vbo_id = object_id<object_type::vbo>;

	/**
	 * @brief Invariant for storing OpenGL program pipeline object IDs
	*/
	using program_pipeline_id = object_id<object_type::program_pipeline>;
	
	/**
	 * @brief Invariant for storing OpenGL texture object IDs
	*/
	using texture_id = object_id<object_type::texture>;


	template <object_type Type, typename... ArgTs> requires requires(ArgTs&&... _args)
	{
		object_id<Type>{ object_traits<Type>::create(std::forward<ArgTs>(_args)...) };
	}
	JCLIB_NODISCARD("raw owning object id") inline object_id<Type> create(ArgTs&&... _args)
	{
		return object_id<Type>{ object_traits<Type>::create(std::forward<ArgTs>(_args)...) };
	};

	template <object_type Type> requires requires(typename object_id<Type>::value_type v)
	{
		object_traits<Type>::destroy(v);
	}
	inline void destroy(object_id<Type>& _value)
	{
		object_traits<Type>::destroy(_value.get());
		_value = jc::null;
	};

	template <object_type Type> requires requires(typename object_id<Type>::value_type v)
	{
		{ object_traits<Type>::check(v) } -> jc::cx_same_as<bool>;
	}
	inline bool check(const object_id<Type>& _value)
	{
		return object_traits<Type>::check(_value.get());
	};



	/**
	 * @brief Owning RAII OpenGL object id wrapper.
	 *
	 * This can only be used for objects with lifetime functionality implemented
	 * by their associated object_traits type. As of 11/24/2021 this requires that
	 * a function named "destroy" be defined and match the following signature:
	 *
	 *			void object_traits<Type>::destroy(typename object_traits<Type>::value_type);
	 *
	 * and a function named "check" matching the following signature:
	 *
	 * 			bool object_traits<Type>::check(typename object_traits<Type>::value_type);
	 *
	*/
	template <object_type Type> requires requires (typename object_traits<Type>::value_type _value)
	{
		{ object_traits<Type>::destroy(_value) } -> jc::cx_same_as<void>;
		{ object_traits<Type>::check(_value) } -> jc::cx_convertible_to<bool>;
	}
	struct unique_object
	{
	public:

		using id_type = object_id<Type>;

		using value_type = typename id_type::value_type;
		using traits_type = typename id_type::traits_type;

		constexpr id_type id() const& noexcept
		{
			return this->id_;
		};
		constexpr operator id_type() const& noexcept
		{
			return this->id();
		};

		id_type id() && = delete;
		operator id_type() && = delete;


		constexpr value_type get() const& noexcept
		{
			return this->id().get();
		};
		value_type get() && = delete;



		constexpr bool good() const noexcept
		{
			return this->id().good();
		};
		constexpr explicit operator bool() const noexcept
		{
			return this->good();
		};

		friend constexpr inline auto operator<=>(const unique_object& lhs, const unique_object& rhs) noexcept = default;
		friend constexpr inline auto operator<=>(const unique_object& lhs, const id_type& rhs) noexcept
		{
			return lhs.id() <=> rhs;
		};
		friend constexpr inline auto operator<=>(const id_type& lhs, const unique_object& rhs) noexcept
		{
			return lhs <=> rhs.id();
		};

		constexpr void release() noexcept
		{
			this->id_ = jc::null;
		};
		JCLIB_NODISCARD("raw owning object id") constexpr id_type extract() noexcept
		{
			const auto _out = this->id();
			this->release();
			return _out;
		};

		void reset() noexcept
		{
			gl::destroy(this->id_);
			JCLIB_ASSERT(!this->good());
		};



		constexpr unique_object() = default;
		constexpr explicit unique_object(id_type _id) :
			id_{ _id }
		{
			JCLIB_ASSERT(!_id.good() || traits_type::check(_id.get()));
		};

		constexpr unique_object(jc::null_t) noexcept :
			id_{ jc::null }
		{};
		unique_object& operator=(jc::null_t) noexcept
		{
			this->reset();
			return *this;
		};

		constexpr unique_object(unique_object&& _other) noexcept :
			id_{ _other.extract() }
		{};
		unique_object& operator=(unique_object&& _other) noexcept
		{
			this->reset();
			this->id_ = _other.extract();
			return *this;
		};

	private:
		id_type id_{};

		unique_object(const unique_object&) = delete;
		unique_object& operator=(const unique_object&) = delete;

	};


	using unique_shader = unique_object<object_type::shader>;
	using unique_program = unique_object<object_type::program>;
	using unique_vao = unique_object<object_type::vao>;
	using unique_vbo = unique_object<object_type::vbo>;
	using unique_program_pipeline = unique_object<object_type::program_pipeline>;

	/**
	 * @brief Owning RAII handle to an OpenGL texture object
	*/
	using unique_texture = unique_object<object_type::texture>;



	template <object_type Type>
	inline void destroy(unique_object<Type>& _value)
	{
		_value.reset();
	};



	template <object_type Type, typename... ArgTs> requires requires (object_id<Type> _id, ArgTs&&... _args)
	{
		object_id<Type>::traits_type::bind(_id.get(), std::forward<ArgTs>(_args)...);
	}
	inline void bind(const object_id<Type>& _id, ArgTs&&... _args)
	{
		object_id<Type>::traits_type::bind(_id.get(), std::forward<ArgTs>(_args)...);
	};

	template <object_type Type, typename... ArgTs>
	inline auto bind(const unique_object<Type>& _obj, ArgTs&&... _args) ->
		decltype(bind(_obj.id(), std::forward<ArgTs>(_args)...))
	{
		return bind(_obj.id(), std::forward<ArgTs>(_args)...);
	};




	template <std::ranges::contiguous_range RangeT>
	inline void buffer_data(const vbo_id& _vbo, const RangeT& _data, vbo_usage _usage = vbo_usage::static_draw)
	{
		glNamedBufferData
		(
			_vbo.get(),
			std::ranges::size(_data) * sizeof(jc::ranges::value_t<RangeT>),
			std::ranges::data(_data),
			jc::to_underlying(_usage)
		);
	};





	namespace gl_impl
	{
		template <typename T, typename TagT>
		struct integer_invariant
		{
		public:
			using value_type = T;

			friend constexpr inline auto operator<=>(const integer_invariant& lhs, const integer_invariant& rhs) noexcept = default;

			constexpr value_type get() const noexcept { return this->value_; };
			constexpr explicit operator value_type() const noexcept { return this->get(); };

			constexpr integer_invariant() noexcept = default;
			constexpr explicit integer_invariant(const value_type& _value) noexcept :
				value_{ _value }
			{};

		private:
			value_type value_;
		};

	};






#if GL_VERSION_4_3

	using resource_location = gl_impl::integer_invariant<GLuint, struct resource_location_tag>;
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

	inline jc::optional<resource_location> get_program_resource_location(const program_id& _program, resource_type _resourceType, const GLchar* _name)
	{
		JCLIB_ASSERT(_name != null);
		const auto _location = glGetProgramResourceLocation(_program.get(), jc::to_underlying(_resourceType), _name);
		if (_location != -1) JCLIB_LIKELY
		{
			return resource_location{ static_cast<resource_location::value_type>(_location) };
		}
		else
		{
			return std::nullopt;
		};
	};

#endif


	using vertex_binding_index = gl_impl::integer_invariant<GLuint, struct vertex_binding_index_tag>;
	struct vertex_attribute_index : public gl_impl::integer_invariant<GLuint, struct vertex_attribute_index_tag>
	{
	private:
		using parent_type = gl_impl::integer_invariant<GLuint, struct vertex_attribute_index_tag>;
	public:

		/**
		 * @brief Constructs the attribute index using a program resource location
		 * @param _location Attribute location as a program resource
		*/
		constexpr vertex_attribute_index(const resource_location _location) :
			parent_type{ _location.get() }
		{};

		using parent_type::parent_type;
		using parent_type::operator=;
	};

	/**
	 * @brief Integer invariant for holding program uniform locations
	*/
	struct uniform_location : public gl_impl::integer_invariant<GLuint, struct uniform_location_tag>
	{
	private:
		using parent_type = gl_impl::integer_invariant<GLuint, struct uniform_location_tag>;
	public:

		/**
		 * @brief Constructs the attribute index using a program resource location
		 * @param _location Attribute location as a program resource
		*/
		constexpr uniform_location(const resource_location _location) :
			parent_type{ _location.get() }
		{};

		using parent_type::parent_type;
		using parent_type::operator=;
	};


	inline void bind_vertex_buffer(vertex_binding_index _buffer, const vbo_id& _vbo, GLintptr _offsetBytes, GLsizei _strideBytes)
	{
		glBindVertexBuffer(_buffer.get(), _vbo.get(), _offsetBytes, _strideBytes);
	};

	inline void set_vertex_divisor(vertex_binding_index _buffer, GLuint _divisor)
	{
		glVertexBindingDivisor(_buffer.get(), _divisor);
	};
	inline void set_vertex_divisor(vertex_attribute_index _attribute, GLuint _divisor)
	{
		glVertexAttribDivisor(_attribute.get(), _divisor);
	};




	inline void enable_attribute_array(vertex_attribute_index _index)
	{
		glEnableVertexAttribArray(_index.get());
	};


	/**
	 * @brief Enumerates the opengl type codes (ie. GL_FLOAT as a "typecode" for the floating point type)
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

		gl_float_mat2	= GL_FLOAT_MAT2,
		gl_float_mat3x2 = GL_FLOAT_MAT3x2,
		gl_float_mat2x3 = GL_FLOAT_MAT2x3,
		gl_float_mat3	= GL_FLOAT_MAT3,
		gl_float_mat3x4 = GL_FLOAT_MAT3x4,
		gl_float_mat4x3 = GL_FLOAT_MAT4x3,
		gl_float_mat2x4 = GL_FLOAT_MAT2x4,
		gl_float_mat4x2 = GL_FLOAT_MAT4x2,
		gl_float_mat4	= GL_FLOAT_MAT4,

		gl_double_mat2	= GL_DOUBLE_MAT2,
		gl_double_mat3x2 = GL_DOUBLE_MAT3x2,
		gl_double_mat2x3 = GL_DOUBLE_MAT2x3,
		gl_double_mat3	= GL_DOUBLE_MAT3,
		gl_double_mat3x4 = GL_DOUBLE_MAT3x4,
		gl_double_mat4x3 = GL_DOUBLE_MAT4x3,
		gl_double_mat2x4 = GL_DOUBLE_MAT2x4,
		gl_double_mat4x2 = GL_DOUBLE_MAT4x2,
		gl_double_mat4	= GL_DOUBLE_MAT4,

		gl_sampler_1D = GL_SAMPLER_1D,
		gl_sampler_2D = GL_SAMPLER_2D,
		gl_sampler_3D = GL_SAMPLER_3D,
		gl_sampler_1D_array = GL_SAMPLER_1D_ARRAY,
		gl_sampler_2D_array = GL_SAMPLER_2D_ARRAY,
	};

	/**
	 * @brief Gets the name string of an opengl type code
	 * @param _type Opengl type code
	 * @return Name string
	*/
	constexpr inline const char* get_typename(typecode _type)
	{
		switch (_type)
		{
		case typecode::gl_float:
			return "float";
		case typecode::gl_double:
			return "double";
		case typecode::gl_int:
			return "int";
		case typecode::gl_unsigned_int:
			return "unsigned_int";
		case typecode::gl_short:
			return "short";
		case typecode::gl_unsigned_short:
			return "unsigned_short";
		case typecode::gl_byte:
			return "byte";
		case typecode::gl_unsigned_byte:
			return "unsigned_byte";
		case typecode::gl_int_vec2:
			return "int_vec2";
		case typecode::gl_int_vec3:
			return "int_vec3";
		case typecode::gl_int_vec4:
			return "int_vec4";
		case typecode::gl_unsigned_int_vec2:
			return "unsigned_int_vec2";
		case typecode::gl_unsigned_int_vec3:
			return "unsigned_int_vec3";
		case typecode::gl_unsigned_int_vec4:
			return "unsigned_int_vec4";
		case typecode::gl_float_vec2:
			return "float_vec2";
		case typecode::gl_float_vec3:
			return "float_vec3";
		case typecode::gl_float_vec4:
			return "float_vec4";
		case typecode::gl_double_vec2:
			return "double_vec2";
		case typecode::gl_double_vec3:
			return "double_vec3";
		case typecode::gl_double_vec4:
			return "double_vec4";
		case typecode::gl_float_mat2:
			return "float_mat2";
		case typecode::gl_float_mat3x2:
			return "float_mat3x2";
		case typecode::gl_float_mat2x3:
			return "float_mat2x3";
		case typecode::gl_float_mat3:
			return "float_mat3";
		case typecode::gl_float_mat3x4:
			return "float_mat3x4";
		case typecode::gl_float_mat4x3:
			return "float_mat4x3";
		case typecode::gl_float_mat2x4:
			return "float_mat2x4";
		case typecode::gl_float_mat4x2:
			return "float_mat4x2";
		case typecode::gl_float_mat4:
			return "float_mat4";
		case typecode::gl_double_mat2:
			return "double_mat2";
		case typecode::gl_double_mat3x2:
			return "double_mat3x2";
		case typecode::gl_double_mat2x3:
			return "double_mat2x3";
		case typecode::gl_double_mat3:
			return "double_mat3";
		case typecode::gl_double_mat3x4:
			return "double_mat3x4";
		case typecode::gl_double_mat4x3:
			return "double_mat4x3";
		case typecode::gl_double_mat2x4:
			return "double_mat2x4";
		case typecode::gl_double_mat4x2:
			return "double_mat4x2";
		case typecode::gl_double_mat4:
			return "double_mat4";


		case typecode::gl_sampler_1D:
			return "sampler1D";
		case typecode::gl_sampler_2D:
			return "sampler2D";
		case typecode::gl_sampler_3D:
			return "sampler3D";
		case typecode::gl_sampler_1D_array:
			return "sampler1DArray";
		case typecode::gl_sampler_2D_array:
			return "sampler2DArray";

		default:
			std::terminate();
			return nullptr;
		};
	};


	/**
	 * @brief Gets the type associated with an opengl typecode
	*/
	template <typecode Code>
	struct opengl_type;

	/**
	 * @brief Gets the type associated with an opengl typecode
	*/
	template <typecode Code>
	using opengl_type_t = typename opengl_type<Code>::type;

	template <> struct opengl_type<typecode::gl_float> { using type = GLfloat; };
	using gl_float = opengl_type_t<typecode::gl_float>;

	template <> struct opengl_type<typecode::gl_double> { using type = GLdouble; };
	using gl_double = opengl_type_t<typecode::gl_double>;

	template <> struct opengl_type<typecode::gl_int> { using type = GLint; };
	using gl_int = opengl_type_t<typecode::gl_int>;
	
	template <> struct opengl_type<typecode::gl_unsigned_int> { using type = GLuint; };
	using gl_unsigned_int = opengl_type_t<typecode::gl_unsigned_int>;
	
	template <> struct opengl_type<typecode::gl_short> { using type = GLshort; };
	using gl_short = opengl_type_t<typecode::gl_short>;
	
	template <> struct opengl_type<typecode::gl_unsigned_short> { using type = GLushort; };
	using gl_unsigned_short = opengl_type_t<typecode::gl_unsigned_short>;
	
	template <> struct opengl_type<typecode::gl_byte> { using type = GLbyte; };
	using gl_byte = opengl_type_t<typecode::gl_byte>;
	
	template <> struct opengl_type<typecode::gl_unsigned_byte> { using type = GLubyte; };
	using gl_unsigned_byte = opengl_type_t<typecode::gl_unsigned_byte>;

	using gl_sizei = GLsizei;
	using gl_sizeiptr = GLsizeiptr;
	

	inline void set_attribute_format(vertex_attribute_index _attribute, typecode _type, gl_int _count, bool _normalize, gl_unsigned_int _relativeOffsetBytes)
	{
		glVertexAttribFormat(_attribute.get(), _count, jc::to_underlying(_type), _normalize, _relativeOffsetBytes);
	};
	inline void set_attribute_binding(vertex_attribute_index _attribute, vertex_binding_index _binding)
	{
		glVertexAttribBinding(_attribute.get(), _binding.get());
	};





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

	inline void clear(buffer_bit _mask)
	{
		glClear(jc::to_underlying(_mask));
	};



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

	inline void draw_arrays(primitive _mode, GLuint _count, GLuint _first = 0)
	{
		glDrawArrays(jc::to_underlying(_mode), _first, _count);
	};





#pragma region SET_UNIFORM_FUNCTIONS

	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_float& _data)
	{
		glProgramUniform1f(_program.get(), _uniform.get(), _data);
	};
	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_float& _data0, const gl_float& _data1)
	{
		glProgramUniform2f(_program.get(), _uniform.get(), _data0, _data1);
	};
	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_float& _data0, const gl_float& _data1, const gl_float& _data2)
	{
		glProgramUniform3f(_program.get(), _uniform.get(), _data0, _data1, _data2);
	};
	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_float& _data0, const gl_float& _data1, const gl_float& _data2, const gl_float& _data3)
	{
		glProgramUniform4f(_program.get(), _uniform.get(), _data0, _data1, _data2, _data3);
	};

	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_double& _data)
	{
		glProgramUniform1d(_program.get(), _uniform.get(), _data);
	};
	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_double& _data0, const gl_double& _data1)
	{
		glProgramUniform2d(_program.get(), _uniform.get(), _data0, _data1);
	};
	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_double& _data0, const gl_double& _data1, const gl_double& _data2)
	{
		glProgramUniform3d(_program.get(), _uniform.get(), _data0, _data1, _data2);
	};
	inline void set_uniform(const program_id& _program, const uniform_location& _uniform, const gl_double& _data0, const gl_double& _data1, const gl_double& _data2, const gl_double& _data3)
	{
		glProgramUniform4d(_program.get(), _uniform.get(), _data0, _data1, _data2, _data3);
	};
	
#pragma endregion SET_UNIFORM_FUNCTIONS

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

	/**
	 * @brief Gets the shader code for a pipeline from the stages of another program
	 * @param _pipeline Program pipeline to set stages of
	 * @param _program Must be a seperable program or null
	 * @param _stages Shader stages to use
	*/
	inline void use_program_stages(const program_pipeline_id& _pipeline, const program_id& _program, shader_stage_bit _stages)
	{
		glUseProgramStages(_pipeline.get(), jc::to_underlying(_stages), _program.get());
	};

	/**
	 * @brief Gets the shader code for a pipeline from the stages of another program
	 * @param _pipeline Program pipeline to set stages of
	 * @param _program Must be a seperable program or null
	*/
	inline void use_program_stages(const program_pipeline_id& _pipeline, const program_id& _program)
	{
		return use_program_stages(_pipeline, _program, shader_stage_bit::all);
	};

};


#pragma region TEXTURE

namespace jc::gl
{
	/**
	 * @brief Specifies the storage for a 1D texture
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage1D.xhtml
	 *
	 * @param _texture Texture to allocate
	 * @param _format Internal texture data format
	 * @param _width Width of the texture in texels
	 * @param _levels Number of texture mipmap levels
	*/
	inline void set_storage_1D(texture_id _texture, internal_format _format, GLsizei _width, GLsizei _levels = 1)
	{
		glTextureStorage1D(_texture.get(), _levels, jc::to_underlying(_format), _width);
	};

	/**
	 * @brief Specifies the storage for a 2D texture
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
	 * 
	 * @param _texture Texture to allocate
	 * @param _format Internal texture data format
	 * @param _width Width of the texture in texels
	 * @param _height Height of the texture in texels
	 * @param _levels Number of texture mipmap levels
	*/
	inline void set_storage_2D(texture_id _texture, internal_format _format, GLsizei _width, GLsizei _height, GLsizei _levels = 1)
	{
		glTextureStorage2D(_texture.get(), _levels, jc::to_underlying(_format), _width, _height);
	};

	/**
	 * @brief Specifies the storage for a 3D texture
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage3D.xhtml
	 *
	 * @param _texture Texture to allocate
	 * @param _format Internal texture data format
	 * @param _width Width of the texture in texels
	 * @param _height Height of the texture in texels
	 * @param _depth Depth of the texture in texels
	 * @param _levels Number of texture mipmap levels
	*/
	inline void set_storage_3D(texture_id _texture, internal_format _format, GLsizei _width, GLsizei _height, GLsizei _depth, GLsizei _levels = 1)
	{
		glTextureStorage3D(_texture.get(), _levels, jc::to_underlying(_format), _width, _height, _depth);
	};


	/**
	 * @brief Writes 1D data to a texture
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage1D.xhtml
	 * 
	 * @param _texture 
	 * @param _format 
	 * @param _type 
	 * @param _pixels 
	 * @param _width 
	 * @param _xoffset 
	 * @param _level 
	*/
	inline void set_subimage_1D
	(
		texture_id _texture, format _format,
		pixel_typecode _type, const void* _pixels,
		GLsizei _width,
		GLsizei _xoffset = 0,
		GLsizei _level = 0
	)
	{
		glTextureSubImage1D(_texture.get(), _level, _xoffset, _width, jc::to_underlying(_format), jc::to_underlying(_type), _pixels);
	};

	/**
	 * @brief Writes 2D data to a texture
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml
	 *
	 * @param _texture
	 * @param _format 
	 * @param _type 
	 * @param _pixels 
	 * @param _width 
	 * @param _height 
	 * @param _xoffset 
	 * @param _yoffset 
	 * @param _level 
	*/
	inline void set_subimage_2D
	(
		texture_id _texture, format _format,
		pixel_typecode _type, const void* _pixels,
		GLsizei _width, GLsizei _height,
		GLsizei _xoffset = 0, GLsizei _yoffset = 0,
		GLsizei _level = 0
	)
	{
		glTextureSubImage2D(_texture.get(), _level, _xoffset, _yoffset, _width, _height, jc::to_underlying(_format), jc::to_underlying(_type), _pixels);
	};

	/**
	 * @brief Writes 3D data to a texture
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage3D.xhtml
	 *
	 * @param _texture
	 * @param _format
	 * @param _type
	 * @param _pixels
	 * @param _width
	 * @param _height
	 * @param _depth
	 * @param _xoffset
	 * @param _yoffset
	 * @param _zoffset
	 * @param _level
	*/
	inline void set_subimage_3D
	(
		texture_id _texture, format _format,
		pixel_typecode _type, const void* _pixels,
		GLsizei _width, GLsizei _height, GLsizei _depth,
		GLsizei _xoffset = 0, GLsizei _yoffset = 0, GLsizei _zoffset = 0,
		GLsizei _level = 0
	)
	{
		glTextureSubImage3D
		(
			_texture.get(), _level,
			_xoffset, _yoffset, _zoffset,
			_width, _height, _depth,
			jc::to_underlying(_format), jc::to_underlying(_type), _pixels
		);
	};


	/**
	 * @brief Color components for texture data
	*/
	enum class color_component
	{
		red = GL_RED,
		green = GL_GREEN,
		blue = GL_BLUE,
		alpha = GL_ALPHA,
		zero = GL_ZERO,
		one = GL_ONE
	};

	/**
	 * @brief Indexes for adjusting texture color swizzling
	*/
	enum class swizzle_channel
	{
		red = GL_TEXTURE_SWIZZLE_R,
		green = GL_TEXTURE_SWIZZLE_G,
		blue = GL_TEXTURE_SWIZZLE_B,
		alpha = GL_TEXTURE_SWIZZLE_A,
	};

	/**
	 * @brief Sets a single swizzle channels component
	 * https://www.khronos.org/opengl/wiki/Texture#Swizzle_mask
	 * 
	 * @param _texture 
	 * @param _channel 
	 * @param _component 
	*/
	inline void set_texture_swizzle(texture_id _texture, swizzle_channel _channel, color_component _component)
	{
		glTextureParameteri(_texture.get(), jc::to_underlying(_channel), jc::to_underlying(_component));
	};

	/**
	 * @brief Sets the textures swizzle channels to map to specified color components
	 * https://www.khronos.org/opengl/wiki/Texture#Swizzle_mask
	 *
	 * @param _texture
	 * @param _r Red swizzle channel component
	 * @param _g Green swizzle channel component
	 * @param _b Blue swizzle channel component
	 * @param _a Alpha swizzle channel component
	*/
	inline void set_texture_swizzle(texture_id _texture,
			color_component _r = color_component::red,
			color_component _g = color_component::green,
			color_component _b = color_component::blue,
			color_component _a = color_component::alpha)
	{
		const GLint _components[4]{ jc::to_underlying(_r), jc::to_underlying(_g), jc::to_underlying(_b), jc::to_underlying(_a) };
		glTextureParameteriv(_texture.get(), GL_TEXTURE_SWIZZLE_RGBA, _components);
	};

	/**
	 * @brief Attach a buffer object's data store to a buffer texture object
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexBuffer.xhtml
	 * 
	 * @param _texture Texture to attach the buffer to, must be a buffer texture
	 * @param _iformat Internal data format of the buffer
	 * @param _buffer Buffer to attach
	*/
	inline void attach_buffer_to_texture(const texture_id& _texture, internal_format _iformat, const vbo_id& _buffer)
	{
		glTextureBuffer(_texture.get(), jc::to_underlying(_iformat), _buffer.get());
	};

};

#pragma endregion TEXTURE

#endif