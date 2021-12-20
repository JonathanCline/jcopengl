#pragma once
#ifndef JCLIB_OPENGL_GL_HPP
#define JCLIB_OPENGL_GL_HPP

#include "gllib.hpp"
#include "globject.hpp"
#include "glenum.hpp"

#include <glad/glad.h>

#include <jclib/type.h>
#include <jclib/ranges.h>
#include <jclib/concepts.h>
#include <jclib/optional.h>
#include <jclib/algorithm.h>
#include <jclib/type_traits.h>

#define _JCLIB_OPENGL_GL_ 

#include <span>
#include <tuple>
#include <string>
#include <format>
#include <compare>
#include <iostream>
#include <charconv>
#include <string_view>



// TODO : Move the below macros into jclib

/**
 * Defines a full specialization of a type trait class template and adds a member alias named "type" set to "_type"
 * 
 * @param _typetrait The class template to specialize.
 * @param _type Type to alias for the member alias "type".
 * @param ... Template specialization arguements.
*/
#define JCLIB_FULL_SPECIALIZE_CLASS_WITH_TYPE(typetrait, _type, ...) template <> \
struct typetrait < __VA_ARGS__ > \
{ \
	using type = _type;\
}

/**
 * Defines a full specialization of a type trait class template and inherits from the type "_parent".
 *
 * @param _typetrait The class template to specialize.
 * @param _parent Type to inherit from for the specialization.
 * @param ... Template specialization arguements.
*/
#define JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(typetrait, _parent, ...)template <> \
struct typetrait < __VA_ARGS__ > : public _parent \
{}



namespace jc::gl
{


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


	namespace gl_impl
	{
		// Converts a parameter type to a target type for that parameter
		template <typename T>
		struct parameter_target_type;

		// Converts a parameter type to a target type for that parameter
		template <typename T>
		using parameter_target_type_t = typename parameter_target_type<T>::type;
		
		JCLIB_FULL_SPECIALIZE_CLASS_WITH_TYPE(parameter_target_type, texture_target, texture_parameter);
		


		/**
		 * @brief Helper type for constant object type values
		 * @param _value OpenGL object type value
		*/
		template <object_type T>
		using object_type_constant = std::integral_constant<object_type, T>;

		/**
		 * @brief Converts a parameter type into object type value for said parameter
		*/
		template <typename T>
		struct parameter_object_type;
		
		JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(parameter_object_type, object_type_constant<object_type::texture>,	texture_parameter);
		JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(parameter_object_type, object_type_constant<object_type::shader>,	shader_parameter);
		JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(parameter_object_type, object_type_constant<object_type::program>,	program_parameter);

		/**
		 * @brief Converts a target type into object type value for said target
		*/
		template <typename T>
		struct target_object_type;

		JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(target_object_type, object_type_constant<object_type::texture>, texture_target);
		JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(target_object_type, object_type_constant<object_type::vbo>,		vbo_target);
		



		template <object_type T, typename ValueT, typename Enable = void>
		struct has_get : jc::false_type
		{};

		template <object_type T, typename ValueT>
		struct has_get<T, ValueT, jc::void_t
		<
			decltype(object_traits<T>::get
			(
				std::declval<object_value_t<T>>(),
				std::declval<object_parameter_t<T>>(),
				std::declval<std::span<ValueT>>()
			))
		>> :
			jc::true_type
		{};


#if JCLIB_FEATURE_CONCEPTS_V
		template <object_type T, typename ValueT>
		concept cx_has_get = requires (const object_value_t<T>& _object, object_parameter_t<T> _param, std::span<ValueT> _values)
		{
			object_traits<T>::get(_object, _param, _values);
		};
#endif
	};

	/**
	 * @brief Type trait for determining if a type is a target type enum.
	 * @tparam T Type to check.
	*/
	template <typename T>
	struct is_target_type :
		jc::false_type
	{};
	
	JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(is_target_type, jc::true_type, texture_target);
	JCLIB_FULL_SPECIALIZE_CLASS_WITH_PARENT(is_target_type, jc::true_type, vbo_target);


#if JCLIB_FEATURE_INLINE_VARIABLES_V
	/**
	 * @brief Type trait for determining if a type is a target type enum.
	 * @tparam T Type to check.
	*/
	template <typename T>
	constexpr inline auto is_target_type_v = is_target_type<T>::value;
#endif

	/**
	 * @brief Gets the value(s) of a parameter for an OpenGL object.
	 * 
	 * @tparam Type Object type enum value.
	 * @tparam ValueT Parameter value type.
	 * 
	 * @param _object Object to get parameter values from.
	 * @param _param Parameter to get.
	 * @param _values Output variable for where to write the parameter value(s) to.
	*/
	template <object_type Type, typename ValueT, size_t Extent>
	JCLIB_REQUIRES
	((
		gl_impl::cx_has_get<Type, ValueT>
	))
	inline auto get(const object_id<Type>& _object, object_parameter_t<Type> _param, std::span<ValueT, Extent> _values) ->
		JCLIB_RET_SFINAE_CXSWITCH(void, gl_impl::has_get<Type, ValueT>::value)
	{
		object_traits<Type>::get(_object.get(), _param, _values);
	};


	/**
	 * @brief Gets the value(s) of a parameter for an OpenGL object.
	 *
	 * @tparam Type Object type enum value.
	 * @tparam ValueT Parameter value type.
	 *
	 * @param _object Object to get parameter values from.
	 * @param _param Parameter to get, parameter MUST only have a single value associated with it.
	 *
	 * @return The value of the parameter.
	*/
	template <typename ValueT = GLint, object_type Type>
	JCLIB_REQUIRES((gl_impl::cx_has_get<Type, ValueT>))
	inline auto get(const object_id<Type>& _object, object_parameter_t<Type> _param) ->
		JCLIB_RET_SFINAE_CXSWITCH(ValueT, gl_impl::has_get<Type, ValueT>::value)
	{
		ValueT _value{};
		get(_object, _param, std::span<ValueT>{ &_value, 1 });
		return _value;
	};



	/**
	 * @brief Gets the value(s) of a parameter for an OpenGL object bound to a target.
	 *
	 * @tparam TargetT Target type for the object being accessed.
	 * @tparam ValueT Parameter value type.
	 *
	 * @param _target Target the object is bound to.
	 * @param _param Parameter to get.
	 * @param _values Output variable for where to write the parameter value(s) to.
	*/
	template <typename TargetT, typename ValueT, size_t Extent>
	JCLIB_REQUIRES
	((
		gl_impl::cx_has_get<gl_impl::target_object_type<TargetT>::value, ValueT>
	))
	inline auto get(const TargetT& _target, gl_impl::parameter_target_type_t<TargetT> _param, std::span<ValueT, Extent> _values) ->
		JCLIB_RET_SFINAE_CXSWITCH(void, gl_impl::has_get<gl_impl::target_object_type<TargetT>::value, ValueT>::value)
	{
		object_traits<gl_impl::target_object_type<TargetT>::value>::get(_target, _param, _values);
	};





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
	struct unique_object :
		gl_impl::object_interface<Type, unique_object<Type>>
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


	// Helper functions for ease of use

	inline unique_shader new_shader(shader_type _type)
	{
		return unique_shader{ create<object_type::shader>(_type) };
	};
	inline unique_program new_program()
	{
		return unique_program{ create<object_type::program>() };
	};
	inline unique_vao new_vao()
	{
		return unique_vao{ create<object_type::vao>() };
	};
	inline unique_vbo new_vbo()
	{
		return unique_vbo{ create<object_type::vbo>() };
	};
	inline unique_texture new_texture(texture_target _target)
	{
		return unique_texture{ create<object_type::texture>(_target) };
	};



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

