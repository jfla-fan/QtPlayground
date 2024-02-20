#pragma once

#include "systemFileStorage.h"

#include <QDir>


namespace db
{
    class CScopedFileStorage final: public CSystemFileStorage
    {
    public:

        static FileStoragePtr Create(QStringView directoryPath)
        {
            return Create(std::filesystem::path { directoryPath.toString().toStdString() });
        }

        static FileStoragePtr Create(const std::filesystem::path& directoryPath);

    protected:
        std::filesystem::path GetPath(QStringView filePath) override
        {
            return m_rootDirectory / CSystemFileStorage::GetPath(filePath);
        }

    private:
        CScopedFileStorage(const std::filesystem::path& rootDirectoryPath)
            : m_rootDirectory(rootDirectoryPath)
        {
        }

        std::filesystem::path m_rootDirectory;
    };


}
