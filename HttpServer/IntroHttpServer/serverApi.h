#pragma once

#include <QtGlobal>

QT_BEGIN_NAMESPACE
template< class T > class QFuture;
template< class T > class QSharedPointer;
class QHttpServerRequest;
class QHttpServerResponse;
class QUrl;
QT_END_NAMESPACE

namespace db
{
    class IFileStorage;
    using FileStoragePtr = QSharedPointer< IFileStorage >;
}

namespace net
{
    class IServerApi;
    using ServerApiPtr = QSharedPointer< IServerApi >;

    /*
     * Http routes api.
     * Must be thread-safe.
    */
    class IServerApi
    {
    public:
        virtual ~IServerApi() = default;

        virtual QFuture< QHttpServerResponse > getRequestInfo(const QHttpServerRequest& request) = 0;
        virtual QFuture< QHttpServerResponse > uploadFile(const QUrl& filePath, const QHttpServerRequest& request) = 0;
        virtual QFuture< QHttpServerResponse > downloadFile(const QUrl& filePath, const QHttpServerRequest& request) = 0;
        virtual QFuture< QHttpServerResponse > getAsset(const QUrl& assetPath, const QHttpServerRequest& request) = 0;
        virtual QFuture< QHttpServerResponse > getTime(const QHttpServerRequest& request) = 0;
        virtual QFuture< QHttpServerResponse > getAboutPage(const QHttpServerRequest& request) = 0;
    };

    ServerApiPtr createServerApi(db::FileStoragePtr fileStorage);
}


namespace net::details
{
    QString createRequestInfo(const QHttpServerRequest& request);
}
