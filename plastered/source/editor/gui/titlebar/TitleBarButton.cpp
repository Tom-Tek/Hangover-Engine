#include "TitleBarButton.h"

#include <QPainter>

namespace
{
	// Half the size of a glyph, in device independent pixels
	constexpr int k_GlyphExtent = 5;
}

TitleBarButton::TitleBarButton(Kind kind)
	: m_kind(kind)
{
	switch (m_kind)
	{
	case Kind::Minimize: setObjectName("editor_titleBarMinimize"); break;
	case Kind::Maximize: setObjectName("editor_titleBarMaximize"); break;
	case Kind::Close:    setObjectName("editor_titleBarClose");    break;
	}

	setFocusPolicy(Qt::NoFocus);
}

void TitleBarButton::SetMaximized(bool maximized)
{
	m_maximized = maximized;
	update();
}

void TitleBarButton::paintEvent(QPaintEvent* event)
{
	// Background (hover/pressed) comes from the stylesheet
	QPushButton::paintEvent(event);

	QPainter painter(this);
	painter.setPen(QPen(palette().color(QPalette::ButtonText), 1.0));

	// Offset by half a pixel so 1px lines land on whole pixels
	const QPointF center(width() / 2 + 0.5, height() / 2 + 0.5);
	const qreal e = k_GlyphExtent;

	switch (m_kind)
	{
	case Kind::Minimize:
		painter.drawLine(QPointF(center.x() - e, center.y()), QPointF(center.x() + e, center.y()));
		break;

	case Kind::Maximize:
		if (!m_maximized)
		{
			painter.drawRect(QRectF(center.x() - e, center.y() - e, e * 2, e * 2));
		}
		else
		{
			// Restore: a front window with a second one peeking out behind it
			painter.drawRect(QRectF(center.x() - e, center.y() - e + 2, e * 2 - 2, e * 2 - 2));
			painter.drawPolyline(QPolygonF({
				QPointF(center.x() - e + 2, center.y() - e + 2),
				QPointF(center.x() - e + 2, center.y() - e),
				QPointF(center.x() + e, center.y() - e),
				QPointF(center.x() + e, center.y() + e - 2),
				QPointF(center.x() + e - 2, center.y() + e - 2),
			}));
		}
		break;

	case Kind::Close:
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawLine(QPointF(center.x() - e, center.y() - e), QPointF(center.x() + e, center.y() + e));
		painter.drawLine(QPointF(center.x() - e, center.y() + e), QPointF(center.x() + e, center.y() - e));
		break;
	}
}
