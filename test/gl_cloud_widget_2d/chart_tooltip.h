#pragma once

#include <QFont>
#include <QObject>
#include <QPointF>
#include <QRectF>

class QPainter;
class QPaintEvent;

namespace test::gl_painter {

class GLCloudWidget2D;

class Callout : public QObject {
 public:
  Callout(GLCloudWidget2D* chart, int32_t paintIdx);

  void setText(const QString& text);
  void setAnchor(QPointF point);

  void paint(QPainter* painter, QPaintEvent* event);

 private:
  void updateRect();

 private:
  QString m_text;
  QRectF m_textRect;
  QRectF m_rect;
  QPointF m_anchor;
  QFont m_font;
  GLCloudWidget2D* m_chart;
  int32_t m_seriesIndex;
};

}  // namespace test::gl_painter