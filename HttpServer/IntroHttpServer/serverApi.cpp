#include "serverApi.h"
#include "res.hpp"
#include "db/fileStorage.h"

#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QFuture>
#include <QtConcurrent>
#include <QDateTime>
#include <QJsonObject>
#include <QtAssert>


using net::ServerApiPtr;
using db::FileStoragePtr;

QString net::details::createRequestInfo(const QHttpServerRequest& request)
{
    return "Query: " + request.query().toString() + "\n" +
           "Url: " + request.url().toString()+ '\n' +
           "Remote address: " + request.remoteAddress().toString() + '\n' +
           "Remote port: " + QString::number(request.remotePort()) + '\n' +
           "Local address: " + request.localAddress().toString() + '\n' +
           "Local port: " + QString::number(request.localPort()) + '\n' +
           "Host: " + QString::fromLatin1(request.value("Host")) + '\n' +
           "Body: " + QString::fromLatin1(request.body()) + '\n' +
           "Server time: " + QDateTime::currentDateTime().toString() + '\n';
}


class CServerApi final: public net::IServerApi
{
public:
    CServerApi(FileStoragePtr fileStorage)
        : m_fileStorage(fileStorage)
    {
        Q_ASSERT(m_fileStorage);
        qDebug() << "CServerApi created.";
    }

    ~CServerApi() override
    {
        qDebug() << "CServerApi destroyed.";
    }

    QFuture< QHttpServerResponse > getRequestInfo(const QHttpServerRequest& request) override
    {
        return QtFuture::makeReadyValueFuture(QHttpServerResponse(net::details::createRequestInfo(request)));
    }

    QFuture< QHttpServerResponse > uploadFile(const QUrl& filePath, const QHttpServerRequest& request) override
    {
        qDebug() << "uploadFile call, but not implemented!";
        return QtFuture::makeReadyValueFuture(QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented));
    }

    QFuture< QHttpServerResponse > downloadFile(const QUrl& filePath, const QHttpServerRequest& request) override
    {
        qDebug() << "downloadFile call, but not implemented!";
        return QtFuture::makeReadyValueFuture(QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented));
    }

    QFuture< QHttpServerResponse > getAsset(const QUrl& assetPath, const QHttpServerRequest& request) override
    {
        return getAssetInternal(res::assetPath(assetPath.toString()), request);
    }

    QFuture< QHttpServerResponse > getTime(const QHttpServerRequest& request) override
    {
        return QtFuture::makeReadyValueFuture(QHttpServerResponse {
            QJsonObject{
                { "server_time", QDateTime::currentDateTime().toString() }
            }
        });
    }

    QFuture< QHttpServerResponse > getAboutPage(const QHttpServerRequest& request) override
    {
        return getAssetInternal(res::ABOUT, request);
    }

private:

    QFuture< QHttpServerResponse > getAssetInternal(QStringView fullPath, const QHttpServerRequest& request)
    {
        Q_UNUSED(request)
        using StatusCode = QHttpServerResponse::StatusCode;

        return QtConcurrent::run([=]() {
            QHttpServerResponse result = QHttpServerResponse::fromFile(fullPath.toString());
            if (result.statusCode() != QHttpServerResponse::StatusCode::Ok)
            {
                return QHttpServerResponse(StatusCode::NotFound);
            }

            return result;
        });
    }

    FileStoragePtr m_fileStorage;
};


ServerApiPtr net::createServerApi(FileStoragePtr fileStorage)
{
    return ServerApiPtr(new CServerApi(fileStorage));
}
