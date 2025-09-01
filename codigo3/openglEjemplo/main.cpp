#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Vertex Shader source code (GLSL 4.10)
const char* vertexShaderSource = R"(
    #version 410 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 vertexColor;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        vertexColor = aColor ;
    }
)";

// Fragment Shader source code (GLSL 4.10)
const char* fragmentShaderSource = R"(
    #version 410 core
    out vec4 FragColor;
    in vec3 vertexColor;

    void main()
    {
        //FragColor = vec4(vec3(1.0,0.0,0.0), 1.0);
        FragColor = vec4(vertexColor, 1.0);
    }
)";

void generaCirculo(float* arr, int num_segments, float radio, float centroX, float centroY) {
    const float PI = 3.14159265358979323846f;
    
    // Primer vertice: el centro del circulo
    arr[0] = centroX;
    arr[1] = centroY;
    arr[2] = 0.0f;

    // Genera los vertices del perimetro
    for (int i = 0; i <= num_segments; ++i) {
        float angle = 2.0f * PI * (float)i / (float)num_segments;
        float x = centroX + (radio * cosf(angle));
        float y = centroY + (radio * sinf(angle));
        
        arr[3 * i + 3] = x;
        arr[3 * i + 4] = y;
        arr[3 * i + 5] = 0.0f;
    }
}
int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version to 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Build and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Build and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders to a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers and configure vertex attributes
    /*float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
    };*/

    float vertices[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         
         1.0f, -1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f
    };

    float colors[] = {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         
         1.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 1.0f,
         0.0f, 1.0f, 1.0f
    };
    

    // Define el numero de segmentos
    const int numSegments = 100;
    // El tamaño del array es (numSegments + 2) vertices * 3 componentes (x, y, z)
    const int totalVertices = (numSegments + 2) * 3;
    float circleVertices[totalVertices];

    // Llama a la funcion para llenar el array con los vertices del circulo
    // El radio es 0.5 y el centro esta en (0.0, 0.0)
    generaCirculo(circleVertices, numSegments, 0.5f, 0.0f, 0.0f);

    GLuint VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /*(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);*/


    // Unbind the VAO
    glBindVertexArray(0);


    GLuint VBOcolor;
    
    glGenBuffers(1, &VBOcolor);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render
        glClearColor(0.0f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 3);
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
