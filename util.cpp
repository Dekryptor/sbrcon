#include "util.h"

QString util::escapeQuotes(QString in)
{
    return in.replace(
                QStringLiteral("\""),
                QStringLiteral("\\\"")
    );
}
