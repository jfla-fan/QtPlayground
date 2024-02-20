#pragma once

#include "../fileStorage.h"

#include <filesystem>


namespace db
{
    class CSystemFileStorage : public IFileStorage
    {
    public:
        CSystemFileStorage();
        ~CSystemFileStorage() override;

        QFuture< bool >       ExistsFile(QStringView filePath) override;
        QFuture< bool >       SaveFile(QStringView filePath, QByteArrayView data) override;
        QFuture< QFileInfo >  FindFile(QStringView filePath) override;
        QFuture< QByteArray > GetFileContent(QStringView filePath) override;
        QFuture< bool >       DeleteFile(QStringView filePath) override;

    protected:
        virtual std::filesystem::path GetPath(QStringView filePath);
    };
}