	inline jc::optional<resource_location> get_resource_location(const program_id& _program, resource_type _resourceType, const GLchar* _name)
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






	inline void clear(buffer_bit _mask)
	{
		glClear(jc::to_underlying(_mask));
	};



};


#pragma region VAO

namespace jc::gl
{
	inline void draw_arrays(primitive _mode, size_t _count, size_t _first = 0)
	{
		glDrawArrays(jc::to_underlying(_mode), static_cast<GLuint>(_first), static_cast<GLuint>(_count));
	};
	inline void draw_arrays(size_t _count, size_t _first = 0)
	{
		return draw_arrays(primitive::triangles, _count, _first);
	};

	inline void draw_elements(primitive _mode, typecode _indiceType, size_t _count, size_t _first = 0)
	{
		glDrawElements(jc::to_underlying(_mode), static_cast<GLuint>(_first),
			jc::to_underlying(_indiceType), reinterpret_cast<const void*>(_count));
	};
	inline void draw_elements(typecode _indiceType, size_t _count, size_t _first = 0)
	{
		return draw_elements(primitive::triangles, _indiceType, _count, _first);
	};

	inline void draw_arrays_instanced(primitive _mode, size_t _instanceCount, size_t _count, size_t _first = 0)
	{
		glDrawArraysInstanced(jc::to_underlying(_mode), static_cast<GLuint>(_first),
			static_cast<GLuint>(_count), static_cast<GLuint>(_instanceCount));
	};
	inline void draw_arrays_instanced(size_t _instanceCount, size_t _count, size_t _first = 0)
	{
		return draw_arrays_instanced(primitive::triangles, _instanceCount, _count, _first);
	};
	
	inline void draw_elements_instanced(primitive _mode, size_t _instanceCount, typecode _indiceType, size_t _count, size_t _first = 0)
	{
		glDrawElementsInstanced(jc::to_underlying(_mode), static_cast<GLuint>(_first), jc::to_underlying(_indiceType),
			reinterpret_cast<const void*>(_count), static_cast<GLuint>(_instanceCount));
	};
	inline void draw_elements_instanced(size_t _instanceCount, typecode _indiceType, size_t _count, size_t _first = 0)
	{
		return draw_elements_instanced(primitive::triangles, _instanceCount, _indiceType, _count, _first);
	};
};
#pragma endregion

#pragma region VBO
namespace jc::gl
{
	/**
	 * @brief Resizes a vbo and assigns its contents.
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 *
	 * @param _target The target that the vbo is bound to.
	 * @param _data The data to assign to the vbo.
	 * @param _usage Buffer usage hint.
	*/
	template <std::ranges::contiguous_range RangeT>
	inline void buffer_data(gl::vbo_target _target, const RangeT& _data, vbo_usage _usage = vbo_usage::static_draw)
	{
		glBufferData
		(
			jc::to_underlying(_target),
			std::ranges::size(_data) * sizeof(jc::ranges::value_t<RangeT>),
			std::ranges::data(_data),
			jc::to_underlying(_usage)
		);
	};

	/**
	 * @brief Resizes a vbo and clears its data.
	 *
	 * Buffer data will be unitialized!
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 *
	 * @param _target The target that the vbo is bound to.
	 * @param _elementCount Number of elements to allocate space for.
	 * @param _usage Buffer usage hint.
	*/
	template <typename ElementT = std::byte>
	inline void resize_buffer(gl::vbo_target _target, size_t _elementCount, vbo_usage _usage = vbo_usage::static_draw)
	{
		glBufferData(jc::to_underlying(_target), _elementCount * sizeof(ElementT), nullptr, jc::to_underlying(_usage));
	};

