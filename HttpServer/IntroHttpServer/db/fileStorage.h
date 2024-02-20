#pragma once

#include <QtGlobal>

QT_BEGIN_NAMESPACE
template< class T > class QFuture;
template< class T > class QSharedPointer;
class QFileInfo;
class QByteArray;
class QByteArrayView;
class QStringView;
QT_END_NAMESPACE


namespace db
{
    class IFileStorage;
    using FileStoragePtr = QSharedPointer< IFileStorage >;

    class IFileStorage
    {
    public:
        virtual ~IFileStorage() = default;

        virtual QFuture< bool >       ExistsFile(QStringView filePath) = 0;
        virtual QFuture< bool >       SaveFile(QStringView filePath, QByteArrayView data) = 0;
        virtual QFuture< QFileInfo >  FindFile(QStringView filePath) = 0;
        virtual QFuture< QByteArray > GetFileContent(QStringView filePath) = 0;
        virtual QFuture< bool >       DeleteFile(QStringView filePath) = 0;
    };


    /*
     * \brief Default file system.
     *
     * \returns Pointer to the file system.
    */
    FileStoragePtr GetSystemFileStorage();

    /*
     * \brief In this file system all paths are relative to rootDirectoryPath.
     *
     * \param rootDirectoryPath File system path that points to root storage directory.
     * \returns Pointer to file system or null if failed to create one.
    */
    FileStoragePtr CreateScopedFileStorage(QStringView rootDirectoryPath);
}
