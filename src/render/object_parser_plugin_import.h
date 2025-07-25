#ifndef RENDER_OBJECT_PARSER_PLUGIN_IMPORT_H_
#define RENDER_OBJECT_PARSER_PLUGIN_IMPORT_H_

#include "base/decl.h"
#include "base/plugin.h"
#include "render/object_parser_plugin.h"

namespace render {

class ObjectParserPlugin final : base::Plugin {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(ObjectParserPlugin);

  explicit ObjectParserPlugin(const std::string& path)
    : Plugin(path),
      parser_create_(LOAD_SYM(PluginObjectParserCreate)),
      parser_destroy_(LOAD_SYM(PluginObjectParserDestroy)) {}

  [[nodiscard]] ObjectParser* CreateObjectParser() const {
    return parser_create_();
  }

  void DestroyObjectParser(ObjectParser* parser) const {
    parser_destroy_(parser);
  }
private:
  DECLARE_SYM(PluginObjectParserCreate) parser_create_ = nullptr;
  DECLARE_SYM(PluginObjectParserDestroy) parser_destroy_ = nullptr;
};

} // namespace render

#endif // RENDER_OBJECT_PARSER_PLUGIN_IMPORT_H_
