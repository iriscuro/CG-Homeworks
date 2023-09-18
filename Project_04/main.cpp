#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

//Dibujar un circulo
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, int& drawMode);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 fragColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "   fragColor = aColor;\n"
                                 "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragColor;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(fragColor, 1.0);\n"
                                   "}\n\0";

// Función para generar los vértices de una circunferencia
std::vector<float> generateCircleVertices(float radius, int numSegments) {
    std::vector<float> vertices;
    float segmentAngle = 2.0f * M_PI / static_cast<float>(numSegments);

    for (int i = 0; i < numSegments; ++i) {
        float angle = static_cast<float>(i) * segmentAngle;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // Z coordinate
    }

    return vertices;
}

int main() {
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
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress)) {
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
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
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
    // Circunferencia
    float circleRadius = 0.5f;
    int numCircleSegments = 18; // Cambia este valor según la cantidad de partes que desees

    std::vector<float> circleVertices = generateCircleVertices(circleRadius, numCircleSegments);
	/*
	circleVertices.push_back(0.0f);  // Coordenada x del centro
	circleVertices.push_back(0.0f);  // Coordenada y del centro
	circleVertices.push_back(0.0f);  // Coordenada z del centro
	*/
	// Agregar líneas que conectan los vértices del círculo con el vértice central
	for (int i = 0; i < numCircleSegments; ++i) {
		circleVertices.push_back(0.0f);                     // X del vértice central
		circleVertices.push_back(0.0f);                     // Y del vértice central
		circleVertices.push_back(0.0f);                     // Z del vértice central
		
		circleVertices.push_back(circleVertices[i * 3]);     // X del vértice del círculo
		circleVertices.push_back(circleVertices[i * 3 + 1]); // Y del vértice del círculo
		circleVertices.push_back(0.0f);                     // Z del vértice del círculo

	}
	


    unsigned int circleVBO, circleVAO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
	
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
	


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glBindVertexArray(0);
	glBindVertexArray(circleVAO);

    int drawMode = GL_TRIANGLES;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window, drawMode);

        // render
        // ------
        glClearColor(1.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibuja la circunferencia
        glUseProgram(shaderProgram);
        glBindVertexArray(circleVAO);

        int numCircleVertices = static_cast<int>(circleVertices.size()) / 3;

        switch (drawMode) {
            case GL_POINTS:
                glPointSize(5.0f);
                glDrawArrays(GL_POINTS, 0, numCircleVertices);
                break;
            case GL_LINES:
                glDrawArrays(GL_LINE_LOOP, 0, numCircleVertices);
				// Dibujar líneas que conectan vértices del círculo con el vértice central
				glDrawArrays(GL_LINES, numCircleVertices, numCircleVertices * 2);
                break;
            case GL_LINE_STRIP:
                glDrawArrays(GL_LINE_STRIP, 0, numCircleVertices);
				// Dibujar líneas que conectan vértices del círculo con el vértice central
				glDrawArrays(GL_LINES, numCircleVertices, numCircleVertices * 2);
				break;
            case GL_TRIANGLES:
                //glDrawArrays(GL_TRIANGLES, 0, numCircleVertices);
				glDrawArrays(GL_TRIANGLE_FAN, 0, numCircleVertices);
				
				/*
				for (int i = 0; i < numCircleVertices - 1; ++i) {
					glDrawArrays(GL_TRIANGLES, 0, i + 2);
				}*/
                break;
        }

        // Intercambia los búferes de pantalla
        glfwSwapBuffers(window);

        // Comprueba y llama a los eventos de GLFW
        glfwPollEvents();
    }

    // Limpieza y terminación
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, int& drawMode) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Cambia el modo de dibujo según la tecla presionada
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        drawMode = GL_LINE_STRIP;
    } else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        drawMode = GL_TRIANGLES;
    } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        drawMode = GL_POINTS;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        drawMode = GL_LINES;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}