#include "systemFileStorage.h"

#include <QDebug>
#include <QFile>
#include <QtConcurrent>


namespace db
{
    CSystemFileStorage::CSystemFileStorage()
    {
        qDebug() << "System file storage created.";
    }

    CSystemFileStorage::~CSystemFileStorage()
    {
        qDebug() << "System file storage destroyed.";
    }

    QFuture< bool > CSystemFileStorage::ExistsFile(QStringView filePath)
    {
        return QtConcurrent::run([=]() { return QFile::exists(GetPath(filePath)); });
    }

    QFuture< bool > CSystemFileStorage::SaveFile(QStringView filePath, QByteArrayView data)
    {
        return QtConcurrent::run([=]() {
            const auto stdPath = GetPath(filePath);

            QFile file(stdPath);
            if (file.open(QFile::WriteOnly))
            {
                return file.write(data.constData(), data.length()) == data.length();
            }

            return false;
        });
    }

    QFuture< QFileInfo > CSystemFileStorage::FindFile(QStringView filePath)
    {
        return QtConcurrent::run([=]() {
            return QFileInfo(GetPath(filePath));
        });
    }

    QFuture< QByteArray > CSystemFileStorage::GetFileContent(QStringView filePath)
    {
        return QtConcurrent::run([=]() {
            const auto stdPath = GetPath(filePath);

            QFile file(stdPath);
            if (file.open(QFile::ReadOnly))
            {
                return file.readAll();
            }

            return QByteArray();
        });
    }

    QFuture< bool > CSystemFileStorage::DeleteFile(QStringView filePath)
    {
        return QtConcurrent::run([=]() {
            return QFile::remove(GetPath(filePath));
        });
    }


    std::filesystem::path CSystemFileStorage::GetPath(QStringView filePath)
    {
        return { filePath.toString().toStdString() };
    }


    FileStoragePtr GetSystemFileStorage()
    {
        static FileStoragePtr s_fileStorage(new CSystemFileStorage());
        return s_fileStorage;
    }
}