	/**
	 * @brief Resizes a vbo and assigns its contents.
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 *
	 * @param _target The target that the vbo is bound to.
	 * @param _data The data to assign to the vbo.
	 * @param _usage Buffer usage hint.
	*/
	template <std::ranges::contiguous_range RangeT>
	inline void buffer_subdata(gl::vbo_target _target, const RangeT& _data, size_t _offset = 0)
	{
		glBufferSubData
		(
			jc::to_underlying(_target),
			_offset * sizeof(jc::ranges::value_t<RangeT>),
			jc::ranges::distance(_data) * sizeof(jc::ranges::value_t<RangeT>),
			std::ranges::data(_data)
		);
	};


#if GL_VERSION_4_5

	/**
	 * @brief Resizes a vbo and assigns its contents.
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 *
	 * @param _vbo The vbo to buffer data into.
	 * @param _data The data to assign to the vbo.
	 * @param _usage Buffer usage hint.
	*/
	template <std::ranges::contiguous_range RangeT>
	inline void buffer_data(const vbo_id& _vbo, const RangeT& _data, vbo_usage _usage = vbo_usage::static_draw)
	{
		JCLIB_ASSERT(_vbo);
		glNamedBufferData
		(
			_vbo.get(),
			std::ranges::size(_data) * sizeof(jc::ranges::value_t<RangeT>),
			std::ranges::data(_data),
			jc::to_underlying(_usage)
		);
	};

	/**
	 * @brief Resizes a vbo and clears its data.
	 * 
	 * Buffer data will be unitialized!
	 * 
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * 
	 * @param _vbo The vbo to resize.
	 * @param _elementCount Number of elements to allocate space for.
	 * @param _usage Buffer usage hint.
	*/
	template <typename ElementT = std::byte>
	inline void resize_buffer(const vbo_id& _vbo, size_t _elementCount, vbo_usage _usage = vbo_usage::static_draw)
	{
		JCLIB_ASSERT(_vbo);
		glNamedBufferData(_vbo.get(), _elementCount * sizeof(ElementT), nullptr, jc::to_underlying(_usage));
	};

	/**
	 * @brief Resizes a vbo and assigns its contents.
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 *
	 * @param _vbo The vbo to buffer data into.
	 * @param _data The data to assign to the vbo.
	 * @param _usage Buffer usage hint.
	*/
	template <std::ranges::contiguous_range RangeT>
	inline void buffer_subdata(const vbo_id& _vbo, const RangeT& _data, size_t _offset = 0)
	{
		JCLIB_ASSERT(_vbo);
		glNamedBufferSubData
		(
			_vbo.get(),
			_offset * sizeof(jc::ranges::value_t<RangeT>),
			jc::ranges::distance(_data) * sizeof(jc::ranges::value_t<RangeT>),
			std::ranges::data(_data)
		);
	};

#endif

};
#pragma endregion

#pragma region SHADER
namespace jc::gl
{

	/**
	 * @brief Sets the source code for an opengl shader object
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
	 *
	 * @param _shader Shader to set source of
	 * @param _source Source code string
	*/
	inline void set_shader_source(const shader_id& _shader, std::string_view _source)
	{
		JCLIB_ASSERT(_shader);
		std::array<const GLchar*, 1> _stringPtrs{ _source.data() };
		std::array<GLsizei, 1>  _stringLengths{ _source.size() };
		glShaderSource(_shader.get(), _stringPtrs.size(), _stringPtrs.data(), _stringLengths.data());
	};

	/**
	 * @brief Gets the compile status for a shader
	 * @param _shader Shader to get status of
	 * @return Compile status value
	*/
	inline bool get_compile_status(const shader_id& _shader)
	{
		GLint _status = 0;
		glGetShaderiv(_shader.get(), GL_COMPILE_STATUS, &_status);
		return _status == GL_TRUE;
	};

	/**
	 * @brief Compiles a shader and returns the compile status
	 * @param _shader Shader to compile
	 * @return True on good compile, false otherwise
	*/
	inline bool compile(const shader_id& _shader)
	{
		glCompileShader(_shader.get());
		return get_compile_status(_shader);
	};

	/**
	 * @brief Compiles a shader using the provided source and returns the compile status
	 * @param _shader Shader to compile
	 * @param _source Shader source code
	 * @return True on good compile, false otherwise
	*/
	inline bool compile(const shader_id& _shader, const std::string_view _source)
	{
		set_shader_source(_shader, _source);
		return compile(_shader);
	};

	/**
	 * @brief Gets the shader info log string
	 * @param _shader Shader to get info log of
	 * @return Info log string
	*/
	inline std::string get_info_log(const shader_id& _shader)
	{
		GLint _infoLogLength{};
		glGetShaderiv(_shader.get(), GL_INFO_LOG_LENGTH, &_infoLogLength);

		std::string _infoLogData(static_cast<size_t>(_infoLogLength), '\0');
		GLint _actualInfoLogLength{};
		glGetShaderInfoLog(_shader.get(), _infoLogData.size(), &_actualInfoLogLength, _infoLogData.data());

		_infoLogData.resize(static_cast<size_t>(_actualInfoLogLength));
		return _infoLogData;
	};

};
#pragma endregion

#pragma region PROGRAM_RESOURCE
namespace jc::gl
{
	/**
	 * @brief Queries information about a program's resources.
	 * 
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgramResource.xhtml
	 * Also See https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_query
	 * 
	 * @param _program Program object to query. MUST NOT BE NULL!
	 * @param _type The type for the resource being queried.
	 * @param _index Index of the specific resource to query.
	 * @param _params The list of parameters to query.
	 * @param _values Output span where the parameter values are written to.
	 * 
	 * @return The number of values actually written into "_values".
	*/
	inline size_t get_resource(	const program_id& _program, resource_type _type, gl_unsigned_int _index,
								std::span<const resource_parameter> _params, std::span<GLint> _values)
	{
		JCLIB_ASSERT(_program);

		// Holds how many parameter values were actually read
		gl_sizei _gotCount = 0;
		
		const auto _propData = reinterpret_cast<const std::underlying_type_t<resource_parameter>*>(_params.data());
		glGetProgramResourceiv(
			_program.get(), jc::to_underlying(_type), _index,
			_params.size(), _propData,
			_values.size(), &_gotCount, _values.data()
		);
		return static_cast<size_t>(_gotCount);
	};

	/**
	 * @brief Queries an interface of a program.
	 * 
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgramInterface.xhtml
	 * Also See https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_query
	 * 
	 * @param _program The program to query an interface of. MUST NOT BE NULL.
	 * @param _type The type of the resource being queried.
	 * @param _interface The interface to query.
	 * 
	 * @return Value of the interface specified.
	*/
	inline gl_int get_interface(const program_id& _program, resource_type _type, program_interface _interface)
	{
		JCLIB_ASSERT(_program);

		// Get value
		gl_int _out{};
		glGetProgramInterfaceiv(_program.get(), jc::to_underlying(_type), jc::to_underlying(_interface), &_out);

		return _out;
	};

	/**
	 * @brief Gets the name of a program resource using its index.
	 *
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgramResourceName.xhtml
	 *
	 * @param _program The program the resource is in.
	 * @param _type The type of the resource being queried.
	 * @param _index The index of the resource to get.
	 * @param _nameBuffer The buffer to write the name to.
	 *
	 * @return The number of characters actually written.
	*/
	inline size_t get_resource_name(const program_id& _program, resource_type _type, GLuint _index, std::span<GLchar> _nameBuffer)
	{
		const auto _bufSize = static_cast<GLsizei>(_nameBuffer.size());
		GLsizei _readCount = 0;
		glGetProgramResourceName(_program.get(), jc::to_underlying(_type), _index, _bufSize, &_readCount, _nameBuffer.data());
		return static_cast<size_t>(_readCount);
	};

