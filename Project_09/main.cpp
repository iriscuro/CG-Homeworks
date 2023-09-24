//HOMEWORK N° 2
//Computacón Gráfica
//Alumna: Iris Rocio Curo Quispe
//Menu de instrucciones(Entrada por teclado)
	//a: stripline 
	//t: triangule
	//p: point
	//l: line
//Dibujar una estrella  (S)
//Dibujar un circulo (C)
//Dibujar una casa (H)

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, int& drawMode);
void updateStarPosition(std::vector<float>& Vertices, float& starX, float& starY, float limite, bool isClockwise);
void updateHousePosition(std::vector<float>& Vertices, float& houseX, float& houseY, float tamaño, bool isClockwise);
void updateCirclePosition(std::vector<float>& circleVertices, float& circleX, float& circleY, float radius, bool isClockwise);

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
	
// PosiciÃ³n inicial donde se dibuja la estrella
float starX = -0.2f;  // Cambia esto a la posiciÃ³n X deseada
float starY = -0.2f; // Centrado verticalmente
float houseX = -0.5f;
float houseY = -0.5f;
float circleX = -0.8f;  // Cambia esto a la posición X deseada
float circleY = -0.8f; // Centrado verticalmente

float limite = 0.8f;	

//Star
float sSpeedX = 0.0005f;  // Velocidad en el eje X
float sSpeedY = 0.0005f;  // Velocidad en el eje Y

//Hause
float SpeedX = 0.0005f;  // Velocidad en el eje X
float SpeedY = 0.0005f;  // Velocidad en el eje Y

//Circle
float circleSpeedX = 0.0005f;  // Velocidad en el eje X
float circleSpeedY = 0.0005f;  // Velocidad en el eje Y

float circleRadius = 0.15f;
int numCircleSegments = 18;

bool isCircleMoving = false; // Variable para rastrear si el círculo está en movimiento
bool isStarMoving = false;	
bool isHauseMoving = false; 

float r = 0.8f;
float g = 0.4f;
float b = 0.3f;	
	
// FunciÃ³n para generar los vÃ©rtices de una estrella
std::vector<float> generateStarVertices(float size, float offsetX, float offsetY) {
    std::vector<float> vertices =
	{
		0.0f,  0.5f*size, 0.0f,  1.0f, 0.5f, 1.0f, // PUNTA
		0.09f*size, 0.18f*size, 0.0f,  1.0f, 0.7f, 1.0f,  
		0.5f*size,  0.2f*size, 0.0f,  0.0f, 0.5f, 1.0f, 
		0.16f*size, -0.12f*size, 0.0f,  0.0f, 0.5f, 1.0f, 
		0.3f*size,  -0.5f*size, 0.0f,  0.0f, 0.5f, 1.0f, 
		0.0f, -0.25f*size, 0.0f,  1.0f, 1.0f, 1.0f,  
		-0.3f*size, -0.5f*size, 0.0f,  0.0f, 0.5f, 1.0f, 
		-0.16f*size, -0.12f*size, 0.0f,  0.0f, 0.5f, 1.0f, 
		-0.5f*size,  0.2f*size, 0.0f,  0.0f, 0.5f, 1.0f,  
		-0.09f*size, 0.18f*size, 0.0f,  1.0f, 0.7f, 1.0f  
	};

    for (int i = 0; i < vertices.size(); i += 6) {
        vertices[i] += offsetX;
        vertices[i + 1] += offsetY;
    }

    return vertices;
}

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

// Función para generar los vértices de una circunferencia
std::vector<float> generateCircleVertices(float radius, int numSegments, float offsetX, float offsetY) {
    std::vector<float> vertices;
    float segmentAngle = 2.0f * M_PI / static_cast<float>(numSegments);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = static_cast<float>(i) * segmentAngle;
        float x = offsetX + radius * std::cos(angle);
        float y = offsetY + radius * std::sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // Z coordinate
		
		vertices.push_back(r);    // Componente roja del color
        vertices.push_back(g);    // Componente verde del color
        vertices.push_back(b);
    }

    return vertices;
}


