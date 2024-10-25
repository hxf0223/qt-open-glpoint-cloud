#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QString>

class OpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core {
  //Q_OBJECT
 public:
  OpenGLWidget(QWidget* parent = nullptr);
  ~OpenGLWidget();

 protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  unsigned int buildMeshline(float size, int count);
  void buildCooraxis(float length);
  unsigned int buildPointdata(std::vector<float>& point_vertexs);

 private:
  QOpenGLShaderProgram shaderProgram_mesh;
  QOpenGLShaderProgram shaderProgram_axis;
  QOpenGLShaderProgram shaderProgram_point;
  unsigned int VAO_MeshLine;
  unsigned int VBO_MeshLine;
  unsigned int VAO_Axis;
  unsigned int VBO_Axis;
  unsigned int VAO_Point;
  unsigned int VBO_Point;

  unsigned int num_vertex_;
  unsigned int num_points_;
  std::vector<float> pointData;

  float rotate_x_{}, rotate_z_{};
  float trans_x_{}, trans_y_{};
  float zoom_ratio_{1.0};

  bool first_mouse;
  bool leftMousePress;
  bool MidMousePress;
};
