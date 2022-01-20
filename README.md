# jcopengl

## DISCLAIMER

This is still extremely WIP and no where near production ready.
So naturally I am using it anyways.
No guarantee on stability or compatiblity with anything prior to C++20 until I slap a "version 1" on this.


## Description

`jcopengl` is a C++ header-only extension onto the OpenGL API that provides type safety and some RAII.

The goal of this project is not to encapsulate, abstract, or wrap away OpenGL. That would be
foolish as I have no way of knowing what features may or may not be needed. Instead, this project
is purely to make working with OpenGL from C++ a much smoother experience and turn as many of
the common runtime errors due to improper usage of the OpenGL API into compile time errors.


## Design

OpenGL functions that use some restricted set of the possible `GLenum` values are implemented
to take a scoped enumerator with those possible values as to avoid implicit conversions.
Functions that use OpenGL object types have type safety implemented using a type named `object_id`.
OpenGL objects have their common functionality defined in specializations of the type `object_traits`.
OpenGL object types are enumerated using the enum class `object_type`.

Using the above ideas, the basic API is layed out as minimalistic functions wrapping their raw OpenGL
counterparts.

`unique_object` provides RAII for OpenGL objects allowing more typical C++ usage.

```cpp

// Example of how the common types for VAOs are defined

// Non-owning integer invariant for VAOs.
using vao_id = object_id<object_type::vao>;

// Owning RAII wrapper for a VAO.
// Allows implicit conversion to vao_id on non-temporaries.
using unique_vao = unique_object<object_type::vao>;

```


Below is an example hello triangle program using `jcopengl` and GLFW for context handling.

```cpp
#include <jclib/gl/gl.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>

namespace gl = jc::gl;

int main()
{
    // Context initialization code here, I'm using glfw for this

    JCLIB_ASSERT(glfwInit() != 0);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    auto _window = glfwCreateWindow(800, 600, "jclib is amazing", nullptr, nullptr);
    JCLIB_ASSERT(_window);
    
    glfwMakeContextCurrent(_window);
    JCLIB_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0);

    // The source code for our shader

    std::string _vertexShaderSource = R"(
#version 330 core
in vec3 pos;
void main()
{
    gl_Position = vec4(pos.xyz, 1.0);
};
)";
    std::string _fragmentShaderSource = R"(
#version 330 core
out vec3 color;
void main()
{
    color = vec3(1.0);
};
)";

    // Create the program object
    gl::unique_program _program = gl::new_program();
    {
        // Create our shader stages
        std::array<gl::unique_shader, 2> _shaders
        {
            gl::new_shader(gl::shader_type::vertex),
            gl::new_shader(gl::shader_type::fragment)
        };

        // Compile vertex shader and check for errors
        auto& _vertex = _shaders[0];
        if (!gl::compile(_vertex, _vertexShaderSource))
        {
            std::cout << gl::get_info_log(_vertex) << '\n';
            return 1;
        };
        
        // Compile fragment shader and check for errors
        auto& _fragment = _shaders[1];
        if (!gl::compile(_fragment, _fragmentShaderSource))
        {
            std::cout << gl::get_info_log(_fragment) << '\n';
            return 1;
        };

        // Link program together and check for errors.
        if (!gl::link(_program, _shaders))
        {
            std::cout << gl::get_info_log(_program) << '\n';
            return 1;
        };
    };

    // "bind" the program to use it
    _program.bind();

    // Our triangle vertex data
    std::array<float, 9> _verts
    {
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    // Create our vao and vbo
    gl::unique_vao _vao = gl::new_vao();
    gl::unique_vbo _vbo = gl::new_vbo();
    
    // Bind together to form state
    _vao.bind();
    _vbo.bind(gl::vbo_target::array);

    // Upload vertex data to vbo, default usage is gl_static_draw
    gl::buffer_data(_vbo, _verts);

    // Set the vertex binding
    gl::vertex_binding_index _bindIdx{ 0 };
    gl::bind_vertex_buffer(_bindIdx, _vbo, 0, sizeof(float) * 3);

    // A tad verbose for now, but uses modern shader introspection API
    gl::vertex_attribute_index _posAttr =
        gl::get_resource_location(_program, gl::resource_type::program_input, "pos").value();

    // Configure attribute
    gl::enable_attribute_array(_posAttr);
    gl::set_attribute_binding(_posAttr, _bindIdx);
    gl::set_attribute_format(_posAttr, gl::typecode::gl_float, 3, false, 0);

    // Draw until death
    while (true)
    {
        // Clear screen
        gl::clear(gl::buffer_bit::color);
        
        // Draw our triangle
        _vao.bind();
        gl::draw_arrays(3); // primitive type CAN be specified "gl::draw_arrays(gl::primitive::triangle, 3)"

        // Swap buffers and pull for events (GLFW specific)
        glfwSwapBuffers(_window);
        glfwWaitEventsTimeout(0.016f);
    };

    return 0;
};
```



## CMake Target

`jcopengl` is the library target. `jclib::opengl` is also provided as an alias.


