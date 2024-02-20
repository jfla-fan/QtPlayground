#pragma once

#include <QtGlobal>
#include <QString>

namespace res
{
    static const QString ASSETS = ":/assets";
    static const QString ABOUT = ASSETS + "/about.html";

    inline static QString assetPath(QStringView path) { return ASSETS + "/" + path.toString(); }
}
