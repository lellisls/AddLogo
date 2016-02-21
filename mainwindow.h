#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ipmodule.h"

#include <QMainWindow>
#include <QProgressDialog>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow( QWidget *parent = 0 );
  ~MainWindow( );

private slots:
  void on_pushButtonEntrada_clicked( );

  void on_pushButtonSaida_clicked( );

  void on_pushButtonLogo_clicked( );

  void on_pushButtonStart_clicked( );

  void finished( int result );

private:
  Ui::MainWindow *ui;
  QProgressDialog *progress;
  IPModule *ipm;
};

#endif /* MAINWINDOW_H */