	/**
	 * @brief Gets the name of a program resource using its index.
	 *
	 * This overload allows the max name size to be pre-queried and passed in.
	 * 
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgramResourceName.xhtml
	 *
	 * @param _program The program the resource is in.
	 * @param _type The type of the resource being queried.
	 * @param _index The index of the resource to get.
	 * @param _maxLength The length of the name, usually gotten by using "get_resource()".
	 *
	 * @return The name of the resource.
	*/
	inline std::string get_resource_name(const program_id& _program, resource_type _type, GLuint _index, size_t _maxLength)
	{
		std::string _name(_maxLength, '\0');
		const auto _size = get_resource_name(_program, _type, _index, std::span{ _name });
		_name.resize(_size);
		return _name;
	};

	/**
	 * @brief Gets the name of a program resource using its index.
	 *
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgramResourceName.xhtml
	 *
	 * @param _program The program the resource is in.
	 * @param _type The type of the resource being queried.
	 * @param _index The index of the resource to get.
	 *
	 * @return The name of the resource.
	*/
	inline std::string get_resource_name(const program_id& _program, resource_type _type, GLuint _index)
	{
		const auto _params = std::array{ gl::resource_parameter::name_length };
		std::array<GLint, 1> _values{};
		get_resource(_program, _type, _index, _params, _values);
		return get_resource_name(_program, _type, _index, static_cast<size_t>(_values.front()));
	};

	/**
	 * @brief Integer invariant for holding program uniform block locations
	*/
	struct uniform_block_location : public gl_impl::integer_invariant<GLuint, struct uniform_block_location_tag>
	{
	private:
		using parent_type = gl_impl::integer_invariant<GLuint, struct uniform_block_location_tag>;
	public:

		/**
		 * @brief Constructs the uniform block location using a program resource location
		 * @param _location Uniform block location as a program resource
		*/
		constexpr uniform_block_location(const resource_location _location) :
			parent_type{ _location.get() }
		{};

		using parent_type::parent_type;
		using parent_type::operator=;
	};

	/**
	 * @brief Gets the location of a program uniform block.
	 * 
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetUniformBlockIndex.xhtml
	 * 
	 * @param _program Program ID, must not be null.
	 * @param _name Name of the uniform block, must be null.
	 * 
	 * @return The uniform blocks location, or null if not found.
	*/
	inline jc::optional<uniform_block_location> get_uniform_block_index(const program_id& _program, const GLchar* _name)
	{
		JCLIB_ASSERT(_program);
		JCLIB_ASSERT(_name);

		if (const auto _location = glGetUniformBlockIndex(_program.get(), _name); _location != -1)
		{
			return uniform_block_location{ static_cast<GLuint>(_location) };
		}
		else
		{
			return nullopt;
		};
	};


};
#pragma endregion

#pragma region PROGRAM
namespace jc::gl
{

	/**
	 * @brief Gets the program info log string
	 * @param _program Program to get info log of
	 * @return Info log string
	*/
	inline std::string get_info_log(const program_id& _program)
	{
		GLint _infoLogLength{};
		glGetProgramiv(_program.get(), GL_INFO_LOG_LENGTH, &_infoLogLength);

		std::string _infoLogData(static_cast<size_t>(_infoLogLength), '\0');
		GLint _actualInfoLogLength{};
		glGetProgramInfoLog(_program.get(), _infoLogData.size(), &_actualInfoLogLength, _infoLogData.data());

		_infoLogData.resize(static_cast<size_t>(_actualInfoLogLength));
		return _infoLogData;
	};


	/**
	 * @brief Gets the link status for a program
	 * @param _program Program to get status of
	 * @return Link status value
	*/
	inline bool get_link_status(const program_id& _program)
	{
		GLint _status = 0;
		glGetProgramiv(_program.get(), GL_LINK_STATUS, &_status);
		return _status == GL_TRUE;
	};

	/**
	 * @brief Links a program object
	 * 
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml
	 * 
	 * @param _program Program object to link
	 * @return True on good link, false otherwise
	*/
	inline bool link(const program_id& _program)
	{
		glLinkProgram(_program.get());
		return get_link_status(_program);
	};

