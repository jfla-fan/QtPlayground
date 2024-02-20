#include <QCoreApplication>
#include <QHttpServer>
#include <QPointer>
#include <QtAssert>
#include <QTcpServer>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QHttpMultipart>

#include "db/fileStorage.h"
#include "serverApi.h"


using namespace Qt::StringLiterals;

static const QString HOST = QHostAddress(QHostAddress::LocalHost).toString();
static const quint16 PORT = 8080;



QString GetDatabasePath()
{
    return "";
}

namespace net
{
    class Server
    {
        using ServerPtr = QPointer< QHttpServer >;
    public:
        Server(ServerApiPtr serverApi, ServerPtr server)
            : m_serverApi(serverApi)
            , m_server(server)
        {
        }

        void initialize()
        {
            buildRoutes();
        }

        bool run()
        {
            const auto port = m_server->listen(QHostAddress::Any, PORT);
            if (!port)
            {
                qWarning() << QCoreApplication::tr("QHttpServer", "Server failed to listen on a port.");
                return false;
            }
            else
            {
                qInfo() << "Server ports: ";
                foreach(qint16 port, m_server->serverPorts())
                {
                    qInfo() << port << " ";
                }

                qDebug() << "Servers: ";
                foreach(QTcpServer* tcpServer, m_server->servers())
                {
                    qInfo() << tcpServer->serverAddress().toString() << ":" << tcpServer->serverPort() << '\n';
                }
            }

            return true;
        }

    private:


        void buildRoutes()
        {
            m_server->route("/", [this](const QHttpServerRequest& request) {
                return m_serverApi->getRequestInfo(request);
            });

            m_server->route("/upload/", [this](const QUrl& url, const QHttpServerRequest& request) {
                return m_serverApi->uploadFile(url, request);
            });

            m_server->route("/download/", [this](const QUrl& url, const QHttpServerRequest& request) {
                return m_serverApi->downloadFile(url, request);
            });

            m_server->route("/assets/", [this](const QUrl& url, const QHttpServerRequest& request) {
                return m_serverApi->getAsset(url, request);
            });

            m_server->route("/time", [this](const QHttpServerRequest& request) {
                return m_serverApi->getTime(request);
            });

            m_server->route("/about", [this](const QHttpServerRequest& request) {
                return m_serverApi->getAboutPage(request);
            });

#ifdef QT_DEBUG
            m_server->afterRequest([this] (const QHttpServerRequest& request, QHttpServerResponse&& response) {
                Q_UNUSED(request);
                QTextStream(stdout) << details::createRequestInfo(request);

                return std::move(response);
            });
#endif
        }

        ServerApiPtr m_serverApi;
        ServerPtr m_server;
    };
}


// Tasks:
// - server time
// - file uploading and downloading
// - html "about" page
// - authorization (users, session)
// - https support

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString appPath = QCoreApplication::applicationDirPath();
    auto fileStorage = db::CreateScopedFileStorage(appPath);
    auto serverApi = net::createServerApi(fileStorage);
    auto httpServer = new QHttpServer;

    net::Server server(serverApi, httpServer);

    server.initialize();
    server.run();

    return a.exec();
}
