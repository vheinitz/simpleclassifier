#ifndef EXTIMAGEVIEWER_H
#define EXTIMAGEVIEWER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>


#ifdef BUILDING_GUI_DLL
#   define GUI_EXPORT Q_DECL_EXPORT
#else
#   define GUI_EXPORT 
#endif



class GUI_EXPORT ExtImageViewer : public QWidget
{
    Q_OBJECT
public:
    ExtImageViewer(QWidget *parent = 0);
    void setImage( QImage );
	QGraphicsView *_view;
signals:
    
public slots:
    void loadImage( QString );
    
private:
    QImage _currentImage;
    QGraphicsScene _imageScene;
    
    double _currentScaleFactor;
};

#endif // EXTIMAGEVIEWER_H
