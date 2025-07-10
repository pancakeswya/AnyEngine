#ifndef RENDER_PARSERS_OBJ_PARSER_H_
#define RENDER_PARSERS_OBJ_PARSER_H_

#include "render/object_parser.h"
#include "render/parsers/obj/transferer.h"

#include <string>
#include <vector>

namespace obj {

class Parser final : public render::ObjectParser {
public:
  Parser() noexcept = default;
  ~Parser() override = default;

  render::GeometryTransferer& Parse(const std::string& path, std::vector<std::string>& texture_paths) override;
private:
  GeometryTransferer transferer_;
};

} // namespace obj

#endif // RENDER_PARSERS_OBJ_PARSER_H_
