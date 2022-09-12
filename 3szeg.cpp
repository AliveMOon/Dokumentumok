#include <stdio.h>
#include <stdlib.h>

// g++ main.cpp -lGLESv2 -lglfw
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

static const GLuint WIDTH = 800;
static const GLuint HEIGHT = 600;
static const GLchar* sGLSLvx =
    "#version 100\n"
    "attribute vec3 position;\n"
    "void main() {\n"
    "   gl_Position = vec4(position, 1.0);\n"
    "}\n";
static const GLchar* sGLSLfr =
    "#version 100\n"
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";
static const GLfloat aVX3[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

GLint cmpGLprg(const char *pVXshdr, const char *pFRshdr) {
    enum Consts {INFOLOG_LEN = 512};
    GLchar sLOG[INFOLOG_LEN];
    GLint iGLfr;
    GLint iGLprg;
    GLint iRES;
    GLint iGLvx;

    /* Vertex shader */
    iGLvx = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(iGLvx, 1, &pVXshdr, NULL);
    glCompileShader(iGLvx);
    glGetShaderiv(iGLvx, GL_COMPILE_STATUS, &iRES);
    if (!iRES) {
        glGetShaderInfoLog(iGLvx, INFOLOG_LEN, NULL, sLOG);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", sLOG);
    }

    /* Fragment shader */
    iGLfr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(iGLfr, 1, &pFRshdr, NULL);
    glCompileShader(iGLfr);
    glGetShaderiv(iGLfr, GL_COMPILE_STATUS, &iRES);
    if (!iRES) {
        glGetShaderInfoLog(iGLfr, INFOLOG_LEN, NULL, sLOG);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", sLOG);
    }

    /* Link shaders */
    iGLprg = glCreateProgram();
    glAttachShader(iGLprg, iGLvx);
    glAttachShader(iGLprg, iGLfr);
    glLinkProgram(iGLprg);
    glGetProgramiv(iGLprg, GL_LINK_STATUS, &iRES);
    if (!iRES) {
        glGetProgramInfoLog(iGLprg, INFOLOG_LEN, NULL, sLOG);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", sLOG);
    }

    glDeleteShader(iGLvx);
    glDeleteShader(iGLfr);
    return iGLprg;
}

int main( int nARG, char** ppARG ) {
    for( int i = nARG; i; i-- ) 
	    printf( "%d. \"%s\r\n", nARG-i,*(ppARG+nARG-i));

    GLuint iGLSLprg, iGLvxb;
    GLint pos;
    GLFWwindow* pWND;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    pWND = glfwCreateWindow(WIDTH, HEIGHT, "__FILE__", NULL, NULL);
    if( !pWND ) {
        printf( "%0.8x. not good!\r\n", pWND  );
        return EXIT_FAILURE;
    }
    printf( "pWND %0.8x\r\n", pWND  );
    glfwMakeContextCurrent(pWND);

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

    iGLSLprg = cmpGLprg(sGLSLvx, sGLSLfr);
    pos = glGetAttribLocation(iGLSLprg, "position");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, WIDTH, HEIGHT);

    glGenBuffers(1, &iGLvxb);
    glBindBuffer(GL_ARRAY_BUFFER, iGLvxb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aVX3), aVX3, GL_STATIC_DRAW);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(pWND)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(iGLSLprg);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(pWND);
    }
    glDeleteBuffers(1, &iGLvxb);
    glfwTerminate();
    return EXIT_SUCCESS;
}
