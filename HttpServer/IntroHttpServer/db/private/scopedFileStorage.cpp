#include "scopedFileStorage.h"


namespace db
{
    FileStoragePtr CScopedFileStorage::Create(const std::filesystem::path& directoryPath)
    {
        QDir root(directoryPath);
        if (!root.mkpath("."))
        {
            return nullptr;
        }

        return FileStoragePtr(new CScopedFileStorage(directoryPath));
    }


    FileStoragePtr CreateScopedFileStorage(QStringView rootDirectoryPath)
    {
        return CScopedFileStorage::Create(rootDirectoryPath);
    }
}
