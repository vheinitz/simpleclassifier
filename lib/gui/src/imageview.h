#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <Qt>



#ifdef BUILDING_GUI_DLL
#   define GUI_EXPORT Q_DECL_EXPORT
#else
#   define GUI_EXPORT 
#endif


class ObjectSelectorInterface;

class GUI_EXPORT ImageScene : public QGraphicsScene
{
  ObjectSelectorInterface *_selector;
  QImage _image;
  QGraphicsPixmapItem *_imageItem;
public:
  ImageScene(  QObject *p );
  virtual ~ImageScene( );
  void setSelector(ObjectSelectorInterface *s){_selector=s;}
  void setImage( QImage img ); 
  void updateImage( QImage img );
  const QImage & image(  ) const;
  void setMarkingActive( bool );

  void clearItems( );

  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};



//Interface for Selector

class GUI_EXPORT ObjectSelectorInterface 
{

public:
  ObjectSelectorInterface( QGraphicsScene * =0 ){};
  virtual ~ObjectSelectorInterface( ){};
  virtual void reset(){};
  virtual void accept(){};
  virtual void mouseMoved( QPoint ){};
  virtual void mousePressed( QPoint , Qt::MouseButton = Qt::NoButton){};
  virtual void mouseReleased( QPoint ){};
  virtual bool isSelecting() const{return false;}
  virtual void setSelectionColor( QColor ){};
  virtual QColor selectionColor(  )const{return QColor();}
  virtual void setActive( bool ){};
  virtual bool isActive(  )const{return false;}


/*signals:
  void canceled();
  void accepted(QPolygon);
  */
};


#endif // IMAGEVIEW_H
