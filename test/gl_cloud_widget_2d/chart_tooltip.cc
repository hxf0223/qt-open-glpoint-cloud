
#include <QDebug>
#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>

#include "gl_widget_2d/gl_cloud_widget_2d.h"

#include <qglobal.h>
#include "chart_tooltip.h"

namespace test::gl_painter {

Callout::Callout(GLCloudWidget2D* chart, int32_t paintIdx) : QObject(), m_chart(chart), m_seriesIndex(paintIdx) {}

void Callout::paint(QPainter* painter, QPaintEvent* event) {
  Q_UNUSED(event);
  QPainterPath path;
  path.addRoundedRect(m_rect, 5, 5);

  QPointF anchor = m_chart->mapToPosition(m_anchor, m_seriesIndex);
  if (!m_rect.contains(anchor)) {
    QPointF point1, point2;

    // establish the position of the anchor point in relation to m_rect
    bool above = anchor.y() <= m_rect.top();
    bool aboveCenter = anchor.y() > m_rect.top() && anchor.y() <= m_rect.center().y();
    bool belowCenter = anchor.y() > m_rect.center().y() && anchor.y() <= m_rect.bottom();
    bool below = anchor.y() > m_rect.bottom();

    bool onLeft = anchor.x() <= m_rect.left();
    bool leftOfCenter = anchor.x() > m_rect.left() && anchor.x() <= m_rect.center().x();
    bool rightOfCenter = anchor.x() > m_rect.center().x() && anchor.x() <= m_rect.right();
    bool onRight = anchor.x() > m_rect.right();

    // get the nearest m_rect corner.
    qreal x = (onRight + rightOfCenter) * m_rect.right();
    qreal y = (below + belowCenter) * m_rect.bottom();
    bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
    bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

    qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
    qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);
    ;
    point1.setX(x1);
    point1.setY(y1);

    qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);
    qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);
    ;
    point2.setX(x2);
    point2.setY(y2);

    path.moveTo(point1);
    path.lineTo(anchor);
    path.lineTo(point2);
    path = path.simplified();
  }

  painter->save();

  painter->setBrush(QColor(255, 255, 255));
  painter->drawPath(path);
  painter->drawText(m_textRect, m_text);

  painter->restore();
}

void Callout::setText(const QString& text) {
  m_text = text;
  QFontMetrics metrics(m_font);
  m_textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
  m_textRect.translate(5, 5);
  updateRect();
}

void Callout::setAnchor(QPointF point) {
  m_anchor = point;
  updateRect();
}

void Callout::updateRect() {
  QPointF anchor = m_chart->mapToPosition(m_anchor, m_seriesIndex);
  const auto width = m_textRect.width(), height = m_textRect.height();
  double ltx = anchor.x() - width - 30, lty = anchor.y() - height - 20;

  m_textRect = QRectF(ltx, lty, width, height);
  m_rect = m_textRect.adjusted(-5, -5, 5, 5);
}

}  // namespace test::gl_painter