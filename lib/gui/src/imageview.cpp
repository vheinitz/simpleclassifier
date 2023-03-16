#include "imageview.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QList>
#include <QPoint>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QPolygon>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QPolygon>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>


ImageScene::ImageScene( QObject *p ) :QGraphicsScene(p),_imageItem(0),_selector(0)
{
}

ImageScene::~ImageScene( )
{

}

void ImageScene::setMarkingActive( bool m )
{
	_selector->setActive( m );
}

void ImageScene::keyReleaseEvent(QKeyEvent *event)
{
  switch(  event->key() )
  {
      case Qt::Key_Escape:
      {
        if ( _selector ) _selector->reset();
        event->accept();
      }break;

    case Qt::Key_Enter:
      {
        if ( _selector ) _selector->accept();
        event->accept();
      }break;
  }
}

void ImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if ( !_selector ) return;

	QPointF p = event->scenePos();
	if ( !sceneRect().contains( event->scenePos() ) )
	{
		event->accept();
		return;
	}
	_selector->mouseMoved( event->scenePos().toPoint() );
	event->accept();
}

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if ( !_selector ) return;

	QPointF p = event->scenePos();
	_selector->mousePressed( event->scenePos().toPoint(), event->button() );
	 event->accept();

}

void ImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ( !_selector ) return;

	_selector->mouseReleased( event->scenePos().toPoint() );
	event->accept();
}

const QImage & ImageScene::image(  ) const 
{
	return _image;
}

void ImageScene::updateImage( QImage img )
{	
	_image = img; //TODO required?
	QPixmap p = QPixmap::fromImage(  _image  );
	if (_imageItem){		
		_imageItem->setPixmap(p);
	}
	else {
		_imageItem = addPixmap( p );
	}	
	this->setSceneRect( QRect(QPoint(0,0),_image.size()) );
}

void ImageScene::setImage( QImage img )
{	
	updateImage( img );
	clearItems( );
}

void ImageScene::clearItems( )
{	
	foreach( QGraphicsItem *it , this->items() )
	{
		if ( it != _imageItem )
			this->removeItem(it);
	}
    _imageItem=0;
}
