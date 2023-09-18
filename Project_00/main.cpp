//HOMEWORK N° 2
//Computación Gráfica
//Alumna: Iris Rocio Curo Quispe
//Menu de instrucciones(Entrada por teclado)
    //a: stripline 
    //t: triangule
    //p: point
    //l: line
//Dibujar una casa

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, int& drawMode);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/*
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
*/
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 fragColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   fragColor = aColor;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 fragColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(fragColor, 1.0);\n"
    "}\n\0";
	
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	/*
    float houseVertices[] = {
        // Pared frontal
        -0.23f, -0.2f, 0.0f,
         0.23f, -0.2f, 0.0f,
         0.23f,  0.2f, 0.0f,
        -0.23f,  0.2f, 0.0f,

        // Techo
        -0.3f,  0.2f, 0.0f,
         0.3f,  0.2f, 0.0f,
         0.0f,  0.4f, 0.0f,
		 
		// Ventana izquierda
		
		-0.19f, 0.1f, 0.0f,
        -0.1f, 0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
        -0.19f, -0.1f, 0.0f,
		
		// Ventana derecha
		
		0.19f, 0.1f, 0.0f,
        0.1f, 0.1f, 0.0f,
        0.1f, -0.1f, 0.0f,
        0.19f, -0.1f, 0.0f,
		
		 // Puerta
		-0.05f, -0.2f, 0.0f, 
        0.05f, -0.2f, 0.0f,
        0.05f, 0.0f, 0.0f, 
        -0.05f, 0.0f, 0.0f,
    };
	*/
	float houseVertices[] = {
        // Pared frontal
        -0.23f, -0.2f, 0.0f, 0.8f, 0.8f, 0.8f, // Pared con color gris
         0.23f, -0.2f, 0.0f, 0.8f, 0.8f, 0.8f,
         0.23f,  0.2f, 0.0f, 0.8f, 0.8f, 0.8f,
        -0.23f,  0.2f, 0.0f, 0.8f, 0.8f, 0.8f,

        // Techo
        -0.3f,  0.2f, 0.0f, 1.0f, 0.5f, 0.0f, // Techo con color rojo
         0.3f,  0.2f, 0.0f, 0.8f, 0.1f, 0.1f,
         0.0f,  0.4f, 0.0f, 0.8f, 0.1f, 0.1f,

        // Ventana izquierda
        -0.19f, 0.1f, 0.0f, 0.1f, 0.7f, 0.2f, // Ventana con color cian
        -0.1f, 0.1f, 0.0f, 0.1f, 0.7f, 0.2f,
        -0.1f, -0.1f, 0.0f, 0.0f, 0.5f, 0.5f,
        -0.19f, -0.1f, 0.0f, 0.0f, 0.5f, 0.5f,

        // Ventana derecha
        0.19f, 0.1f, 0.0f, 0.1f, 0.7f, 0.2f,
         0.1f, 0.1f, 0.0f, 0.1f, 0.7f, 0.2f,
         0.1f, -0.1f, 0.0f, 0.0f, 0.5f, 0.5f,
		0.19f, -0.1f, 0.0f, 0.0f, 0.5f, 0.5f,
		
		// Puerta
        -0.05f, -0.2f, 0.0f, 0.5f, 0.0f, 0.0f, // Puerta con color marrón
         0.05f, -0.2f, 0.0f, 0.5f, 0.0f, 0.0f,
         0.05f, 0.0f, 0.0f, 0.4f, 0.2f, 0.1f,
        -0.05f, 0.0f, 0.0f, 0.4f, 0.2f, 0.1f,
	};
	/*
    unsigned int houseIndices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
		
		7,8,9,
		9,10,11,
		12,13,14,
		14,15,16,
		17,18
    };*/
	unsigned int houseTIndices[] = {
		0, 1, 2,
        2, 3, 0,
        4, 5, 6,
		
		7,8,9,
		9,10,7,
		
		11,12,13,
		13,11,14,
		
		15,16,17,
		17,18,15
    };
	
	unsigned int houseIndices[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 };
	unsigned int houseLIndices[] = {0,1,1,2,2,3,3,0, 3, 4,4,5,5,6,6,4,
	10,7,7,8,8,9,9,10, 15,18,18,17,17,16, 11,12,12,13,13,14,14,11
	};
	unsigned int houseSLIndices[] = {0, 1, 2, 5,6,4,3,2,3,0,
	9,10,7,8,9, 15,18,17,16, 13,12,11,14,13
	};
	
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);

	/*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
	
    glBindVertexArray(0);

    int drawMode = GL_TRIANGLES;

    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, drawMode);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibuja la casa
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        switch (drawMode)
        {
            case GL_POINTS:
                glPointSize(5.0f);
                glDrawElements(GL_POINTS, sizeof(houseIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, houseIndices);
                break;
            case GL_LINES:
                glDrawElements(GL_LINES, sizeof(houseLIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, houseLIndices);
                break;
            case GL_LINE_STRIP:
                glDrawElements(GL_LINE_STRIP, sizeof(houseSLIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, houseSLIndices);
                break;
            case GL_TRIANGLES:
                glDrawElements(GL_TRIANGLES, sizeof(houseTIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, houseTIndices);
                break;
        }

        // Intercambia los búferes de pantalla
        glfwSwapBuffers(window);

        // Comprueba y llama a los eventos de GLFW
        glfwPollEvents();
    }

    // Limpieza y terminación
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, int& drawMode)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Cambia el modo de dibujo según la tecla presionada
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        drawMode = GL_LINE_STRIP;
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        drawMode = GL_TRIANGLES;
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        drawMode = GL_POINTS;
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        drawMode = GL_LINES;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}





//========================================================================
// OpenGL triangle example
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//! [code]
/*
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include <filesystem>

namespace fs = std::filesystem;

typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;

static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char* argv[])
{
    std::string argv_str(argv[0]);
    std::string base = argv_str.substr(0, argv_str.find_last_of("//"));
	std::cout << "\nThe current path: " << base << " \n";
	
	std::cout << "\nType of filesystem: " << std::filesystem::path::preferred_separator << std::endl;
	
	
	fs::path p = fs::current_path();
	std::cout << "Separator in my system " << fs::path::preferred_separator << std::endl;
 
    std::cout << "The current path: " << p << " decomposes into:\n"
              << "root name: " << p.root_name() << '\n'
              << "root directory: " << p.root_directory() << '\n'
              << "relative path: " << p.relative_path() << '\n';
			  

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
*/
    // NOTE: OpenGL error checks have been omitted for brevity
/*
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
*/
//! [code