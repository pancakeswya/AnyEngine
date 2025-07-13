#include "render/apis/gl/error.h"

#include <string>

namespace gl {

namespace {

std::string GetGLMessage(const GLuint who) {
  GLint length;
  glGetShaderiv(who, GL_INFO_LOG_LENGTH, &length);

  std::string message(length, '\0');
  glGetShaderInfoLog(who, length, &length, message.data());

  return message;
}

} // namespace

Error Error::WithShaderMessage(const GLuint who) const {
  return Error{std::string(what()) + " [Message: " + GetGLMessage(who) + ']'};
}

} // namespace gl