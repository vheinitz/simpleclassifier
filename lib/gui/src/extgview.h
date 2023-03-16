#ifndef _EXTENDED_GRAPHICS_VIEW__HG
#define _EXTENDED_GRAPHICS_VIEW__HG

#include <QGraphicsView>
#include <QMouseEvent>
#include <QPointF>

#ifdef BUILDING_GUI_DLL
#   define GUI_EXPORT Q_DECL_EXPORT
#else
#   define GUI_EXPORT 
#endif

 /*! \brief Extended grafics view
  *
  * Able to handle and report mouse events
  */

class GUI_EXPORT ExtGraphicsView : public QGraphicsView
{
    Q_OBJECT
signals:
    void clicked( QPointF );
	void pressed( QPointF );
    void mouseOver( QPointF );

public:
    ExtGraphicsView(QWidget *p=0) :
            QGraphicsView(p)
    {
        setMouseTracking(true);
    }
protected:
    void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent ( QMouseEvent * event );
};

#endif
