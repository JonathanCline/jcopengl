#pragma once
#ifndef JCLIB_GL_OBJECT_HPP
#define JCLIB_GL_OBJECT_HPP

#include "gllib.hpp"
#include "glenum.hpp"

#include <jclib/type.h>
#include <jclib/concepts.h>


#if !__has_include(<span>)
#error "Stop being lazy and finish jc::span"
#endif

#include <span>

#define _JCLIB_GL_OBJECT_


#pragma region TRAITS_TYPE

namespace jc::gl
{
	/**
	 * @brief Traits type for describing how OpenGL objects can be interacted with
	*/
	template <object_type Type>
	struct object_traits;

	/**
	 * @brief Gets the target enum type for an object.
	 * @tparam Type OpenGL object type.
	*/
	template <object_type Type>
	using object_target_t = typename object_traits<Type>::target_type;

	/**
	 * @brief Gets the raw ID value type for an object.
	 * @tparam Type OpenGL object type.
	*/
	template <object_type Type>
	using object_value_t = typename object_traits<Type>::value_type;

	/**
	 * @brief Type trait for getting the "parameter_type" type for an opengl object.
	 * This type is used to store the raw OpenGL object ID.
	*/
	template <object_type T>
	using object_parameter_t = typename object_traits<T>::parameter_type;





	/**
	 * @brief Type trait for checking if an object type is bindable.
	 * @tparam Type Object type.
	 * @tparam Enable SFINAE specialization point.
	*/
	template <object_type Type, typename BindArgT = object_target_t<Type>, typename Enable = void>
	struct is_object_bindable : jc::false_type {};

#if JCLIB_FEATURE_CONCEPTS_V

	/**
	 * @brief Concept is fufilled if an object type is bindable.
	 * @tparam Type Object type.
	*/
	template <object_type Type, typename BindArgT>
	concept cx_bindable_object =
	(
		cx_same_as<BindArgT, void> && requires (object_value_t<Type> _value)
		{
			object_traits<Type>::bind(_value);
		}
	) || requires (object_value_t<Type> _value, BindArgT _arg)
	{
		object_traits<Type>::bind(_value, _arg);
	};

	/**
	 * @brief Type trait for checking if an object type is bindable.
	 * @tparam Type Object type.
	*/
	template <object_type Type, typename BindArgT>
	requires cx_bindable_object<Type, BindArgT>
	struct is_object_bindable<Type, BindArgT> : jc::true_type {};

#else

	/**
	 * @brief Type trait for checking if an object type is bindable.
	 * @tparam Type Object type.
	 * @tparam BindArgT optional arguement used to bind with, commonly the target_type.
	*/
	template <object_type Type, typename TargetT>
	struct is_object_bindable<Type, TargetT, void_t<
		decltype(object_traits<Type>::bind(std::declval<object_value_t<Type>>(), std::declval<TargetT>()))
	>> : jc::true_type {};

	/**
	 * @brief Type trait for checking if an object type is bindable.
	 * @tparam Type Object type.
	 * @tparam BindArgT optional arguement used to bind with, commonly the target_type.
	*/
	template <object_type Type>
	struct is_object_bindable<Type, TargetT = void, void_t<
		decltype(object_traits<Type>::bind(std::declval<object_value_t<Type>>()))
		>> : jc::true_type {};

#endif // JCLIB_FEATURE_CONCEPTS_V

#if JCLIB_FEATURE_INLINE_VARIABLES_V

	/**
	 * @brief Type trait for checking if an object type is bindable.
	 * @tparam Type Object type.
	 * @tparam Enable SFINAE specialization point.
	*/
	template <object_type Type, typename TargetT = object_target_t<Type>>
	constexpr inline auto is_object_bindable_v = is_object_bindable<Type, TargetT>::value;

#endif // JCLIB_FEATURE_INLINE_VARIABLES_V






};

#pragma endregion






namespace jc::gl
{

