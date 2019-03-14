#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>

/*!
 * This class adds ability to zoom QGraphicsView using mouse wheel. The point under cursor
 * remains motionless while it's possible.
 *
 * Note that it becomes not possible when the scene's
 * size is not large enough comparing to the viewport size. QGraphicsView centers the picture
 * when it's smaller than the view. And QGraphicsView's scrolls boundaries don't allow to
 * put any picture point at any viewport position.
 *
 * When the user starts scrolling, this class remembers original scene position and
 * keeps it until scrolling is completed. It's better than getting original scene position at
 * each scrolling step because that approach leads to position errors due to before-mentioned
 * positioning restrictions.
 *
 * When zommed using scroll, this class emits zoomed() signal.
 *
 * Usage:
 *
 *   new Graphics_view_zoom(view);
 *
 * The object will be deleted automatically when the view is deleted.
 *
 * You can set keyboard modifiers used for zooming using set_modified(). Zooming will be
 * performed only on exact match of modifiers combination. The default modifier is Ctrl.
 *
 * You can change zoom velocity by calling set_zoom_factor_base().
 * Zoom coefficient is calculated as zoom_factor_base^angle_delta
 * (see QWheelEvent::angleDelta).
 * The default zoom factor base is 1.0015.
 */
class ImageView : public QGraphicsView
{
  Q_OBJECT

  double zoomFactorBbase;
  QPointF targetScenePos, targetViewportPos;

  bool eventFilter(QObject* object, QEvent* event);


public:
  ImageView(QWidget *parent);
  void gentleZoom(double factor);
  void setZoomFactorBase(double value);

public Q_SLOTS :
  void zoom1to1();
  void zoomIn();
  void zoomOut();
  void fitToWindow(bool yes);



Q_SIGNALS:
  void zoomed(bool);
};


#endif // IMAGEVIEW_H
