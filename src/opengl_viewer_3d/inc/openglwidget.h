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
  QOpenGLShaderProgram shaderProgram_mesh_;
  QOpenGLShaderProgram shaderProgram_axis_;
  QOpenGLShaderProgram shaderProgram_point_;
  unsigned int VAO_MeshLine_;
  unsigned int VBO_MeshLine_;
  unsigned int VAO_Axis_;
  unsigned int VBO_Axis_;
  unsigned int VAO_Point_;
  unsigned int VBO_Point_;

  unsigned int num_vertex_;
  unsigned int num_points_;
  std::vector<float> point_data_;

  float rotate_x_{}, rotate_z_{};
  float trans_x_{}, trans_y_{};
  float zoom_ratio_{1.0};

  bool first_mouse;
  bool leftMousePress;
  bool MidMousePress;
};
