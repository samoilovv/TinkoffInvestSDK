#ifndef TINKOFFINVESTCONSTS_H
#define TINKOFFINVESTCONSTS_H

#include <QVector>
#include <QVariant>
#include <QStringList>
#include <QString>

#include "hevaa_transport.h"

//const hevaa::transport::Row TINKOFF_SERVISES = {
hevaa::transport::Row TINKOFF_SERVISES = {
    QStringList({"sandbox",             "Sandbox Service"}),
    QStringList({"instruments",         "Instruments Service"}),
    QStringList({"marketdata",          "Market Data Service"}),
    QStringList({"marketdatastream",    "Market Data Stream Service"}),
    QStringList({"operations",          "Operations Service"}),
    QStringList({"orders",              "Orders Service"}),
    QStringList({"ordersstream",        "Orders Stream Service"}),
    QStringList({"stoporders",          "Stop Orders Service"}),
    QStringList({"users",               "Users Service"}),
    };

#endif // TINKOFFINVESTCONSTS_H
