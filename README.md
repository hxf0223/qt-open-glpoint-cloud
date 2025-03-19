# README #

`Qt/OpenGL`相关资料：

* [learnopengl_book](/doc/learnopengl_book.pdf)
* [Qt/OpenGL tutorials](/doc/OpenGLTutorial.pdf)
* [github -- OpenGL + Qt Tutorial](https://github.com/ghorwin/OpenGLWithQt-Tutorial/tree/master)
* [github -- 3D Forest](https://github.com/VUKOZ-OEL/3d-forest/tree/main)
* [github -- show-point-cloud-and-stl](https://github.com/Lehamber/show-point-cloud-and-stl)
* [github -- Large Point Cloud Rendering](https://github.com/AlfonsoLRz/PointCloudRendering)
* [ControlPointsTransformAndEditing](https://github.com/giraphics/ControlPointsTransformAndEditing/tree/master)

## 问题 ##

### 1. Unresolved external symbol ###

当把 `imp` 源文件与 `api` 源文件都作为DLL这一个目标的源码来编译时，在`imp`相关源码的链接时，出现如下链接错误：

```bash
error LNK2001: 无法解析的外部符号 "public: static struct QMetaObject const test::gl_painter::GLCloudWidget2D::staticMetaObject" (?staticMetaObject@GLCloudWidget2D@gl_painter@test@@2UQMetaObject@@B)
```

这个问题，在`test`使用__declspec(dllexport)导入该`DLL`时，也会出现该链接错误：[qt forum --Unresolved symbol in user DLL "public: static struct QMetaObject"](https://forum.qt.io/topic/159421/unresolved-symbol-in-user-dll-public-static-struct-qmetaobject).
