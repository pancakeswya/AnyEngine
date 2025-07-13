#include "render/apis/gl/texture.h"

#include "render/apis/gl/check.h"

namespace gl {

Texture::Texture(const GLenum format, const uint8_t* data, const int width, const int height) 
  : ArrayHandle(glGenTextures, glDeleteTextures) {
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle_));
  GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data));
  GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
}

} // namespace gl