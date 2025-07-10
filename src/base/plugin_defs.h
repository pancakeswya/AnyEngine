#ifndef BASE_PLUGIN_DEFS_H_
#define BASE_PLUGIN_DEFS_H_

#ifdef PLUGIN_SHARED
#   if defined(_WIN32) && !defined(__MINGW32__)
#       ifdef PLUGIN_EXPORT
#         define PLUGIN_API __declspec(dllexport)
#       else
#         define PLUGIN_API __declspec(dllimport)
#       endif
#       define PLUGIN_CONV __stdcall
#   else
#     define PLUGIN_API __attribute__ ((visibility ("default")))
#     define PLUGIN_CONV
#   endif
#else
#   define PLUGIN_API
#   define PLUGIN_CONV
#endif

#endif // BASE_PLUGIN_DEFS_H_
