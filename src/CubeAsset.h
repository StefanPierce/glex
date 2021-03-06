#ifndef CUBEASSET_H
#define CUBEASSET_H

#include <vector>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameAsset.h"

class CubeAsset : public GameAsset {
 public:

  /**
 * Initializes a new CubeAsset. Passed in is a vec3 for the position and a vec3
 * for the colour values (RGB).
 */
  CubeAsset(glm::vec3, glm::vec3);
  ~CubeAsset();
  virtual void Draw(GLuint);

 private:
  GLuint element_buffer_length;
  GLuint vertex_buffer_token, element_buffer_token;
  GLuint colorbuffer;

};

#endif // CUBEASSET_H
