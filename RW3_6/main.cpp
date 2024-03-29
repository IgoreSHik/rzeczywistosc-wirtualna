#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

#include <vector>

const int edges = 3;

/*static const struct
{
    float x, y, z;
    float r, g, b;
} vertices[3][6] =
{
{
    { 0.0f, 0.5f, 1.f, 1.f, 0.f, 0.f, 0.f },
    { 0.5f, 0.1f, 1.f, 0.f, 1.f, 0.f, 0.f },
    { 0.2f, -0.5f, 1.f, 0.f, 0.f, 1.f, 0.f },
    { 0.2f, -0.5f, 1.f, 0.f, 0.f, 0.f, 1.f }
},
{
    { 0.0f, 0.5f, -1.0f, 1.f, 0.f, 0.f, 0.f },
    { -0.5f, 0.1f, -1.0f, 0.f, 1.f, 0.f, 0.f },
    { -0.2f, -0.5f, -1.0f, 0.f, 0.f, 1.f, 0.f },
    { -0.2f, -0.5f, -1.0f, 0.f, 0.f, 0.f, 1.f }
},
{
    { 0.0f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 0.f },
    { 0.2f, -0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f },
    { -0.2f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f },
    { -0.2f, -0.5f, 0.0f, 0.f, 0.f, 0.f, 1.f }
}
};*/

using namespace std;

struct vec3d
{
    float x,y,z;
};

struct triangle
{
    vec3d p[3];
};

struct mesh
{
    //struct triangle tris[16];
    vector<triangle> tris;
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
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

int main(void)
{
    mesh graniastoslup;

    graniastoslup.tris = {
    //FRONT
    {-1.f, 0.2f, 1.0f,   0.0f, 1.0f, 1.0f,   -0.5f, -1.0f, 1.0f},
    {1.f, 0.2f, 1.0f,   0.0f, 1.0f, 1.0f,   0.5f, -1.0f, 1.0f},
    {0.5f, -1.0f, 1.0f,   0.0f, 1.0f, 1.0f,   -0.5f, -1.0f, 1.0f},
    //BACK
    {-1.f, 0.2f, -1.0f,   0.0f, 1.0f, -1.0f,   -0.5f, -1.0f, -1.0f},
    {1.f, 0.2f, -1.0f,   0.0f, 1.0f, -1.0f,   0.5f, -1.0f, -1.0f},
    {0.5f, -1.0f, -1.0f,   0.0f, 1.0f, -1.0f,   -0.5f, -1.0f, -1.0f}

    };

    GLFWwindow* window;
    GLuint vertex_buffer[edges], vertex_shader[edges], fragment_shader[edges], program[edges];
    GLint mvp_location[edges], vpos_location[edges], vcol_location[edges];

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    for (int i = 0; i < edges; i++)
    {

    glGenBuffers(1, &vertex_buffer[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

    vertex_shader[i] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader[i], 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader[i]);

    fragment_shader[i] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader[i], 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader[i]);

    program[i] = glCreateProgram();
    glAttachShader(program[i], vertex_shader[i]);
    glAttachShader(program[i], fragment_shader[i]);
    glLinkProgram(program[i]);

    mvp_location[i] = glGetUniformLocation(program[i], "MVP");
    vpos_location[i] = glGetAttribLocation(program[i], "vPos");
    vcol_location[i] = glGetAttribLocation(program[i], "vCol");

    glEnableVertexAttribArray(vpos_location[i]);
//    glVertexAttribPointer(vpos_location[i], 2, GL_FLOAT, GL_FALSE,
//                          sizeof(vertices[i][0]), (void*) 0);
//    glEnableVertexAttribArray(vcol_location[i]);
//    glVertexAttribPointer(vcol_location[i], 3, GL_FLOAT, GL_FALSE,
//                          sizeof(vertices[i][0]), (void*) (sizeof(float) * 2));

    }

    int counter = 0;
    float mover = 0.0f;

    while (!glfwWindowShouldClose(window))
    {

        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_X(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());

            if (mover >= 1.0f) mover = -1.0f;
            mover += 0.01f;
            switch (counter)
            {
                case 0 ... 200: {mat4x4_translate(p, mover, mover, mover); counter++; break;}
                case 201 ... 400: {mat4x4_translate(p, mover, -mover, mover); counter++; break;}
                case 401 ... 600: {mat4x4_translate(p, -mover, -mover, mover); counter++; break;}
                case 601 ... 800: {mat4x4_translate(p, -mover, mover, mover); counter++; break;}
                case 801: {counter = 0; break;}
            }
            //ÑÒÀÐÒ

        for (int i = 0; i < edges; i++)
    {
        glGenBuffers(1, &vertex_buffer[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[i]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

    /*vertex_shader[i] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader[i], 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader[i]);

    fragment_shader[i] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader[i], 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader[i]);

    program[i] = glCreateProgram();
    glAttachShader(program[i], vertex_shader[i]);
    glAttachShader(program[i], fragment_shader[i]);
    glLinkProgram(program[i]);

    mvp_location[i] = glGetUniformLocation(program[i], "MVP");
    vpos_location[i] = glGetAttribLocation(program[i], "vPos");
    vcol_location[i] = glGetAttribLocation(program[i], "vCol");*/

    glEnableVertexAttribArray(vpos_location[i]);
//    glVertexAttribPointer(vpos_location[i], 2, GL_FLOAT, GL_FALSE,
//                          sizeof(vertices[i][0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location[i]);
//    glVertexAttribPointer(vcol_location[i], 3, GL_FLOAT, GL_FALSE,
//                          sizeof(vertices[i][0]), (void*) (sizeof(float) * 2));
            //ÔÈÍÈØ
        mat4x4_mul(mvp, p, m);

        glUseProgram(program[i]);
        glUniformMatrix4fv(mvp_location[i], 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
