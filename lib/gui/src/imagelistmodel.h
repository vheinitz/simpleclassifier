#ifndef _IMAGE_LIST_MODEL__HG
#define _IMAGE_LIST_MODEL__HG


#include <QAbstractListModel>
#include <QList>
#include <QImage>
#include <QStyledItemDelegate>


#ifdef BUILDING_GUI_DLL
#   define GUI_EXPORT Q_DECL_EXPORT
#else
#   define GUI_EXPORT 
#endif
 /*! \brief List model for images
  *
  * 
  */
class GUI_EXPORT ImageListModel : public QAbstractListModel
{
	Q_OBJECT

    QList<QImage> _data;

    void reset(){};
public slots:
        void clear();
public:

	QList<QImage> & imgData () {return _data;}
	
	void addImage( QImage img )
	{
		_data.append( img );
        _viewImage.append(QImage());
        reset();
	}
    mutable QList<QImage> _viewImage;
	
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

class GUI_EXPORT ImageDelegate : public QStyledItemDelegate
{
	QAbstractItemView *_view;

public:
	ImageDelegate(QObject *parent = 0, QAbstractItemView *view = 0) : QStyledItemDelegate(parent), _view(view) { }
	~ImageDelegate() { }

	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