void setupStar(unsigned int &starVAO, unsigned int &starVBO, std::vector<float>& starVertices) {
    glGenVertexArrays(1, &starVAO);
    glGenBuffers(1, &starVBO);
    glBindVertexArray(starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(float), starVertices.data(), GL_STATIC_DRAW);
    // Configura los atributos de vértice (posición y color) aquí si es necesario.
}

void setupCircle(unsigned int &circleVAO, unsigned int &circleVBO, std::vector<float>& circleVertices) {
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    // Configura los atributos de vértice (posición y color) aquí si es necesario.
}

void setupHouse(unsigned int &houseVAO, unsigned int &houseVBO, std::vector<float>& houseVertices) {
    glGenVertexArrays(1, &houseVAO);
    glGenBuffers(1, &houseVBO);
    glBindVertexArray(houseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, houseVertices.size() * sizeof(float), houseVertices.data(), GL_STATIC_DRAW);
    // Configura los atributos de vértice (posición y color) aquí si es necesario.
}


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
    if( !gladLoadGL(glfwGetProcAddress) )
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
	std::vector<float> starVertices = generateStarVertices(0.2f, starX, starY);

	// Define los indices para las diferentes primitivas
	//STAR
	unsigned int starPointIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	unsigned int starLineIndices[] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 0};
	unsigned int starLineStripIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	unsigned int starTriangleIndices[] = {0, 1, 2, 2, 3, 4, 4, 5, 0, 6, 7, 8, 8, 9, 0};

	unsigned int starIndices[] = {
		1, 2, 3, //Izquierda
		7, 8, 9, //derecha
		0, 4, 5, //Derecha abajo 
		0, 5, 6  //Izquierda abajo
	};
	
	//HOUSE
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
	
	
	//CIRCLE
	std::vector<float> circleVertices = generateCircleVertices(circleRadius, numCircleSegments, circleX, circleY);
	for (int i = 0; i < numCircleSegments; ++i) {
		circleVertices.push_back(circleX);                     // X del vértice central
		circleVertices.push_back(circleY);                     // Y del vértice central
		circleVertices.push_back(0.0f);                     // Z del vértice central
		
		circleVertices.push_back(circleVertices[i * 6]);     // X del vértice del círculo
		circleVertices.push_back(circleVertices[i * 6 + 1]); // Y del vértice del círculo
		circleVertices.push_back(0.0f);                     // Z del vértice del círculo

	}
	int numCircleVertices = static_cast<int>(circleVertices.size()) / 6;
	//

    unsigned int starVBO, starVAO;
    setupStar(starVAO, starVBO, starVertices);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
 	
	//----------------------
	unsigned int circleVBO, circleVAO;
	setupCircle(circleVAO, circleVBO, circleVertices);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
	glBindVertexArray(circleVAO);

   //------------
	    
	unsigned int houseVBO, houseVAO;
    setupHouse(houseVAO, houseVBO, houseVertices);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
	
	//************************************
    int drawMode = GL_TRIANGLES;
	

    // render loop
    // -----------
	
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, drawMode);

		// Update
		
		
		if (isStarMoving){
			updateStarPosition(starVertices, starX, starY, limite, 1);
		}
		
		if (isHauseMoving) {
            updateHousePosition(houseVertices, houseX, houseY, 0.5f, 1);
        }
        if (isCircleMoving) {
            updateCirclePosition(circleVertices, circleX, circleY, circleRadius, 0);
        }
		
		
		
        // render
        // ------
		glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		glBindVertexArray(starVAO);
		glBindBuffer(GL_ARRAY_BUFFER, starVBO);
		glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(float), starVertices.data(), GL_STATIC_DRAW);

		//glBufferSubData(GL_ARRAY_BUFFER, 0, starVertices.size() * sizeof(float), starVertices.data());
	

		
		switch (drawMode)
        {
            case GL_POINTS:
                glPointSize(5.0f);
                glDrawElements(GL_POINTS, 11, GL_UNSIGNED_INT, starPointIndices);
                break;
            case GL_LINES:
				glDrawElements(GL_LINES, 20, GL_UNSIGNED_INT, starLineIndices);
                break;
            case GL_LINE_STRIP:
				glDrawElements(GL_LINE_STRIP, 11, GL_UNSIGNED_INT, starLineStripIndices);
                break;
            case GL_TRIANGLES:
				glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, starIndices);
                break;
        }
		glBindVertexArray(0);
		

		// Dibujar el círculo
        //glBindVertexArray(circleVAO);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, circleVertices.size() * sizeof(float), circleVertices.data());
		
		glBindVertexArray(circleVAO);
		glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
		glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
		
		switch (drawMode) {
            case GL_POINTS:
                glPointSize(5.0f);
                glDrawArrays(GL_POINTS, 0, numCircleVertices);
                break;
            case GL_LINES:
                glDrawArrays(GL_LINE_LOOP, 0, numCircleVertices);
				// Dibujar líneas que conectan vértices del círculo con el vértice central
                break;
            case GL_LINE_STRIP:
                glDrawArrays(GL_LINE_STRIP, 0, numCircleVertices);
				// Dibujar líneas que conectan vértices del círculo con el vértice central
				break;
            case GL_TRIANGLES:
                //glDrawArrays(GL_TRIANGLES, 0, numCircleVertices);
				glDrawArrays(GL_TRIANGLE_FAN, 0, numCircleVertices);
                break;
        }

		
		glBindVertexArray(0);
		// Dibujar la casa
        glBindVertexArray(houseVAO);
		glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
		glBufferData(GL_ARRAY_BUFFER, houseVertices.size() * sizeof(float), houseVertices.data(), GL_STATIC_DRAW);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, houseVertices.size() * sizeof(float), houseVertices.data());


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
		
		glBindVertexArray(0);
        // Intercambia los bÃºferes de pantalla
        glfwSwapBuffers(window);

        // Comprueba y llama a los eventos de GLFW
        glfwPollEvents();
		
		
    }

    // Limpieza y terminaciÃ³n
	glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
	
	glDeleteVertexArrays(1, &houseVAO);
    glDeleteBuffers(1, &circleVBO);
	
	glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
	
    glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, int& drawMode )
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
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ) {
		std::cout<<"Star"<<std::endl;
        // Cambia el estado de la estrella(inicia/detiene el movimiento)
        isStarMoving = !isStarMoving;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS ) {
		std::cout<<"House"<<std::endl;
        // Cambia el estado de la casa (inicia/detiene el movimiento)
        isHauseMoving = !isHauseMoving;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS ) {
		std::cout<<"Circle"<<std::endl;
        // Cambia el estado del círculo (inicia/detiene el movimiento)
        isCircleMoving = !isCircleMoving;
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


void updateStarPosition(std::vector<float>& Vertices, float& starX, float& starY, float limite, bool isClockwise) {
    static int direction = 0; // Variable para rastrear la direcciÃ³n del movimiento

    // Mueve el cÃ­rculo segÃºn la direcciÃ³n actual
    switch (direction) {
        case 0: // Mover hacia arriba
            starY += isClockwise ? sSpeedY : -sSpeedY;
			for (int i = 0; i < Vertices.size(); i += 6) {
				Vertices[i + 1] += isClockwise ? sSpeedY : -sSpeedY;
			}
            break;
        case 1: // Mover hacia la derecha
            starX += isClockwise ? sSpeedX : -sSpeedX;
			for (int i = 0; i < Vertices.size(); i += 6) {
				Vertices[i] += isClockwise ? sSpeedX : -sSpeedX;
			}
            break;
        case 2: // Mover hacia abajo
            starY -= isClockwise ? sSpeedY : -sSpeedY;
			for (int i = 0; i < Vertices.size(); i += 6) {
				Vertices[i + 1] -= isClockwise ? sSpeedY : -sSpeedY;
			}
            break;
        case 3: // Mover hacia la izquierda
            starX -= isClockwise ? sSpeedX : -sSpeedX;
			for (int i = 0; i < Vertices.size(); i += 6) {
				Vertices[i] -= isClockwise ? sSpeedX : -sSpeedX;
			}
            break;
    }

    // Verifica si el cÃ­rculo alcanzÃ³ los lÃ­mites de la ventana en la direcciÃ³n actual
	if (direction == 0 && ((starY > 1.0f - limite && isClockwise) || (starY < -1.0f + limite && !isClockwise))) {
        direction = isClockwise ? 1 : 3; // Cambia la dirección a la derecha o a la izquierda según el sentido
    } else if (direction == 1 && ((starX > 1.0f - limite && isClockwise) || (starX < -1.0f + limite && !isClockwise))) {
        direction = isClockwise ? 2 : 0; // Cambia la dirección hacia abajo o hacia arriba según el sentido
    } else if (direction == 2 && ((starY > 1.0f - limite && !isClockwise) || (starY < -1.0f + limite && isClockwise))) {
        direction = isClockwise ? 3 : 1; // Cambia la dirección a la izquierda o a la derecha según el sentido
    } else if (direction == 3 && ((starX > 1.0f - limite && !isClockwise) || (starX < -1.0f + limite && isClockwise))) {
        direction = isClockwise ? 0 : 2; // Cambia la dirección hacia arriba o hacia abajo según el sentido
    }
    
}

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

void updateCirclePosition(std::vector<float>& circleVertices, float& circleX, float& circleY, float radius, bool isClockwise) {
    static int direction = 0; // Variable para rastrear la dirección del movimiento

    // Mueve el círculo según la dirección actual, pero considera si debe ser horario o antihorario
    switch (direction) {
        case 0: // Mover hacia arriba
            circleY += isClockwise ? circleSpeedY : -circleSpeedY;
            break;
        case 1: // Mover hacia la derecha
            circleX += isClockwise ? circleSpeedX : -circleSpeedX;
            break;
        case 2: // Mover hacia abajo
            circleY -= isClockwise ? circleSpeedY : -circleSpeedY;
            break;
        case 3: // Mover hacia la izquierda
            circleX -= isClockwise ? circleSpeedX : -circleSpeedX;
            break;
    }

    // Verifica si el círculo alcanzó los límites de la ventana en la dirección actual
    if (direction == 0 && ((circleY > 1.0f - radius && isClockwise) || (circleY < -1.0f + radius && !isClockwise))) {
        direction = isClockwise ? 1 : 3; // Cambia la dirección a la derecha o a la izquierda según el sentido
    } else if (direction == 1 && ((circleX > 1.0f - radius && isClockwise) || (circleX < -1.0f + radius && !isClockwise))) {
        direction = isClockwise ? 2 : 0; // Cambia la dirección hacia abajo o hacia arriba según el sentido
    } else if (direction == 2 && ((circleY > 1.0f - radius && !isClockwise) || (circleY < -1.0f + radius && isClockwise))) {
        direction = isClockwise ? 3 : 1; // Cambia la dirección a la izquierda o a la derecha según el sentido
    } else if (direction == 3 && ((circleX > 1.0f - radius && !isClockwise) || (circleX < -1.0f + radius && isClockwise))) {
        direction = isClockwise ? 0 : 2; // Cambia la dirección hacia arriba o hacia abajo según el sentido
    }

    // Actualiza los vértices del círculo con las nuevas coordenadas
    for (int i = 0; i < circleVertices.size(); i += 6) {
        circleVertices[i] = circleX + radius * std::cos(2.0f * M_PI * (i / 6) / (circleVertices.size() / 6));
        circleVertices[i + 1] = circleY + radius * std::sin(2.0f * M_PI * (i / 6) / (circleVertices.size() / 6));
    }
}
