#include "imgdb.h"
#include <QRgb>
#include <QPainter>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>


ImageDatabase::ImageDatabase( )
{

}

bool ImageDatabase::setRoot( QString rootPath)
{
	if( rootPath.isEmpty() )
    	return false;

    _rootPath = rootPath;
    QDir().mkpath( _rootPath );
    QDir().mkpath( _rootPath+"/img" );
    QDir().mkpath( _rootPath+"/preview" );
    QDir().mkpath( _rootPath+"/tn" );
    return true;
}

QStringList ImageDatabase::images()
{
    QStringList tmp;
    foreach( QString f,  QDir(_rootPath + "/img" ).entryList( QStringList()<<"*.png" ) )
    {
        tmp << QFileInfo(f).baseName();
    }
    return tmp;
}

QString ImageDatabase::getImagePath( QString imghash )
{
    QString imgpath = QString("%1/img/%2.png").arg( _rootPath, imghash );
    if( !QFileInfo(imgpath).exists() )
        return "";
    return imgpath;
}

QString ImageDatabase::getPreviewPath( QString imghash )
{
    QString imgpath = QString("%1/preview/%2_pv.jpg").arg( _rootPath, imghash );
    if( !QFileInfo(imgpath).exists() )
        return "";
    return imgpath;
}

QString ImageDatabase::getThumbnailPath( QString imghash )
{
    QString imgpath = QString("%1/tn/%2_tn.jpg").arg( _rootPath, imghash );
    if( !QFileInfo(imgpath).exists() )
        return "";
    return imgpath;
}


QImage ImageDatabase::getImage( QString imghash )
{
    return QImage(getImagePath(imghash));
}

QImage ImageDatabase::getPreview( QString imghash )
{
    return QImage(getPreviewPath(imghash));
}

QImage ImageDatabase::getThumbnail( QString imghash )
{
    return QImage(getThumbnailPath(imghash));
}

QString ImageDatabase::getImageHash( QString img  )
{
    QFile isf( img );
    isf.open(QIODevice::ReadOnly);
    return QString(QCryptographicHash::hash( isf.readAll() ,QCryptographicHash::Md5).toHex());
}

bool ImageDatabase::removeImage( QString imghash )
{
    QString imgDstn = QString("%1/img/%2.png").arg( _rootPath, imghash );
    bool ok=true;
    ok &= QFileInfo(imgDstn).exists();
    ok &= QFile::remove( imgDstn );
    imgDstn = QString("%1/preview/%2_pv.jpg").arg( _rootPath, imghash );
    ok &= QFile::remove( imgDstn );
    imgDstn = QString("%1/tn/%2_tn.jpg").arg( _rootPath, imghash );
    ok &= QFile::remove( imgDstn );
    return ok;
}

bool ImageDatabase::contains( QString hash )
{
    return _added.contains(hash);
}


QString ImageDatabase::addImage( QImage img, bool *existed )
{
    int size = img.bytesPerLine()*img.height();
	QByteArray data((const char*)img.bits(),size);
	QByteArray hdata = QCryptographicHash::hash( data, QCryptographicHash::Md5);
	QByteArray xdata = hdata.toHex();

	QString imghash = QString( xdata );
 
    QString imgDstn = QString("%1/img/%2.png").arg( _rootPath, imghash );

    if ( QFileInfo(imgDstn).exists()  )
    {
        if (existed)
            *existed = true;
    }
    else
    {
        if (existed)
            *existed = false;

        QImage tmpimg(img);
        if ( !tmpimg.save(imgDstn) )
        {
            return ""; // is error
        }


		QImage preview = tmpimg.scaled( 800,600,Qt::KeepAspectRatio );
        imgDstn = QString("%1/preview/%2_pv.jpg").arg( _rootPath, imghash );
        preview.save( imgDstn );

        QImage tn = preview.scaled( 160,120,Qt::KeepAspectRatio );
        imgDstn = QString("%1/tn/%2_tn.jpg").arg( _rootPath, imghash );
        tn.save( imgDstn );
    }

    return imghash;
}


QString ImageDatabase::addImage( QString img, bool *existed )
{
    QFile isf( img );
    isf.open(QIODevice::ReadOnly);
    QString imghash = QString(QCryptographicHash::hash( isf.readAll() ,QCryptographicHash::Md5).toHex());
    isf.close();

    QString imgDstn = QString("%1/img/%2.png").arg( _rootPath, imghash );

    if ( QFileInfo(imgDstn).exists()  )
    {
        if (existed)
            *existed = true;
    }
    else
    {
        if (existed)
            *existed = false;

        QImage tmpimg(img);
        if ( !tmpimg.save(imgDstn) )
        {
            return ""; // is error
        }


        QImage preview = tmpimg.scaled( 800,600 );
        imgDstn = QString("%1/preview/%2_pv.jpg").arg( _rootPath, imghash );
        preview.save( imgDstn );

        QImage tn = preview.scaled( 160,120 );
        imgDstn = QString("%1/tn/%2_tn.jpg").arg( _rootPath, imghash );
        tn.save( imgDstn );
    }

    return imghash;
}

QStringList ImageDatabase::addImages( QStringList images )
{
    QStringList result;
    foreach( QString img, images )
    {
        QString hash = addImage(img);
        if (!hash.isEmpty())
        {
            result.append(hash);
        }
    }
    return result;
}


QImage ImageCache::getImage( QImage &img, QSize s )
{
    QMutexLocker dbml(&_mapMx);
    //test without return img;
    QString hash = img.text( "hashkey" );
    if ( img.width() < 2560 )
    {
        return img.scaled(s);
    }
    else if (hash.isEmpty())
    {
        hash = QCryptographicHash::hash( QByteArray( (const char *)img.bits(), img.bytesPerLine()*img.height()), QCryptographicHash::Md5).toBase64();
        img.setText( "hashkey", hash );
        QImage small = img.scaled(s);
        QString hash_s = QString( "%1_%2_%3" ).arg(hash).arg(s.width()).arg(s.height());
        small.setText( "hashkey", hash_s );
        _cachedImages.insert( hash_s, small );
        return small;
    }
    else
    {
        QString key = QString( "%1_%2_%3" ).arg(hash).arg(s.width()).arg(s.height());
        if ( _cachedImages.contains(key) )
            return _cachedImages[key];

        QImage small = img.scaled(s);
        _cachedImages.insert( QString( "%1_%2_%3" ).arg(hash).arg(s.width()).arg(s.height()), small );
        return small;
    }
}

//TODO
QImage ImageCache::getImage( QString imgFileName, QSize  )
{
    return QImage();
}

