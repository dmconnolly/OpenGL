#include <stdio.h>
#include <iostream>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shadow.hpp"

/// Initialise class members, frame buffer and depth texture
Shadow::Shadow(Shader *shader) :
    texture_size(glm::vec2(10240, 7680)),
    m_shader(shader)
{
    printf("Initialising shadow map\n");

    /* Create and bind frame buffer */
    glGenFramebuffers(1, &frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

    /* Create and bind texture */
    glGenTextures(1, &depth_texture_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth_texture_id);

    /* Set texture behaviour */
    GLfloat border_colour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour);  

    /* Bind texture to frame buffer depth component */
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        texture_size.x, texture_size.y, 0,
        GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0
    );

    /* Don't draw to colour buffer  */
    glDrawBuffer(GL_NONE);

    /* Attach texture to frame buffer */
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
        GL_TEXTURE_2D, depth_texture_id, 0
    );

    /* Check frame buffer status */
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("glCheckFramebufferStatus() failed\n");
    }

    /* Switch back to normal frame buffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// Empty destructor
Shadow::~Shadow() {
    /* Empty */
}

/// Return depth matrix
glm::mat4 Shadow::get_depth_matrix(glm::vec3 light_position) {
    glUseProgram(m_shader->shadow.id);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

    /* Bias for modifying depth matrix */
    static const glm::mat4 depth_bias(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    /* Compute the MVP matrix from point of view of the light */
    glm::mat4 light_projection = glm::ortho<double>(
        -25, 25, -25, 25, -25, 100
    );

    /* Create light view matrix */
    glm::mat4 light_view = glm::lookAt(
        glm::normalize(light_position),
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    /* Calculate light view projection matrix */
    glm::mat4 depth_matrix = light_projection * light_view;

    /* Send the unmodified depth matrix to the shadow shader */
    glUniformMatrix4fv(m_shader->shadow.uniforms.depth_matrix, 1, GL_FALSE, glm::value_ptr(depth_matrix));

    /* Return depth matrix in [0, 1] space rather than [-1, 1] */
    return depth_bias * depth_matrix;
}
