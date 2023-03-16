#ifndef SI__Server_HG
#define SI__Server_HG


#include <QtNetwork>
#include <QtCore>

#ifdef BUILDING_OS_DLL
#   define OS_EXPORT Q_DECL_EXPORT
#else
#   define OS_EXPORT
#endif


#include <QtGlobal>
#if QT_VERSION >= 0x060000
    #include <QRegularExpression>
    typedef QRegularExpression QRegExp;
#else
    #include <QRegExp>
#endif


class OS_EXPORT SIServer : public QObject
{
    Q_OBJECT
    QTcpServer _server;
	unsigned short _port;
    QTcpSocket *_serverConnection;

signals:
	void request( QString );
private slots:
    void onConnnected();
    void onError ( QAbstractSocket::SocketError );
    void onDataRead();
public:
    SIServer(QObject*p=0);
	bool start( unsigned short port );
    void stop( );
	void disconnectCurrentClient( );
	void forwardReqiest( QString );	
	virtual ~SIServer();
};

#endif
