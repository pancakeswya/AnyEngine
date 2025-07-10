#include "render/object_parser_plugin.h"

#include "render/parsers/obj/object_parser.h"

render::ObjectParser* PLUGIN_CONV PluginObjectParserCreate() {
    return new obj::Parser();
}

void PLUGIN_CONV PluginObjectParserDestroy(render::ObjectParser* parser) {
    delete parser;
}
