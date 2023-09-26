//HOMEWORK NÂ° 2
//ComputaciÃ³n GrÃ¡fica
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
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, int& drawMode);
void updateHousePosition(std::vector<float>& Vertices, float& hauseX, float& hauseY, float tamano, bool isClockwise);

void updateHousePosition(std::vector<float>& Vertices, float& hauseX, float& hauseY, float tamano, bool isClockwise, float scaleFactor);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	
float SpeedX = 0.0005f;  // Velocidad en el eje X
float SpeedY = 0.0005f;  // Velocidad en el eje Y

bool isHauseMoving = false; 


// FunciÃ³n para generar los vÃ©rtices de una casa
std::vector<float> generateHouseVertices(float size, float offsetX, float offsetY) {
    std::vector<float> vertices =
   {
        // Pared frontal
        -0.23f*size, -0.2f*size, 0.0f, 0.8f, 0.8f, 0.8f, // Pared con color gris
         0.23f*size, -0.2f*size, 0.0f, 0.8f, 0.8f, 0.8f,
         0.23f*size,  0.2f*size, 0.0f, 0.8f, 0.8f, 0.8f,
        -0.23f*size,  0.2f*size, 0.0f, 0.8f, 0.8f, 0.8f,

        // Techo
        -0.3f*size,  0.2f*size, 0.0f, 1.0f, 0.5f, 0.0f, // Techo con color rojo
         0.3f*size,  0.2f*size, 0.0f, 0.8f, 0.1f, 0.1f,
         0.0f*size,  0.4f*size, 0.0f, 0.8f, 0.1f, 0.1f,

        // Ventana izquierda
        -0.19f*size, 0.1f*size, 0.0f, 0.1f, 0.7f, 0.2f, // Ventana con color cian
        -0.1f*size, 0.1f*size, 0.0f, 0.1f, 0.7f, 0.2f,
        -0.1f*size, -0.1f*size, 0.0f, 0.0f, 0.5f, 0.5f,
        -0.19f*size, -0.1f*size, 0.0f, 0.0f, 0.5f, 0.5f,

        // Ventana derecha
        0.19f*size, 0.1f*size, 0.0f, 0.1f, 0.7f, 0.2f,
         0.1f*size, 0.1f*size, 0.0f, 0.1f, 0.7f, 0.2f,
         0.1f*size, -0.1f*size, 0.0f, 0.0f, 0.5f, 0.5f,
		0.19f*size, -0.1f*size, 0.0f, 0.0f, 0.5f, 0.5f,
		
		// Puerta
        -0.05f*size, -0.2f*size, 0.0f, 0.5f, 0.0f, 0.0f, // Puerta con color marrÃ³n
         0.05f*size, -0.2f*size, 0.0f, 0.5f, 0.0f, 0.0f,
         0.05f*size, 0.0f*size, 0.0f, 0.4f, 0.2f, 0.1f,
        -0.05f*size, 0.0f*size, 0.0f, 0.4f, 0.2f, 0.1f,
	};
    // Mover la casa a la posiciÃ³n deseada
    for (int i = 0; i < vertices.size(); i += 6) {
        vertices[i] += offsetX;
        vertices[i + 1] += offsetY;
    }
    
    return vertices;
}


