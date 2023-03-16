#ifndef FS_TOOLS__HG
#define FS_TOOLS__HG


#include <QString>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QTextStream>

#include <QtGlobal>
#if QT_VERSION >= 0x060000
    #include <QRegularExpression>
    typedef QRegularExpression QRegExp;
#else
    #include <QRegExp>
#endif


#ifdef BUILDING_OS_DLL
#   define OS_EXPORT Q_DECL_EXPORT
#else
#   define OS_EXPORT
#endif

class OS_EXPORT FSTools{//TODO dummy. medge with changes from home
	public: static long getFreeSpaceInMB( QString , bool *ok=0){ if(ok)*ok=true; return 10000000;}
    public: static QString readAll( QString fn);
    public: static QStringList fromFile( QString fn);
	public: static bool toFile( QStringList sl, QString fn);
	public: static bool toFile( QString sl, QString fn);
	public: static bool touch( QString fn);
	public: static QMap<QString, QString> mapFromFile( QString fn, QRegExp sep);
	public: static QMap<QString, QString> mapFromText( QString data, QRegExp sepkey, QRegExp sepline=QRegExp("(\\r\\n|\\r|\\n)"));
	public: static bool mapToFile( QMap<QString, QString>m,QString fn, QString sep);
	public: static bool freeSpaceCheck();
	public: static bool removeDir(const QString &dirName);
    public: static QString selectDir(const QString &message);

    public: static bool toCsv(QList<double> l, QString fn, QChar sep=',');
	public: static QStringList getImages(QString dialogMessage, QString & startPath);
    public: static QString getImage(QString dialogMessage, QString & startPath);

};

struct IFile : public QFile { IFile( QString fn ):QFile(fn){ open(QIODevice::ReadOnly); } };
struct OFile : public QFile { OFile( QString fn ):QFile(fn){ open(QIODevice::WriteOnly); } };
struct AFile : public QFile { AFile( QString fn ):QFile(fn){ open(QIODevice::Append); } };

class TOStream : public QTextStream
{
	OFile _f;

public:
	TOStream( QString fn ):_f(fn)
	{
		this->setDevice(&_f);
	}
};


class TAStream : public QTextStream
{
	AFile _f;

public:
	TAStream( QString fn ):_f(fn)
	{
		this->setDevice(&_f);
	}
};

#endif
