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
	mousePress_ = false;
	setAcceptHoverEvents(true);
// 	draw_rect_ = scene()->sceneRect();

	reset();

	grabMouse();
      }


      // Mouse is pressed. The rectangle must be created and about to
      // be enlarged.
      crop_item::crop_item(const QPointF& p, QGraphicsItem *parent)
	: QGraphicsItem(parent)
      {
	mousePress_ = true;
	setAcceptHoverEvents(true);

// 	draw_rect_ = scene()->sceneRect();

	cropRect_ = QRectF(p, QSize(4, 4));
	cropResize_ = CropItemResizeBottomRight;

	grabMouse();
      }

      crop_item::~crop_item()
      {

      }

      void crop_item::reset()
      {
	cropRect_ = QRect();
//         if (parentItem())
//           cropRect_ = parentItem()->boundingRect();
// 	else
// 	  cropRect_ = QRectF(20, 20, 100, 70);
      }

      QRectF crop_item::cropRect() const
      {
	return mapToScene(cropRect_).boundingRect();
      }


      QRectF crop_item::boundingRect() const
      {
	return scene()->sceneRect();
//	return draw_rect_;
      }

      void crop_item::paint(QPainter *painter,
			    const QStyleOptionGraphicsItem *option,
			    QWidget *widget)
      {
	Q_UNUSED(widget);

	painter->save();

	QPainterPath cropPath;
	cropPath.addRect(cropRect_);

	QPainterPath windowPath;
	windowPath.addRect(option->rect);
//	windowPath.addRect(draw_rect_);
	windowPath = windowPath.subtracted(cropPath);

	// Draw Alpha-Black Background.
	painter->fillPath(windowPath, QColor(0x33, 0x33, 0x33, 0xcc));

	// Draw Crop Rect
	// QColor(0xdd, 0xdd, 0xdd)
	painter->setPen(QPen(Qt::magenta, 3));
	painter->drawPath(cropPath);

	int topRightX = cropRect_.x() + cropRect_.width();
	int bottomY = cropRect_.y() + cropRect_.height();

	QPainterPath borderPath;
	int corner_width = std::min(int(cropRect_.width() / 6.f), 80);
	int corner_height = std::min(int(cropRect_.height() / 6.f), 80);

	// Top-Left Corner
	painter->drawRect(QRectF(cropRect_.x(), cropRect_.y(),
				 corner_width, corner_height));


 	// Top-Right Corner
	painter->drawRect(QRectF(topRightX - corner_width, cropRect_.y(),
				 corner_width, corner_height));

 	// Bottom-Left Corner
	painter->drawRect(QRectF(cropRect_.x(), bottomY - corner_height,
				 corner_width, corner_height));

 	// Bottom-Left Corner
	painter->drawRect(QRectF(topRightX - corner_width,
				 bottomY - corner_height,
				 corner_width, corner_height));

	// Draw Border Path
	painter->setPen(QPen(QColor(0xee, 0xee, 0xee), 3));
	painter->drawPath(borderPath);

	painter->restore();
      }


      void crop_item::mousePressEvent(QGraphicsSceneMouseEvent *event)
      {
	QGraphicsItem::mousePressEvent(event);

	mousePress_ = true;
	cropResize_ = CropItemResizeNone;
	if (event->buttons() & Qt::LeftButton)
	{
	  if (!cropRect_.isValid())
	  {
	    cropRect_ = QRectF(event->pos(), QSize(4, 4));
	    cropResize_ = CropItemResizeBottomRight;
	    update();
	    return;
	  }


	  int wsize = std::min(int(cropRect_.width() / 6.f), 80);
	  int hsize = std::min(int(cropRect_.height() / 6.f), 80);

	  int rightX = cropRect_.x() + cropRect_.width() - wsize;
	  int leftX = cropRect_.x();

	  int bottomY = cropRect_.y() + cropRect_.height() - hsize;
	  int topY = cropRect_.y();

	  QRectF bottomRightCorner(rightX, bottomY, wsize, hsize);
	  QRectF bottomLeftCorner(leftX, bottomY, wsize, hsize);
	  QRectF topRightCorner(rightX, topY, wsize, hsize);
	  QRectF topLeftCorner(leftX, topY, wsize, hsize);

	  int sideHeight = cropRect_.height() - 2 * hsize;
	  int sideWidth = cropRect_.width() - 2 * wsize;
	  QRectF leftSide(leftX, topY + hsize, wsize, sideHeight);
	  QRectF rightSide(rightX, topY + hsize, wsize, sideHeight);
	  QRectF topSide(leftX + wsize, topY, sideWidth, hsize);
	  QRectF bottomSide(leftX + wsize, bottomY, sideWidth, hsize);

	  if (bottomRightCorner.contains(event->pos()))
	    cropResize_ = CropItemResizeBottomRight;
	  else if (bottomLeftCorner.contains(event->pos()))
	    cropResize_ = CropItemResizeBottomLeft;
	  else if (topRightCorner.contains(event->pos()))
	    cropResize_ = CropItemResizeTopRight;
	  else if (topLeftCorner.contains(event->pos()))
	    cropResize_ = CropItemResizeTopLeft;
	  else if (leftSide.contains(event->pos()))
	    cropResize_ = CropItemResizeLeft;
	  else if (rightSide.contains(event->pos()))
	    cropResize_ = CropItemResizeRight;
	  else if (topSide.contains(event->pos()))
	    cropResize_ = CropItemResizeTop;
	  else if (bottomSide.contains(event->pos()))
	    cropResize_ = CropItemResizeBottom;
	}

	update();
      }


      void crop_item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
      {
	QGraphicsItem::mouseReleaseEvent(event);

	cropResize_ = CropItemResizeNone;
	mousePress_ = false;

	update();
      }