	namespace gl_impl
	{
		/**
		 * @brief Base type implementation of a non-owning integer invariant for holding OpenGL object IDs
		*/
		template <object_type Type>
		class object_id_invariant
		{
		public:

			/**
			 * @brief The traits type for the object type this holds.
			*/
			using traits_type = object_traits<Type>;

			/**
			 * @brief The raw value type for this object ID.
			*/
			using value_type = typename traits_type::value_type;

		protected:

			/**
			 * @brief Raw value accessor.
			 * @return Reference to the held value.
			*/
			constexpr value_type& value() noexcept { return this->value_; };

			/**
			 * @brief Raw value accessor.
			 * @return Reference to the held value.
			*/
			constexpr const value_type& value() const noexcept { return this->value_; };

		public:

			/**
			 * @brief Gets the held ID value.
			 * @return Raw object ID.
			*/
			constexpr value_type get() const noexcept { return this->value(); };

			/**
			 * @brief Gets the held ID value.
			 * @return Raw object ID.
			*/
			constexpr explicit operator value_type() const noexcept { return this->get(); };

			/**
			 * @brief Checks if this ID is not null.
			 * @return True if not null, false otherwise.
			*/
			constexpr bool good() const noexcept
			{
				return this->get() != traits_type::null();
			};

			/**
			 * @brief Checks if this ID is not null.
			 * @return True if not null, false otherwise.
			*/
			constexpr explicit operator bool() const noexcept
			{
				return this->good();
			};

			/**
			 * @brief Nulls the held object ID.
			*/
			constexpr void release()
			{
				this->value_ = traits_type::null();
			};

			/**
			 * @brief Nulls the held object ID and returns the original value.
			 * @return The object ID prior to being nulled.
			*/
			constexpr value_type extract() noexcept
			{
				const auto _out = this->get();
				this->release();
				return _out;
			};

			/*
				Comparisons
			*/

#if JCLIB_FEATURE_THREE_WAY_COMPARISON_V
			friend constexpr inline auto operator<=>(const object_id_invariant& lhs, const object_id_invariant& rhs) noexcept = default;
#endif
			constexpr bool operator==(const object_id_invariant& rhs) const noexcept = default;
			constexpr bool operator!=(const object_id_invariant& rhs) const noexcept = default;

			constexpr bool operator>(const object_id_invariant& rhs) const noexcept = default;
			constexpr bool operator<(const object_id_invariant& rhs) const noexcept = default;
			constexpr bool operator>=(const object_id_invariant& rhs) const noexcept = default;
			constexpr bool operator<=(const object_id_invariant& rhs) const noexcept = default;

			/*
				Special members
			*/

			/**
			 * @brief Does not initialize the held ID!!!
			*/
			constexpr object_id_invariant() = default;

			/**
			 * @brief Sets the held ID to a raw ID value.
			 *
			 * If debugging is enabled, this will also preform a check to
			 * ensure that the given raw object ID is the correct type for
			 * this object type.
			 *
			 * @param _value Raw object ID value
			*/
			constexpr explicit object_id_invariant(value_type _value) :
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

			// Null construction / assignment

			constexpr object_id_invariant(jc::null_t) noexcept :
				value_{ traits_type::null() }
			{};
			constexpr object_id_invariant& operator=(jc::null_t) noexcept
			{
				this->value() = traits_type::null();
				return *this;
			};

		private:

			/**
			 * @brief The raw object ID value
			*/
			value_type value_;

		};

		/**
		 * @brief Binds an ID invariant.
		 * 
		 * Requires that object_traits<Type> have a valid bind function defined.
		 * 
		 * @tparam TargetT Binding target type, required if traits type defines it.
		 * @param _id Object to bind.
		 * @param _target Target to bind it to.
		*/
		template <object_type Type, typename TargetT>
		JCLIB_REQUIRES((cx_bindable_object<Type, TargetT>))
		inline auto bind(object_id_invariant<Type> _id, TargetT _target) ->
			JCLIB_RET_SFINAE_CXSWITCH(void, is_object_bindable<Type, TargetT>::value)
		{
			return object_traits<Type>::bind(_id.get(), _target);
		};

