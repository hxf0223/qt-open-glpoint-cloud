#include "openglwidget.h"
#include "csv_reader.h"
#include "head.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent), QOpenGLFunctions_3_3_Core() {
  // 视角控制初始化
  rotate_x_ = -30.0;
  rotate_z_ = 100.0;
  trans_x_ = 0.0;
  trans_y_ = 0.0;
  zoom_ratio_ = 45.0;
  first_mouse = true;
  leftMousePress = false;
  MidMousePress = false;

  // 加载点云数据
  point_data_ = read_csv_data(":/pointCloud/csv/test.csv");
}

OpenGLWidget::~OpenGLWidget() {
  // 上下文绑定
  makeCurrent();

  // 释放从 GPU 内存中分配的缓冲对象，避免内存泄露
  glDeleteBuffers(1, &VBO_MeshLine_);
  glDeleteVertexArrays(1, &VAO_MeshLine_);
  glDeleteBuffers(1, &VBO_Axis_);
  glDeleteVertexArrays(1, &VAO_Axis_);
  glDeleteBuffers(1, &VBO_Point_);
  glDeleteVertexArrays(1, &VAO_Point_);

  // 释放shader
  shaderProgram_mesh_.release();
  shaderProgram_axis_.release();
  shaderProgram_point_.release();

  // 上下文解绑
  doneCurrent();
}

// 鼠标点击事件
void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    first_mouse = true;
    leftMousePress = true;
  } else if (event->button() == Qt::MiddleButton) {
    first_mouse = true;
    MidMousePress = true;
  }
}

// 鼠标释放事件
void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    leftMousePress = false;
  } else if (event->button() == Qt::MiddleButton) {
    MidMousePress = false;
  }
}

// 鼠标移动事件
void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  static QPoint lastPos;
  auto currentPos = event->pos();
  QPoint deltaPos;
  if (leftMousePress == true) {
    if (first_mouse == true) {
      lastPos = currentPos;
      first_mouse = false;
    }
    deltaPos = currentPos - lastPos;
    lastPos = currentPos;
    rotate_x_ = rotate_x_ + 0.3 * deltaPos.y();
    rotate_z_ = rotate_z_ + 0.3 * deltaPos.x();
    if (rotate_x_ > 30.0f) rotate_x_ = 30.0f;
    if (rotate_x_ < -120.0f) rotate_x_ = -120.0f;
  } else if (MidMousePress == true) {
    if (first_mouse == true) {
      lastPos = currentPos;
      first_mouse = false;
    }
    deltaPos = currentPos - lastPos;
    lastPos = currentPos;
    trans_x_ = trans_x_ + 0.01 * deltaPos.x();
    trans_y_ = trans_y_ - 0.01 * deltaPos.y();
  }
  update();
}

// 鼠标滚轮事件
void OpenGLWidget::wheelEvent(QWheelEvent* event) {
  auto scroll_offest = event->angleDelta().y() / 120;
  zoom_ratio_ = zoom_ratio_ - (float)scroll_offest;
  if (zoom_ratio_ < 1.0f) {
    zoom_ratio_ = 1.0f;
  }
  if (zoom_ratio_ > 100.0f) {
    zoom_ratio_ = 100.0f;
  }
  update();
}

// 初始化OpenGL，建立OpenGL资源/状态
void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  // enable depth_test
  glEnable(GL_DEPTH_TEST);

  // link meshline shaders
  shaderProgram_mesh_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shader_mesh.vs");
  shaderProgram_mesh_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shader_mesh.fs");
  shaderProgram_mesh_.link();

  // link coordinate axis shaders
  shaderProgram_axis_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shader_axis.vs");
  shaderProgram_axis_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shader_axis.fs");
  shaderProgram_axis_.link();

  // link pointcloud shaders
  shaderProgram_point_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shader_point.vs");
  shaderProgram_point_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shader_point.fs");
  shaderProgram_point_.link();

  // build meshline
  num_vertex_ = buildMeshline(1.0, 10);

  // build axis
  buildCooraxis(1.0);

  // build point
  num_points_ = buildPointdata(point_data_);
}

// 设置OpenGL视口，调整窗口大小
void OpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
}

// 渲染openGL场景
void OpenGLWidget::paintGL() {

  // 设置清除颜色缓冲区时所使用的颜色
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // 清除颜色缓冲区和深度缓冲区的内容
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 投影/视图/模型矩阵
  QMatrix4x4 projection, view, model;

  // 视角（人眼不动情况下视野大概60°），窗口横纵比，范围显示
  projection.perspective(zoom_ratio_, (float)width() / (float)height(), 1.0f, 100.0f);

  // 设置视角 – 眼睛位置（坐标），眼睛看向（坐标），头顶朝向（方向）
  view.lookAt(QVector3D(0.0, 0.0, 10.0), QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 1.0, 0.0));

  // 设置矩阵平移
  model.translate(trans_x_, trans_y_, 0.0);

  // 设置矩阵绕某个轴进行角度旋转 - 参数刻度，旋转轴
  model.rotate(rotate_x_, 1.0, 0.0, 0.0);
  model.rotate(rotate_z_, 0.0, 0.0, 1.0);

  // 绑定变换矩阵
  shaderProgram_mesh_.bind();
  shaderProgram_mesh_.setUniformValue("projection", projection);
  shaderProgram_mesh_.setUniformValue("view", view);
  shaderProgram_mesh_.setUniformValue("model", model);
  shaderProgram_axis_.bind();
  shaderProgram_axis_.setUniformValue("projection", projection);
  shaderProgram_axis_.setUniformValue("view", view);
  shaderProgram_axis_.setUniformValue("model", model);
  shaderProgram_point_.bind();
  shaderProgram_point_.setUniformValue("projection", projection);
  shaderProgram_point_.setUniformValue("view", view);
  shaderProgram_point_.setUniformValue("model", model);

  // draw meshline
  shaderProgram_mesh_.bind();
  glBindVertexArray(VAO_MeshLine_);
  glLineWidth(1.0f);
  glDrawArrays(GL_LINES, 0, num_vertex_);

  // draw axis
  shaderProgram_axis_.bind();
  glBindVertexArray(VAO_Axis_);
  glLineWidth(5.0f);
  glDrawArrays(GL_LINES, 0, 6);

  // draw point
  shaderProgram_point_.bind();
  glBindVertexArray(VAO_Point_);
  glPointSize(1.0f);
  glDrawArrays(GL_POINTS, 0, num_points_);
}