//       void crop_item::updateCursor(QGraphicsSceneMouseEvent *event)
//       {
// 	int lthreshold = (CROP_BORDER_LINE >> 1);
// 	int rthreshold = CROP_BORDER_LINE;
// 	int size = CROP_BORDER_LINE << 1;

// 	int rightX = cropRect_.x() + cropRect_.width() - rthreshold;
// 	int leftX = cropRect_.x() - lthreshold;

// 	int bottomY = cropRect_.y() + cropRect_.height() - rthreshold;
// 	int topY = cropRect_.y() - lthreshold;

// 	QRectF bottomRightCorner(rightX, bottomY, size, size);
// 	QRectF bottomLeftCorner(leftX, bottomY, size, size);
// 	QRectF topRightCorner(rightX, topY, size, size);
// 	QRectF topLeftCorner(leftX, topY, size, size);

// 	int sideHeight = cropRect_.height() - 2 * size;
// 	int sideWidth = cropRect_.width() - 2 * size;
// 	QRectF leftSide(leftX, topY + size, size, sideHeight);
// 	QRectF rightSide(rightX, topY + size, size, sideHeight);
// 	QRectF topSide(leftX + size, topY, sideWidth, size);
// 	QRectF bottomSide(leftX + size, bottomY, sideWidth, size);