		/**
		 * @brief Binds an ID invariant.
		 *
		 * Requires that object_traits<Type> have a valid bind function defined.
		 *
		 * @param _id Object to bind.
		*/
		template <object_type Type>
		JCLIB_REQUIRES((cx_bindable_object<Type, void>))
		inline auto bind(object_id_invariant<Type> _id) ->
			JCLIB_RET_SFINAE_CXSWITCH(void, is_object_bindable<Type, void>::value)
		{
			return object_traits<Type>::bind(_id.get());
		};

		/**
		 * @brief CRTP type for adding a common object interface.
		 * @tparam Type OpenGL Object type.
		 * @tparam ChildT CRTP derived type.
		*/
		template <object_type Type, typename ChildT>
		struct object_interface
		{
		private:

			/**
			 * @brief Calls the derived type's get() function
			 * @return Result of get()
			*/
			constexpr auto crtp_get() const
			{
				return static_cast<const ChildT&>(*this).get();
			};

		public:

			// target type bind function
			template <typename TargetT = object_target_t<Type>>
			JCLIB_REQUIRES((cx_bindable_object<Type, TargetT>))
			auto bind(TargetT _target) const ->
				JCLIB_RET_SFINAE_CXSWITCH(void, is_object_bindable<Type, TargetT>::value)
			{
				object_traits<Type>::bind(this->crtp_get(), _target);
			};
			
			// no target type bind function
			template <typename TargetT = void>
			JCLIB_REQUIRES((cx_bindable_object<Type, void>))
			auto bind() const
			-> JCLIB_RET_SFINAE_CXSWITCH(void, is_object_bindable<Type, void>::value)
			{
				object_traits<Type>::bind(this->crtp_get());
			};
		};


	};

	/**
	 * @brief Non-owning integer invariant for holding OpenGL object IDs
	*/
	template <object_type Type>
	struct object_id :
		public gl_impl::object_id_invariant<Type>,
		public gl_impl::object_interface<Type, object_id<Type>>
	{
	private:
		using parent_type = gl_impl::object_id_invariant<Type>;

	public:


		using parent_type::parent_type;
	};

};


#pragma region TRAITS_IMPLEMENTATIONS

namespace jc::gl
{

	/**
	 * @brief Traits type for OpenGL shader objects
	*/
	template <>
	struct object_traits<object_type::shader>
	{
		/**
		 * @brief Raw object ID value type.
		*/
		using value_type = ::GLuint;

		/**
		 * @brief Enum type containing object parameters
		*/
		using parameter_type = shader_parameter;

		/**
		 * @brief No target type for shaders.
		*/
		using target_type = void;

		/**
		 * @brief Gets the parameter for a shader object.
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetShaderiv.xhtml
		 *
		 * @param _object Object to get parameter value from.
		 * @param _param Parameter to get value of.
		 * @param _values Where to write the parameter values to.
		*/
		static void get(value_type _object, parameter_type _param, std::span<GLint> _values)
		{
			glGetShaderiv(_object, jc::to_underlying(_param), _values.data());
		};

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

	/**
	 * @brief Traits type for OpenGL shader objects
	*/
	using shader_traits = object_traits<object_type::shader>;

	/**
	 * @brief Traits type for OpenGL program objects
	*/
	template <>
	struct object_traits<object_type::program>
	{
	public:

		/**
		 * @brief No target type for programs.
		*/
		using target_type = void;

		/**
		 * @brief Raw ID value type.
		*/
		using value_type = GLuint;

		/**
		 * @brief Enum type containing object parameters
		*/
		using parameter_type = program_parameter;

