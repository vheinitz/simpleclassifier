#ifndef MY_X_PATH_H
#define MY_X_PATH_H

#include <QString>
#include <QDomDocument>
#include <QDomElement>

#ifdef BUILDING_CL2_DLL
# ifndef CL2_EXPORT
#   define CL2_EXPORT Q_DECL_EXPORT
# endif
#else
# ifndef CL2_EXPORT
//#   define CL2_EXPORT Q_DECL_IMPORT
#	define CL2_EXPORT 
# endif
#endif

class  XPath
{
public:
	static int xpathCount( QDomDocument &doc, QString xpath );
    static QString xpathValue( QDomDocument doc, QString xpath );
	static QString xpathAttrValue( QDomDocument &doc, QString xpath );
	static QDomElement xpathElement( QDomDocument &doc, QString xpath );
};



#endif // MY_X_PATH_H