	/**
	 * @brief Attaches a shader to a program object
	 * 
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
	 * 
	 * @param _program Program to attach to
	 * @param _shader Shader to attach to program
	*/
	inline void attach(const program_id& _program, const shader_id& _shader)
	{
		glAttachShader(_program.get(), _shader.get());
	};

	/**
	 * @brief Detaches a shader from a program object
	 *
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDetachShader.xhtml
	 *
	 * @param _program Program to detach from
	 * @param _shader Shader to detach from program
	*/
	inline void detach(const program_id& _program, const shader_id& _shader)
	{
		glDetachShader(_program.get(), _shader.get());
	};

	/**
	 * @brief Attaches and links a program to some shaders, detaching the shaders before returning
	 * @param _program Program to link and attach to
	 * @param _shaders Shaders to link
	 * @return True on good link, false otherwise
	*/
	template <jc::cx_range RangeT>
	requires jc::cx_convertible_to<jc::ranges::const_reference_t<RangeT>, shader_id>
	inline bool link(const program_id& _program, const RangeT& _shaders)
	{
		for (auto& _shader : _shaders)
		{
			attach(_program, _shader);
		};

		const auto _status = link(_program);
		
		for (auto& _shader : _shaders)
		{
			detach(_program, _shader);
		};
		
		return _status;
	};


#pragma region SET_UNIFORM_FUNCTIONS
	
	/**
	 * @brief Customization point for adding types that can be uploaded as uniforms
	 * @tparam T Specialize this type to add the customization
	 * @tparam Enable SFINAE specialization point
	*/
	template <typename T, typename Enable = void>
	struct uniform_ftor;

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
	
	/**
	 * @brief Sets a uniform value for a type with a uniform_ftor specialization implemented
	 * @tparam T Type to set the uniform with
	 * @tparam ...ArgTs Optional additional arguement types to pass to the uniform_ftor
	 * @param _program Program to set uniform on
	 * @param _uniform Uniform location to set
	 * @param _value Value to upload
	 * @param ..._args Optional additional arguements to pass to the uniform_ftor
	 * @return Depends on uniform_ftor::set return type
	*/
	template <typename T, typename... ArgTs>
	inline auto set_uniform(const program_id& _program, const uniform_location& _uniform, const T& _value, ArgTs&&... _args) ->
		decltype(uniform_ftor<T>::set
		(
			std::declval<const program_id&>(),
			std::declval<const uniform_location&>(),
			std::declval<const T&>(),
			std::declval<ArgTs&&>()...
		))
	{
		return uniform_ftor<T>::set(_program, _uniform, _value, std::forward<ArgTs>(_args)...);
	};


#pragma endregion SET_UNIFORM_FUNCTIONS

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
#pragma endregion

#pragma region TEXTURE

namespace jc::gl
{

	/**
	 * @brief Sets the value of a texture parameter.
	 * 
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
	 *
	 * @param _texture Texture to modify.
	 * @param _param Texture parameter to set.
	 * @param _value The value to assign to the parameter.
	*/
	inline void set(texture_id _texture, texture_parameter _param, GLint _value)
	{
		glTextureParameteri(_texture.get(), jc::to_underlying(_param), _value);
	};


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

#pragma endregion


namespace jc::gl
{
	/**
	 * @brief Opengl callback function signature
	*/
	using debug_callback_fn = void(APIENTRY*)
	(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	);

	inline void enable_debug_output()
	{
		glEnable(GL_DEBUG_OUTPUT);
	};
	inline void enable_debug_output_synchronous()
	{
		if (!glIsEnabled(GL_DEBUG_OUTPUT))
		{
			enable_debug_output();
		};
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	};

	inline void set_debug_callback(debug_callback_fn _cb, void* _userParam)
	{
		glDebugMessageCallback(_cb, _userParam);
	};
};

#endif