// 	if (bottomRightCorner.contains(event->pos()))
// 	  setCursor(Qt::SizeFDiagCursor);
// 	else if (bottomLeftCorner.contains(event->pos()))
// 	  setCursor(Qt::SizeBDiagCursor);
// 	else if (topRightCorner.contains(event->pos()))
// 	  setCursor(Qt::SizeBDiagCursor);
// 	else if (topLeftCorner.contains(event->pos()))
// 	  setCursor(Qt::SizeFDiagCursor);
// 	else if (leftSide.contains(event->pos()))
// 	  setCursor(Qt::SizeHorCursor);
// 	else if (rightSide.contains(event->pos()))
// 	  setCursor(Qt::SizeHorCursor);
// 	else if (topSide.contains(event->pos()))
// 	  setCursor(Qt::SizeVerCursor);
// 	else if (bottomSide.contains(event->pos()))
// 	  setCursor(Qt::SizeVerCursor);
//       }


      void crop_item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
      {
//	updateCursor(event);

	QGraphicsItem::mouseMoveEvent(event);

	QPointF delta = event->pos() - event->lastPos();

	if (cropResize_ == CropItemResizeNone)
	{
	  if (//!cropRect_.contains(event->pos())
	      (!(event->buttons() & Qt::LeftButton)))
	    return;

	  // Moving existing rectangle
	  setCursor(Qt::SizeAllCursor);
	  cropRect_.translate(delta);

//	  draw_rect_.translate(delta);
	  update();
	  return;
	}

	resize(delta);
      }


      void crop_item::resize(const QPointF& delta_)
      {
	qreal minSize = 4 + (CROP_BORDER_LINE << 1);
	QPointF delta = delta_;

	QRectF tmp = cropRect_;
	switch (cropResize_)
	{
	  case CropItemResizeTopLeft:
	    delta.setY(qMin(cropRect_.height() - minSize, delta.y()));
	    delta.setX(qMin(cropRect_.width() - minSize, delta.x()));
	    cropRect_.setHeight(cropRect_.height() - delta.y());
	    cropRect_.setWidth(cropRect_.width() - delta.x());
	    cropRect_.translate(delta);
	    setCursor(Qt::SizeFDiagCursor);
	    break;
	  case CropItemResizeTopRight:
	    delta.setY(qMin(cropRect_.height() - minSize, delta.y()));
	    cropRect_.setWidth(cropRect_.width() + delta.x());
	    cropRect_.setHeight(cropRect_.height() - delta.y());
	    cropRect_.translate(0, delta.y());
	    setCursor(Qt::SizeBDiagCursor);
	    break;
	  case CropItemResizeBottomLeft:
	    delta.setX(qMin(cropRect_.width() - minSize, delta.x()));
	    cropRect_.setHeight(cropRect_.height() + delta.y());
	    cropRect_.setWidth(cropRect_.width() - delta.x());
	    cropRect_.translate(delta.x(), 0);
	    setCursor(Qt::SizeBDiagCursor);
	    break;

	  default:
	  case CropItemResizeBottomRight:
	    cropRect_.setWidth(cropRect_.width() + delta.x());
	    cropRect_.setHeight(cropRect_.height() + delta.y());
	    setCursor(Qt::SizeFDiagCursor);
	    break;

	  case CropItemResizeLeft:
	    delta.setX(qMin(cropRect_.width() - minSize, delta.x()));
	    cropRect_.setWidth(cropRect_.width() - delta.x());
	    cropRect_.translate(delta.x(), 0);
	    setCursor(Qt::SizeHorCursor);
	    break;
	  case CropItemResizeRight:
	    cropRect_.setWidth(cropRect_.width() + delta.x());
	    setCursor(Qt::SizeHorCursor);
	    break;
	  case CropItemResizeTop:
	    delta.setY(qMin(cropRect_.height() - minSize, delta.y()));
	    cropRect_.setHeight(cropRect_.height() - delta.y());
	    cropRect_.translate(0, delta.y());
	    setCursor(Qt::SizeVerCursor);
	    break;
	  case CropItemResizeBottom:
	    cropRect_.setHeight(cropRect_.height() + delta.y());
	    setCursor(Qt::SizeVerCursor);
	    break;
	}

	if (cropRect_.width() < minSize)
	  cropRect_.setWidth(minSize);
	if (cropRect_.height() < minSize)
	  cropRect_.setHeight(minSize);


	// Updating bounding rect.
//  	if ((cropRect_.width() * cropRect_.height()) < (tmp.width() * tmp.height()))
//  	  draw_rect_ = tmp;
//  	else
// 	draw_rect_ = cropRect_;

	update();
      }


      void crop_item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
      {
	QGraphicsItem::mouseDoubleClickEvent(event);

	switch (cropResize_)
	{
	  case CropItemResizeNone:
	    if (!cropRect_.contains(event->pos()))
	      return;

	    setCursor(Qt::SizeAllCursor);

	    if (!(event->buttons() & Qt::LeftButton))
	      return;

	    emit ready_for_crop();
	    break;

	  default:
	    break;
	}
      }

    }  // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo

