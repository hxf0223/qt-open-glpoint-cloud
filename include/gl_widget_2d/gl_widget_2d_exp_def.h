#pragma once

#ifdef _WIN32
#ifdef GL_WIDGET_2D_LIB
#define GL_WIDGET_2D_API Q_DECL_EXPORT  //__declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define GL_WIDGET_2D_API Q_DECL_IMPORT  //__declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif
#else  // _WIN32
#ifdef GL_WIDGET_2D_LIB
#define GL_WIDGET_2D_API __attribute__((visibility("default")))
#else
#define GL_WIDGET_2D_API  // __attribute__((visibility("hidden")))
#endif
#endif  // _WIN32
