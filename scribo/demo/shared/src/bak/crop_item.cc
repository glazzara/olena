// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.


#include <shared/src/crop_item.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      crop_item::crop_item(QGraphicsItem *parent)
	: QGraphicsItem(parent)
      {
	_mousePress = false;
	_cropRect = QRectF(20, 20, 100, 70);
	setAcceptHoverEvents(true);

	grabMouse();
      }

      QRectF crop_item::cropRect(void) const
      {
	return(_cropRect);
      }


      QRectF crop_item::boundingRect(void) const
      {
	return(parentItem()->boundingRect());
      }

      void crop_item::paint(QPainter *painter,
			    const QStyleOptionGraphicsItem *option,
			    QWidget *widget)
      {
	Q_UNUSED(widget);

	painter->save();

	QPainterPath cropPath;
	cropPath.addRect(_cropRect);

	QPainterPath windowPath;
	windowPath.addRect(option->rect);
	windowPath = windowPath.subtracted(cropPath);

	// Draw Alpha-Black Background.
	painter->fillPath(windowPath, QColor(0x33, 0x33, 0x33, 0xcc));

	// Draw Crop Rect
	painter->setPen(QPen(QColor(0xdd, 0xdd, 0xdd), 1));
	painter->drawPath(cropPath);

	int topRightX = _cropRect.x() + _cropRect.width();
	int bottomY = _cropRect.y() + _cropRect.height();

	// Draw Grid if mouse is Pressed
	if (_mousePress) {
	  qreal f = 1.0f / CROP_GRID_SIZE;
	  qreal hsize = _cropRect.height() * f;
	  qreal wsize = _cropRect.width() * f;

	  QPainterPath gridPath;
	  for (uint i = 1; i < CROP_GRID_SIZE; ++i) {
	    qreal y = _cropRect.y() + i * hsize;
	    gridPath.moveTo(_cropRect.x(), y);
	    gridPath.lineTo(topRightX, y);

	    for (uint j = 1; j < CROP_GRID_SIZE; ++j) {
	      qreal x = _cropRect.x() + j * wsize;
	      gridPath.moveTo(x, _cropRect.y());
	      gridPath.lineTo(x, bottomY);
	    }
	  }

	  // Draw Grid Path
	  painter->setPen(QPen(QColor(0x99, 0x99, 0x99, 0x80), 1));
	  painter->drawPath(gridPath);
	}

	QPainterPath borderPath;
// 	// Top-Left Corner
// 	borderPath.moveTo(_cropRect.x(), _cropRect.y());
// 	borderPath.lineTo(_cropRect.x() + CROP_BORDER_LINE, _cropRect.y());
// 	borderPath.moveTo(_cropRect.x(), _cropRect.y());
// 	borderPath.lineTo(_cropRect.x(), _cropRect.y() + CROP_BORDER_LINE);

// 	// Top-Right Corner
// 	borderPath.moveTo(topRightX - CROP_BORDER_LINE, _cropRect.y());
// 	borderPath.lineTo(topRightX, _cropRect.y());
// 	borderPath.moveTo(topRightX, _cropRect.y());
// 	borderPath.lineTo(topRightX, _cropRect.y() + CROP_BORDER_LINE);

// 	// Bottom-Left Corner
// 	borderPath.moveTo(_cropRect.x(), bottomY);
// 	borderPath.lineTo(_cropRect.x() + CROP_BORDER_LINE, bottomY);
// 	borderPath.moveTo(_cropRect.x(), bottomY - CROP_BORDER_LINE);
// 	borderPath.lineTo(_cropRect.x(), bottomY);

// 	// Bottom-Left Corner
// 	borderPath.moveTo(topRightX, bottomY);
// 	borderPath.lineTo(topRightX - CROP_BORDER_LINE, bottomY);
// 	borderPath.moveTo(topRightX, bottomY - CROP_BORDER_LINE);
// 	borderPath.lineTo(topRightX, bottomY);

	// Draw Border Path
	painter->setPen(QPen(QColor(0xee, 0xee, 0xee), 3));
	painter->drawPath(borderPath);

	painter->restore();
      }


      void crop_item::mousePressEvent(QGraphicsSceneMouseEvent *event)
      {
	QGraphicsItem::mousePressEvent(event);

	_mousePress = true;
	_cropResize = CropItemResizeNone;
	if (event->buttons() & Qt::LeftButton)
	{
	  int lthreshold = (CROP_BORDER_LINE >> 1);
	  int rthreshold = CROP_BORDER_LINE;
	  int size = CROP_BORDER_LINE << 1;

	  int rightX = _cropRect.x() + _cropRect.width() - rthreshold;
	  int leftX = _cropRect.x() - lthreshold;

	  int bottomY = _cropRect.y() + _cropRect.height() - rthreshold;
	  int topY = _cropRect.y() - lthreshold;

	  QRectF bottomRightCorner(rightX, bottomY, size, size);
	  QRectF bottomLeftCorner(leftX, bottomY, size, size);
	  QRectF topRightCorner(rightX, topY, size, size);
	  QRectF topLeftCorner(leftX, topY, size, size);

	  int sideHeight = _cropRect.height() - 2 * size;
	  int sideWidth = _cropRect.width() - 2 * size;
	  QRectF leftSide(leftX, topY + size, size, sideHeight);
	  QRectF rightSide(rightX, topY + size, size, sideHeight);
	  QRectF topSide(leftX + size, topY, sideWidth, size);
	  QRectF bottomSide(leftX + size, bottomY, sideWidth, size);

	  if (bottomRightCorner.contains(event->pos()))
	    _cropResize = CropItemResizeBottomRight;
	  else if (bottomLeftCorner.contains(event->pos()))
	    _cropResize = CropItemResizeBottomLeft;
	  else if (topRightCorner.contains(event->pos()))
	    _cropResize = CropItemResizeTopRight;
	  else if (topLeftCorner.contains(event->pos()))
	    _cropResize = CropItemResizeTopLeft;
	  else if (leftSide.contains(event->pos()))
	    _cropResize = CropItemResizeLeft;
	  else if (rightSide.contains(event->pos()))
	    _cropResize = CropItemResizeRight;
	  else if (topSide.contains(event->pos()))
	    _cropResize = CropItemResizeTop;
	  else if (bottomSide.contains(event->pos()))
	    _cropResize = CropItemResizeBottom;
	}

	update();
      }


      void crop_item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
      {
	QGraphicsItem::mouseReleaseEvent(event);

	_cropResize = CropItemResizeNone;
	_mousePress = false;

	update();
      }


      void crop_item::updateCursor(QGraphicsSceneMouseEvent *event)
      {
	int lthreshold = (CROP_BORDER_LINE >> 1);
	int rthreshold = CROP_BORDER_LINE;
	int size = CROP_BORDER_LINE << 1;

	int rightX = _cropRect.x() + _cropRect.width() - rthreshold;
	int leftX = _cropRect.x() - lthreshold;

	int bottomY = _cropRect.y() + _cropRect.height() - rthreshold;
	int topY = _cropRect.y() - lthreshold;

	QRectF bottomRightCorner(rightX, bottomY, size, size);
	QRectF bottomLeftCorner(leftX, bottomY, size, size);
	QRectF topRightCorner(rightX, topY, size, size);
	QRectF topLeftCorner(leftX, topY, size, size);

	int sideHeight = _cropRect.height() - 2 * size;
	int sideWidth = _cropRect.width() - 2 * size;
	QRectF leftSide(leftX, topY + size, size, sideHeight);
	QRectF rightSide(rightX, topY + size, size, sideHeight);
	QRectF topSide(leftX + size, topY, sideWidth, size);
	QRectF bottomSide(leftX + size, bottomY, sideWidth, size);

	if (bottomRightCorner.contains(event->pos()))
	  setCursor(Qt::SizeFDiagCursor);
	else if (bottomLeftCorner.contains(event->pos()))
	  setCursor(Qt::SizeBDiagCursor);
	else if (topRightCorner.contains(event->pos()))
	  setCursor(Qt::SizeBDiagCursor);
	else if (topLeftCorner.contains(event->pos()))
	  setCursor(Qt::SizeFDiagCursor);
	else if (leftSide.contains(event->pos()))
	  setCursor(Qt::SizeHorCursor);
	else if (rightSide.contains(event->pos()))
	  setCursor(Qt::SizeHorCursor);
	else if (topSide.contains(event->pos()))
	  setCursor(Qt::SizeVerCursor);
	else if (bottomSide.contains(event->pos()))
	  setCursor(Qt::SizeVerCursor);
      }


      void crop_item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
      {
	updateCursor(event);

	QGraphicsItem::mouseMoveEvent(event);

	qreal minSize = 4 + (CROP_BORDER_LINE << 1);

	QPointF delta = event->pos() - event->lastPos();
	switch (_cropResize)
	{
	  case CropItemResizeNone:
	    if (!_cropRect.contains(event->pos()))
	      return;

	    setCursor(Qt::SizeAllCursor);

	    if (!(event->buttons() & Qt::LeftButton))
	      return;

	    _cropRect.translate(delta);
	    break;
	  case CropItemResizeTopLeft:
	    delta.setY(qMin(_cropRect.height() - minSize, delta.y()));
	    delta.setX(qMin(_cropRect.width() - minSize, delta.x()));
	    _cropRect.setHeight(_cropRect.height() - delta.y());
	    _cropRect.setWidth(_cropRect.width() - delta.x());
	    _cropRect.translate(delta);
	    setCursor(Qt::SizeFDiagCursor);
	    break;
	  case CropItemResizeTopRight:
	    delta.setY(qMin(_cropRect.height() - minSize, delta.y()));
	    _cropRect.setWidth(_cropRect.width() + delta.x());
	    _cropRect.setHeight(_cropRect.height() - delta.y());
	    _cropRect.translate(0, delta.y());
	    setCursor(Qt::SizeBDiagCursor);
	    break;
	  case CropItemResizeBottomLeft:
	    delta.setX(qMin(_cropRect.width() - minSize, delta.x()));
	    _cropRect.setHeight(_cropRect.height() + delta.y());
	    _cropRect.setWidth(_cropRect.width() - delta.x());
	    _cropRect.translate(delta.x(), 0);
	    setCursor(Qt::SizeBDiagCursor);
	    break;
	  case CropItemResizeBottomRight:
	    _cropRect.setWidth(_cropRect.width() + delta.x());
	    _cropRect.setHeight(_cropRect.height() + delta.y());
	    setCursor(Qt::SizeFDiagCursor);
	    break;
	  case CropItemResizeLeft:
	    delta.setX(qMin(_cropRect.width() - minSize, delta.x()));
	    _cropRect.setWidth(_cropRect.width() - delta.x());
	    _cropRect.translate(delta.x(), 0);
	    setCursor(Qt::SizeHorCursor);
	    break;
	  case CropItemResizeRight:
	    _cropRect.setWidth(_cropRect.width() + delta.x());
	    setCursor(Qt::SizeHorCursor);
	    break;
	  case CropItemResizeTop:
	    delta.setY(qMin(_cropRect.height() - minSize, delta.y()));
	    _cropRect.setHeight(_cropRect.height() - delta.y());
	    _cropRect.translate(0, delta.y());
	    setCursor(Qt::SizeVerCursor);
	    break;
	  case CropItemResizeBottom:
	    _cropRect.setHeight(_cropRect.height() + delta.y());
	    setCursor(Qt::SizeVerCursor);
	    break;
	}

	if (_cropRect.width() < minSize)
	  _cropRect.setWidth(minSize);
	if (_cropRect.height() < minSize)
	  _cropRect.setHeight(minSize);

	update();
      }


    }  // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo

