#pragma once

#include <QtGlobal>

namespace std
{
    template< class T > class optional;
}

QT_BEGIN_NAMESPACE
    class QJsonDocument;
    class QJsonArray;
    class QJsonObject;
QT_END_NAMESPACE


namespace utils
{
    std::optional< QJsonArray > tryCreateJsonArray(const QByteArray& byteArray);
    std::optional< QJsonArray > tryCreateJsonArray(const QJsonDocument& document);

    std::optional< QJsonObject > tryCreateJsonObject(const QByteArray& byteArray);
    std::optional< QJsonObject > tryCreateJsonObject(const QJsonDocument& document);

    QString jsonToString(const QJsonDocument& document);
    QString jsonToString(const QJsonObject& object);
    QString jsonToString(const QJsonArray& array);
}
