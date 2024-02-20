#include "utils.h"

#include <optional>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QString>


/////////////////////////////////////////////////////////////////////////////////////////////////////

template< class TJson >
constexpr bool is_json_type_v = std::disjunction_v<std::is_same<TJson, QJsonObject>, std::is_same<TJson, QJsonArray>>;


template< class TJson, std::enable_if_t<is_json_type_v< TJson >, bool> = true >
static std::optional< TJson > TryCreateJson(const QByteArray& byteArray)
{
    return TryCreateJson< TJson >(QJsonDocument::fromJson(byteArray));
}


template< class TJson, std::enable_if_t<is_json_type_v< TJson >, bool> = true >
static std::optional< TJson > TryCreateJson(const QJsonDocument& document)
{
    if (document.isNull())
    {
        qDebug() << "Null json document.";
        return std::nullopt;
    }

    if constexpr (std::is_same_v<TJson, QJsonObject>)
    {
        if (document.isObject())
        {
            return document.object();
        }

        qDebug() << "Not an object;";
    }
    else if constexpr (std::is_same_v<TJson, QJsonArray>)
    {
        if (document.isArray())
        {
            return document.array();
        }

        qDebug() << "Not an array;";
    }

    return std::nullopt;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

std::optional< QJsonObject > utils::tryCreateJsonObject(const QJsonDocument& document)
{
    return TryCreateJson< QJsonObject >(document);
}

std::optional< QJsonObject > utils::tryCreateJsonObject(const QByteArray& byteArray)
{
    return TryCreateJson< QJsonObject >(byteArray);
}


std::optional< QJsonArray > utils::tryCreateJsonArray(const QByteArray& byteArray)
{
    return TryCreateJson< QJsonArray >(byteArray);
}


std::optional< QJsonArray > utils::tryCreateJsonArray(const QJsonDocument& document)
{
    return TryCreateJson< QJsonArray >(document);
}

QString utils::jsonToString(const QJsonDocument& document)
{
    return document.toJson();
}

QString utils::jsonToString(const QJsonObject& object)
{
    return QJsonDocument(object).toJson();
}

QString utils::jsonToString(const QJsonArray& array)
{
    return QJsonDocument(array).toJson();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
