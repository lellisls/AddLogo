#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ipmodule.h"

#include <QMainWindow>
#include <QMutex>
#include <QProgressDialog>

namespace Ui {
  class MainWindow;
}

class Controller;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow( QWidget *parent = 0 );
  ~MainWindow( );

signals:
  void finishProgress( int status );

private slots:
  void on_pushButtonEntrada_clicked( );

  void on_pushButtonSaida_clicked( );

  void on_pushButtonLogo_clicked( );

  void on_pushButtonStart_clicked( );

public slots:
  void finished(int);

  void updateTheProgress( int value );

  void cancel( );

private:
  Ui::MainWindow *ui;
  QProgressDialog *progress;
  Controller *controller;
  bool canceled;
};

#endif /* MAINWINDOW_H */
