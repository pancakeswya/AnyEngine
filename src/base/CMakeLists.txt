add_library(base INTERFACE)
target_sources(base PUBLIC
        decl.h
        error.h
        plugin.h
        plugin_defs.h
)