float minScale = 0.99983f;  // Escala mínima
float maxScale = 1.00076f;  // Escala máxima	

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
	// Definir la posiciÃ³n inicial de la casa
   
	// Generar los vÃ©rtices de la casa
	float houseX = -0.5f;
    float houseY = -0.5f;
	
    std::vector<float> houseVertices = generateHouseVertices(0.5f, houseX, houseY);
	
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
	unsigned int houseLIndices[] = {0,1,1,2,2,3,3,0, 3, 4,4,5,5,6,6,4, 10,7,7,8,8,9,9,10, 15,18,18,17,17,16, 11,12,12,13,13,14,14,11};
	unsigned int houseSLIndices[] = {0, 1, 2, 5,6,4,3,2,3,0, 9,10,7,8,9, 15,18,17,16, 13,12,11,14,13};
	
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, houseVertices.size() * sizeof(float), houseVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
	
    

    int drawMode = GL_TRIANGLES;
	
	
	float currentScale = maxScale;  // Escala actual
	float scaleFactor = 0.0f;

	int scaleDirection = 1;   // Dirección de la escala (1 para aumento, -1 para reducción)
	float scaleSpeed = 0.01f; 
	float grow = 0.001f;
	float sinn = 0.0f;

    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, drawMode);
		
		// Después de la llamada a processInput

		// Update
		if (isHauseMoving){
			sinn = (0.2f)*sin(glfwGetTime());//Funcion seno, mientras menor el k entonces menor amplitud
			scaleFactor = 1.0f + (grow * sinn);
			
			std::cout<<scaleFactor<<std::endl;
			// Aplica el factor de escala a los vértices de la casa
			for (int i = 0; i < houseVertices.size(); i += 6) {
				//houseVertices[i] *= scaleFactor;
				//houseVertices[i + 1] *= scaleFactor;float xOffset = houseVertices[i] - houseX;
				float xOffset = houseVertices[i] - houseX;
				float yOffset = houseVertices[i + 1] - houseY;

				// Aplica la escala alrededor del centro
				houseVertices[i] = houseX + xOffset * scaleFactor;
				houseVertices[i + 1] = houseY + yOffset * scaleFactor;
			}
			updateHousePosition(houseVertices, houseX, houseY, 0.5f, 0);
			
		}
			
			
		
       

		// render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibuja la casa
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, houseVertices.size() * sizeof(float), houseVertices.data(), GL_STATIC_DRAW);

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

        // Intercambia los bÃºferes de pantalla
        glfwSwapBuffers(window);

        // Comprueba y llama a los eventos de GLFW
        glfwPollEvents();
    }

    // Limpieza y terminaciÃ³n
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

    // Cambia el modo de dibujo segÃºn la tecla presionada
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
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS ) {
        // Cambia el estado de la casa (inicia/detiene el movimiento)
        isHauseMoving = !isHauseMoving;
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


// Movimiento horario 1
// Movimiento antihorario 0
void updateHousePosition(std::vector<float>& Vertices, float& houseX, float& houseY, float tamano, bool isClockwise) {
    static int direction = 0; // Variable para rastrear la dirección del movimiento

    // Mueve la casa según la dirección actual, pero considera si debe ser horario o antihorario
    switch (direction) {
        case 0: // Mover hacia arriba
            houseY += isClockwise ? SpeedY : -SpeedY;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i+1] += isClockwise ? SpeedY : -SpeedY;
            }
            break;
        case 1: // Mover hacia la derecha
            houseX += isClockwise ? SpeedX : -SpeedX;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i] += isClockwise ? SpeedX : -SpeedX;
            }
            break;
        case 2: // Mover hacia abajo
            houseY -= isClockwise ? SpeedY : -SpeedY;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i+1] -= isClockwise ? SpeedY : -SpeedY;
            }
            break;
        case 3: // Mover hacia la izquierda
            houseX -= isClockwise ? SpeedX : -SpeedX;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i] -= isClockwise ? SpeedX : -SpeedX;
            }
            break;
    }

    // Verifica si la casa alcanzó los límites de la ventana en la dirección actual
    if (direction == 0 && ((houseY > 1.0f - tamano && isClockwise) || (houseY < -1.0f + tamano && !isClockwise))) {
        direction = isClockwise ? 1 : 3; // Cambia la dirección a la derecha o a la izquierda según el sentido
    } else if (direction == 1 && ((houseX > 1.0f - tamano && isClockwise) || (houseX < -1.0f + tamano && !isClockwise))) {
        direction = isClockwise ? 2 : 0; // Cambia la dirección hacia abajo o hacia arriba según el sentido
    } else if (direction == 2 && ((houseY > 1.0f - tamano && !isClockwise) || (houseY < -1.0f + tamano && isClockwise))) {
        direction = isClockwise ? 3 : 1; // Cambia la dirección a la izquierda o a la derecha según el sentido
    } else if (direction == 3 && ((houseX > 1.0f - tamano && !isClockwise) || (houseX < -1.0f + tamano && isClockwise))) {
        direction = isClockwise ? 0 : 2; // Cambia la dirección hacia arriba o hacia abajo según el sentido
    }
}

