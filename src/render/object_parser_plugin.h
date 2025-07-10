#ifndef RENDER_OBJECT_PARSER_PLUGIN_H_
#define RENDER_OBJECT_PARSER_PLUGIN_H_

#include "base/plugin_defs.h"
#include "render/object_parser.h"

extern "C" {

extern PLUGIN_API render::ObjectParser* PluginObjectParserCreate();
extern PLUGIN_API void PluginObjectParserDestroy(render::ObjectParser* parser);

} // extern "C"

#endif // RENDER_OBJECT_PARSER_PLUGIN_H_