		/**
		 * @brief Gets the value of a parameter for a program object.
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgramiv.xhtml
		 *
		 * @param _object Object to get parameter value from.
		 * @param _param Parameter to get value of.
		 * @param _values Where to write the parameter values to.
		*/
		static void get(value_type _object, parameter_type _param, std::span<GLint> _values)
		{
			glGetProgramiv(_object, jc::to_underlying(_param), _values.data());
		};


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

	/**
	 * @brief Traits type for OpenGL program objects
	*/
	using program_traits = object_traits<object_type::program>;

	/**
	 * @brief Traits type for OpenGL vertex array objects (VAOs)
	*/
	template <>
	struct object_traits<object_type::vao>
	{
		using value_type = GLuint;

		/**
		 * @brief No target type for vaos.
		*/
		using target_type = void;


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

	/**
	 * @brief Traits type for OpenGL vertex array objects (VAOs)
	*/
	using vao_traits = object_traits<object_type::vao>;

	template <>
	struct object_traits<object_type::program_pipeline>
	{
		using value_type = GLuint;

		/**
		 * @brief No target type for programs.
		*/
		using target_type = void;

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
	 * @brief Traits type for OpenGL texture objects
	*/
	template <>
	struct object_traits<object_type::texture>
	{

		using value_type = GLuint;

		/**
		 * @brief Enum type containing object parameters.
		*/
		using parameter_type = texture_parameter;

		/**
		 * @brief Enum containing targets that this object may be bound to.
		*/
		using target_type = texture_target;

		/**
		 * @brief Gets the parameter for a texture object using the target it is bound to.
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetTexParameter.xhtml
		 *
		 * @param _target Target the object is bound to.
		 * @param _param Parameter to get value of.
		 * @param _values Where to write the parameter values to.
		*/
		static void get(target_type _target, texture_parameter _param, std::span<GLint> _values)
		{
			glGetTexParameteriv(jc::to_underlying(_target), jc::to_underlying(_param), _values.data());
		};

#if GL_VERSION_4_5
		/**
		 * @brief Gets the parameter for a texture object. Requires OpenGL 4.5+
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetTexParameter.xhtml
		 *
		 * @param _object Object to get parameter value from.
		 * @param _param Parameter to get value of.
		 * @param _values Where to write the parameter values to.
		*/
		static void get(value_type _object, parameter_type _param, std::span<GLint> _values)
		{
			glGetTextureParameteriv(_object, jc::to_underlying(_param), _values.data());
		};
#endif

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
	 * @brief Traits type for OpenGL vertex buffer objects (VBOs)
	*/
	template <>
	struct object_traits<object_type::vbo>
	{
	public:

		using value_type = GLuint;

		/**
		 * @brief Enum for the targets this object can be bound to
		*/
		using target_type = vbo_target;

		/**
		 * @brief Enum type containing object parameters.
		*/
		using parameter_type = vbo_parameter;

		/**
		 * @brief Gets the parameter for a vbo using the target it is bound to.
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetBufferParameter.xhtml
		 *
		 * @param _target Target the object is bound to.
		 * @param _param Parameter to get value of.
		 * @param _values Where to write the parameter values to.
		*/
		static void get(target_type _target, parameter_type _param, std::span<GLint> _values)
		{
			glGetBufferParameteriv(jc::to_underlying(_target), jc::to_underlying(_param), _values.data());
		};

#if GL_VERSION_4_5
		/**
		 * @brief Gets the parameter for a vbo. Requires OpenGL 4.5+
		 *
		 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetBufferParameter.xhtml
		 *
		 * @param _object Object to get parameter value from.
		 * @param _param Parameter to get value of.
		 * @param _values Where to write the parameter values to.
		*/
		static void get(value_type _object, parameter_type _param, std::span<GLint> _values)
		{
			glGetNamedBufferParameteriv(_object, jc::to_underlying(_param), _values.data());
		};
#endif

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

	/**
	 * @brief Traits type for OpenGL vertex buffer objects (VBOs)
	*/
	using vbo_traits = object_traits<object_type::vbo>;

};

#pragma endregion

#endif // JCLIB_GL_OBJECT_HPP