void updateHousePosition(std::vector<float>& Vertices, float& houseX, float& houseY, float tamano, bool isClockwise, float scaleFactor) {
    static int direction = 0; // Variable para rastrear la dirección del movimiento

    // Aplica la escala a los vértices de la casa
    for (int i = 0; i < Vertices.size(); i += 6) {
        Vertices[i] = Vertices[i] * scaleFactor;
        Vertices[i + 1] = Vertices[i + 1] * scaleFactor;
    }
	
    // Mueve la casa según la dirección actual, pero considera si debe ser horario o antihorario
    switch (direction) {
        case 0: // Mover hacia arriba
            houseY += isClockwise ? SpeedY : -SpeedY;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i+1] += isClockwise ? SpeedY : -SpeedY;
            }
            break;
        case 1: // Mover hacia la derecha
            houseX += isClockwise ? SpeedX : -SpeedX;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i] += isClockwise ? SpeedX : -SpeedX;
            }
            break;
        case 2: // Mover hacia abajo
            houseY -= isClockwise ? SpeedY : -SpeedY;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i+1] -= isClockwise ? SpeedY : -SpeedY;
            }
            break;
        case 3: // Mover hacia la izquierda
            houseX -= isClockwise ? SpeedX : -SpeedX;
            for (int i = 0; i < Vertices.size(); i += 6) {
                Vertices[i] -= isClockwise ? SpeedX : -SpeedX;
            }
            break;
    }

    // Verifica si la casa alcanzó los límites de la ventana en la dirección actual
    if (direction == 0 && ((houseY > 1.0f - tamano && isClockwise) || (houseY < -1.0f + tamano && !isClockwise))) {
        direction = isClockwise ? 1 : 3; // Cambia la dirección a la derecha o a la izquierda según el sentido
    } else if (direction == 1 && ((houseX > 1.0f - tamano && isClockwise) || (houseX < -1.0f + tamano && !isClockwise))) {
        direction = isClockwise ? 2 : 0; // Cambia la dirección hacia abajo o hacia arriba según el sentido
    } else if (direction == 2 && ((houseY > 1.0f - tamano && !isClockwise) || (houseY < -1.0f + tamano && isClockwise))) {
        direction = isClockwise ? 3 : 1; // Cambia la dirección a la izquierda o a la derecha según el sentido
    } else if (direction == 3 && ((houseX > 1.0f - tamano && !isClockwise) || (houseX < -1.0f + tamano && isClockwise))) {
        direction = isClockwise ? 0 : 2; // Cambia la dirección hacia arriba o hacia abajo según el sentido
    }
}


/*
if (isHauseMoving) {
			
			scaleFactor += 0.01f * scaleDirection;  // Aumenta o reduce la escala
			std::cout<<scaleFactor<<std::endl;
			// Aplica la escala a los vértices de la casa
			for (int i = 0; i < houseVertices.size(); i += 6) {
				houseVertices[i] *= scaleFactor;
				houseVertices[i + 1] *= scaleFactor;
			}
			// Cambia la dirección de la escala si alcanza los límites
			if (scaleFactor >= maxScale) {
				//scaleFactor = maxScale;  // Establece la escala máxima
				scaleDirection = -1;    // Cambia la dirección de la escala a reducción
			} else if (scaleFactor <= minScale) {
				//scaleFactor = minScale;  // Establece la escala mínima
				scaleDirection = 1;     // Cambia la dirección de la escala a aumento
			}
		}
*/