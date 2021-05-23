#include "gles3jni.h"
#include <EGL/egl.h>

static const char VERTEX_SHADER[] =
    "#version 300 es\n"
    "layout(location = 0 ) in vec4 vPosition;\n"
    "void main() {\n"
    "    gl_Position = vPosition;\n"
    "}\n";

static const char FRAGMENT_SHADER[] =
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    fragColor = vec4 (1.0, 0.0, 0.0, 1.0 );\n"
    "}\n";

class RendererES3: public Renderer {
public:
    RendererES3();
    virtual ~RendererES3();
    bool init();

private:
    virtual void draw();
    const EGLContext mEglContext;
    GLuint mProgram;
};

Renderer* createES3Renderer() {
    RendererES3* renderer = new RendererES3;
    if (!renderer->init()) {
        delete renderer;
        return NULL;
    }
    return renderer;
}

RendererES3::RendererES3()
:   mEglContext(eglGetCurrentContext()),
    mProgram(0)
    { }

bool RendererES3::init() {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram)
        return false;

    ALOGV("Using OpenGL ES 3.0 renderer");
    return true;
}

RendererES3::~RendererES3() {
    /* The destructor may be called after the context has already been
     * destroyed, in which case our objects have already been destroyed.
     *
     * If the context exists, it must be current. This only happens when we're
     * cleaning up after a failed init().
     */
    if (eglGetCurrentContext() != mEglContext)
        return;
    glDeleteProgram(mProgram);
}

void RendererES3::draw() {
    glUseProgram(mProgram);
    GLfloat vVertices[] = { 0.0f, 0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f};
    // Load the vertex data
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
}
