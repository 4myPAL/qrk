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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <ui_qrk.h>
#include "reports.h"

#include <QWidget>
#include <QDate>

class QLabel;
class QProgressBar;

class QRKHome;
class QRKDocument;
class QRKRegister;
class QStackedWidget;
class QLCDNumber;

class QRK : public QMainWindow
{
    Q_OBJECT

  public:

    QRK();
    ~QRK();

    void setLastEOD(QDate lastEOD) {this->lastEOD = lastEOD;}
    void setNoPrinter() { noPrinter = true; }  // do not print to printer but only to pdf file for testing only
    void setShopName();
    int getCurrentRegisterYear(){return currentRegisterYear;}

    QLabel *currentRegisterYearLabel;
    QLabel *cashRegisterIdLabel;
    QProgressBar *progressBar;

    QRKHome *qrk_home;
    QRKRegister *qrk_register;
    QRKDocument *qrk_document;
    QStackedWidget *stackedWidget;

  protected:
    void iniStack();

  private slots:
    bool endOfDaySlot();
    bool endOfMonthSlot();
    void onRegisterButton_clicked();
    void onCancelRegisterButton_clicked();

    void onDocumentButton_clicked();
    void onCancelDocumentButton_clicked();
    void actionDEP_Export();
    void fullScreenSlot();
    void exitSlot();

  protected slots:
    virtual void timerDone();


  private:
    void init();
    void closeEvent (QCloseEvent *event);

    QString shopname();
    QLCDNumber *dateLcd;

  private:
    Ui::MainWindow *ui;

    QTimer *timer;

    int currentReceipt;
    int currentRegisterYear;
    QString cashRegisterId;

    QString shopName;
    bool noPrinter;
    QDate lastEOD;

};

#endif // MAINWIDGET_H
