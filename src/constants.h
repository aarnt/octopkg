#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

#if defined(__DragonFly__)
  const QString ctn_PKG_BIN = "/usr/local/sbin/pkg";
#elif
  const QString ctn_PKG_BIN = "/usr/sbin/pkg";
#endif

#endif // CONSTANTS_H
