#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

#if defined(__DragonFly__)
  const QString ctn_PKG_BIN = QStringLiteral("/usr/local/sbin/pkg");
#else
  const QString ctn_PKG_BIN = QStringLiteral("/usr/sbin/pkg");
#endif

const QString ctn_OCTOPKG_DOAS = QStringLiteral("/usr/lib/octopkg/octopkg-doas");
const QString ctn_OCTOPKG_DOAS_PARAMS = QStringLiteral("-d");

#endif // CONSTANTS_H