// 构建网格线
unsigned int OpenGLWidget::buildMeshline(float size, int count) {
  // 循环生成网格线的顶点数据 - 每个顶点包含三个分量（x、y、z）
  std::vector<float> mesh_vertexs;

  // 计算起始位置
  float start = count * (size / 2);
  float posX = start, posZ = start;
  for (int i = 0; i <= count; ++i) {
    // 横向
    mesh_vertexs.push_back(posX);
    mesh_vertexs.push_back(start);
    mesh_vertexs.push_back(0);
    mesh_vertexs.push_back(posX);
    mesh_vertexs.push_back(-start);
    mesh_vertexs.push_back(0);

    // 纵向
    mesh_vertexs.push_back(start);
    mesh_vertexs.push_back(posZ);
    mesh_vertexs.push_back(0);
    mesh_vertexs.push_back(-start);
    mesh_vertexs.push_back(posZ);
    mesh_vertexs.push_back(0);

    // 位置偏移
    posX = posX - size;
    posZ = posZ - size;
  }

  // 创建VAO/VBO
  glGenVertexArrays(1, &VAO_MeshLine_);
  glGenBuffers(1, &VBO_MeshLine_);

  // 绑定VAO/VBO
  glBindVertexArray(VAO_MeshLine_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_MeshLine_);
  glBufferData(GL_ARRAY_BUFFER, mesh_vertexs.size() * sizeof(float), &mesh_vertexs[0], GL_STATIC_DRAW);

  // 指定顶点数组对象的顶点属性数据 - 索引、大小、类型、归一化、连续点字节偏移量、顶点缓冲对象首个属性起始位置
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // 解绑VBO/VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // 计算顶点数量
  unsigned int vertex_count = (unsigned int)(mesh_vertexs.size() / 3);
  return vertex_count;
}

// 构建坐标轴
void OpenGLWidget::buildCooraxis(float length) {
  // 顶点数据 - 12个顶点
  // 坐标轴有三条线段：X轴、Y轴、Z轴，每个轴由两个点组成，因此一共有6个点
  // 每个点有3个坐标值（x、y、z）和3个颜色值（r、g、b），因此每个点有6个分量
  //  0   1   2   3   4   5
  // [x0, y0, z0, r0, g0, b0,
  //  x1, y1, z1, r1, g1, b1,
  //  x2, y2, z2, r2, g2, b2,
  //  ... ]

  // X轴：起点 (0, 0, 0)，终点 (length, 0, 0)
  // Y轴：起点 (0, 0, 0)，终点 (0, length, 0)
  // Z轴：起点 (0, 0, 0)，终点 (0, 0, length)
  std::vector<float> axis_vertexs(36, 0.0);
  axis_vertexs[3] = 1.0;
  axis_vertexs[6] = length;
  axis_vertexs[9] = 1.0;
  axis_vertexs[16] = 1.0;
  axis_vertexs[19] = length;
  axis_vertexs[22] = 1.0;
  axis_vertexs[29] = 1.0;
  axis_vertexs[32] = length;
  axis_vertexs[35] = 1.0;

  // 创建 VAO / VBO
  glGenVertexArrays(1, &VAO_Axis_);
  glGenBuffers(1, &VBO_Axis_);

  // 绑定 VAO / VBO
  glBindVertexArray(VAO_Axis_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Axis_);
  glBufferData(GL_ARRAY_BUFFER, axis_vertexs.size() * sizeof(float), &axis_vertexs[0], GL_STATIC_DRAW);

  // 指定顶点数组对象的顶点属性数据 - 索引、大小、类型、归一化、连续点字节偏移量、顶点缓冲对象首个属性起始位置
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 解绑 VBO / VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// 构建点云数据
unsigned int OpenGLWidget::buildPointdata(std::vector<float>& point_vertexs) {
  // 创建 VAO / VBO
  glGenVertexArrays(1, &VAO_Point_);
  glGenBuffers(1, &VBO_Point_);

  // 绑定 VAO / VBO
  glBindVertexArray(VAO_Point_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Point_);
  glBufferData(GL_ARRAY_BUFFER, point_vertexs.size() * sizeof(float), &point_vertexs[0], GL_STATIC_DRAW);

  // 指定顶点数组对象的顶点属性数据 - 索引、大小、类型、归一化、连续点字节偏移量、顶点缓冲对象首个属性起始位置
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 解绑 VBO / VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // 计算顶点数量
  unsigned point_count = (unsigned int)(point_vertexs.size() / 4);
  return point_count;
}
