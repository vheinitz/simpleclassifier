#include "fstools.h"
#include <QIODevice>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QFileInfoList>
#include <QFileDialog>

QString FSTools::readAll( QString fn)
{
    QFile f(fn);
    if(!f.open(QIODevice::ReadOnly))
    {
        return QString();
    }

	return f.readAll();
}


QStringList FSTools::fromFile( QString fn)
{
    QStringList ret;
    QFile f(fn);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    while (!ts.atEnd())
    {
        ret.append(ts.readLine());
    }
    return ret;
}

bool FSTools::toFile( QStringList sl, QString fn)
{
    QFile f(fn);
    if (!f.open(QIODevice::WriteOnly))
		return false;

    QTextStream ts(&f);
    foreach (QString s, sl)
    {
        ts << s <<"\n";
    }
    return true;
}

bool FSTools::toFile( QString s, QString fn)
{
    QFile f(fn);
    if (!f.open(QIODevice::WriteOnly))
		return false;

    QTextStream ts(&f);
   
    ts << s;

    return true;
}

bool FSTools::touch( QString fn)
{
    QFile f(fn);
    if (!f.open(QIODevice::WriteOnly))
		return false;
    return true;
}

bool FSTools::toCsv( QList<double> l, QString fn, QChar sep)
{
    QFile f(fn);
    if (!f.open(QIODevice::WriteOnly))
		return false;

    QTextStream ts(&f);
    foreach (double e, l)
    {
        ts << e << sep;
    }
	ts << "\n";
    return true;
}

QString FSTools::getImage(QString dialogMessage, QString & startPath)
{
    QString res = QFileDialog::getOpenFileName(0, dialogMessage, startPath, "*.png;*jpg;*.bpm;*.tif");

	if (!res.isEmpty())
    {
        startPath = QFileInfo( res ).canonicalPath();
    }

    return res;
}

QStringList FSTools::getImages(QString dialogMessage, QString & startPath)
{
    QStringList res = QFileDialog::getOpenFileNames(0, dialogMessage, startPath, "*.png;*jpg;*.bpm;*.tif");

    if (res.size() > 0)
    {
        startPath = QFileInfo( res.at(0) ).canonicalPath();
    }

    return res;
}

QMap<QString, QString> FSTools::mapFromFile( QString fn, QRegExp sep)
{
	QMap<QString, QString> ret;
    QFile f(fn);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    while (!ts.atEnd())
    {
		QString line = ts.readLine();
		QString k = line.section(sep,0,0).trimmed();
		if (k.isEmpty())
		{
			continue;
		}
		QString v = line.section(sep,1);
        ret[k] = v;
    }
    return ret;
}

QMap<QString, QString> FSTools::mapFromText( QString data, QRegExp sepkey, QRegExp sepline)
{
	QMap<QString, QString> ret;
	QStringList lines = data.split( sepline );
    foreach ( QString line, lines)
    {
		QString k = line.section(sepkey,0,0).trimmed();
		if (k.isEmpty())
		{
			continue;
		}
		QString v = line.section(sepkey,1);
        ret[k] = v;
    }
    return ret;
}

bool FSTools::mapToFile( QMap<QString, QString> m, QString fn, QString sep)
{
	QFile f(fn);
	if (!f.open(QIODevice::WriteOnly))
		return false;

    QTextStream ts(&f);
	foreach (QString mk, m.keys())
    {
		ts << mk<<sep<<m[mk]<<"\n";
    }
    return true;
}

bool FSTools::freeSpaceCheck()
{
	QString wlOutputDrive = "C";
	bool ok=0;
	wlOutputDrive+=":";
	long freeSpace = FSTools::getFreeSpaceInMB( wlOutputDrive , &ok);
 
	if ( !ok || (freeSpace < 1000) )
	{
		int but= QMessageBox::warning( 0,  "Warning", "Left space on hard drive for storing images may be insufficient",QMessageBox::Ok | QMessageBox::Abort );
		if ( but == QMessageBox::Abort )
		{
			return false;
		}
	}
	return true;
}

bool FSTools::removeDir(const QString &dirName)
{
    QDir dir(dirName);	 
    if (dir.exists(dirName))
	{
		QFileInfoList fil = dir.entryInfoList(
			(
				QDir::NoDotAndDotDot | 
				QDir::Files |
				QDir::System | 
				QDir::Hidden  | 
				QDir::AllDirs
			),			 
			QDir::DirsFirst
		);

        foreach(QFileInfo fi, fil ) {
            if (fi.isDir()) {
                if ( !removeDir(fi.absoluteFilePath()) )
					return false;
            }
            else {
                if ( !QFile::remove(fi.absoluteFilePath()) )
					return false;
            }
 
        }
        return dir.rmdir(dirName);
    }
 
    return false;
}


QString FSTools::selectDir(const QString &message)
{
    return QFileDialog::getExistingDirectory(0,message);
}
