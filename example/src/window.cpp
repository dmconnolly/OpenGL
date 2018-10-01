#include <stdio.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "window.hpp"

/// Minimal constructor
Window::Window() :
    cursor_locked(true),
    m_width(1024),
    m_height(768),
    m_title("Super fun OpenGL things")
{
    /* Empty */
}

/// Minimal destructor
Window::~Window() {
    delete renderer;
}

/// Initialises the window

/// Initialise glfw \n
/// Create glfw window
/// Obtain OpenGL context and assign to glfw window \n
/// Initialise glew \n
/// Set glfw window parameters \n
/// Set callbacks in window \n
/// Initialise Renderer instance \n
/// Enter event loop \n
void Window::start() {
    // Initialise glfw
    printf("\nCreating glfw window\n");
    if(!glfwInit()) {
        printf("GLFW failed to initialise");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 16);

    // Create glfw window
    m_glfw_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if(!m_glfw_window) {
        printf("GLFW failed to create window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Obtain an OpenGL context and assign to the GLFW window */
    glfwMakeContextCurrent(m_glfw_window);

    /* Initialise glew */
    printf("Initialise glew\n");
    glewExperimental = GL_TRUE;
    if(glewInit()) {
        fprintf(stderr, "glewInit() failed. Exiting\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Turn on sticky keys
    glfwSetInputMode(m_glfw_window, GLFW_STICKY_KEYS, 1);

    // Lock mouse
    glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(m_glfw_window, reshape_callback);
    glfwSetKeyCallback(m_glfw_window, key_callback);
    glfwSetCursorPosCallback(m_glfw_window, mouse_callback);

    // Store pointer in glfw window to this object
    glfwSetWindowUserPointer(m_glfw_window, this);

    // Show startup screen
    startup_screen();

    // Initialise renderer
    printf("Initialising renderer\n");
    renderer = new Renderer(m_width, m_height);

    printf("Entering event loop (output controls)\n\n\n");

    // Output controls to the console
    print_controls();

    // Enter event loop
    event_loop();
}

/// Reset the scene with new values
void Window::reset() {
    printf("\nCleaning up old scene objects\n");
    delete renderer;

    // Initialise renderer
    printf("Initialising renderer\n");
    renderer = new Renderer(m_width, m_height);

    printf("Entering event loop (output controls)\n\n\n");

    // Output controls to the console
    print_controls();
}

/// Show startup screen
void Window::startup_screen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 1024, 768);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(m_glfw_window);
}

/// Main event loop

/// Get Renderer to check key input for per-frame usage \n
/// Get Renderer to draw the scene into buffer \n
/// Swap the buffers \n
/// Poll for new events
void Window::event_loop() {
    while(!glfwWindowShouldClose(m_glfw_window)) {
        renderer->check_input_frame(keys);
        renderer->draw();
        glfwSwapBuffers(m_glfw_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

/// Print keyboard controls
void Window::print_controls() {
    printf("/////////////////////////////////////\n//\n");
    printf("// CONTROLS\n//\n");
    printf("/////////////////////////////////////\n//\n");
    printf("// W,A,S,D - Move camera\n");
    printf("// Shift - Faster camera movement\n");
    printf("// Mouse - Rotate camera\n//\n");
    printf("// Numpad 8,4,5,6 - Move light source\n");
    printf("// Numpad 7,9 - Light source Up/Down\n//\n");
    printf("// Q,E - Terrain roughness down/up\n//\n");
    printf("// R - Toggle shadow map viewer\n//\n");
    printf("// Tab - Cycle draw mode\n//\n");
    printf("// Space - Re-create scene (new random values)\n//\n");
    printf("// Escape - Lock / unlock mouse\n//\n");
    printf("/////////////////////////////////////\n\n");
}
