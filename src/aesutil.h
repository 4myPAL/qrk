/*
 * This file is part of QRK - Qt Registrier Kasse
 *
 * Copyright (C) 2015-2016 Christian Kvasny <chris@ckvsoft.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AESUTIL_H
#define AESUTIL_H

#include <iostream>
#include <QVariant>

class QString;

class AESUtil
{
  public:
    static QString getPrivateKey();

    static QString encryptTurnoverCounter(QString concatenated, qlonglong turnoverCounter, QString symmetricKey);
    static QString decryptTurnoverCounter(QString concatenated, QString encryptedTurnoverCounter, QString symmetricKey);
    static QString sigLastReceipt(QString);
    static QString generateKey();

    static void test();

  private:
    static QString encryptCTR(std::string, qlonglong, std::string);
    static QString decryptCTR(std::string, QString, std::string);
    static QString HashValue(QString);

    static QString base64_encode(QString string);
    static QString base64_decode(QString string, bool hex = false);
    static QString base32_encode(QString str);
    static QString base32_decode(QString str);

    static QString toHex(QString str);

};

#endif // AESUTIL_H
