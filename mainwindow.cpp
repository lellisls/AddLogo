#include "ipmodule.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfoList>
#include <QSettings>
#include <QThread>
#include <qprogressdialog.h>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
  ui->setupUi( this );
  QSettings settings;
  ui->lineEditEntrada->setText( settings.value( "entrada" ).toString( ) );
  ui->lineEditSaida->setText( settings.value( "saida" ).toString( ) );
  ui->lineEditLogo->setText( settings.value( "logo" ).toString( ) );
}

MainWindow::~MainWindow( ) {

  QSettings settings;
  settings.setValue( "entrada", ui->lineEditEntrada->text( ) );
  settings.setValue( "saida", ui->lineEditSaida->text( ) );
  settings.setValue( "logo", ui->lineEditLogo->text( ) );

  delete ui;
}

void MainWindow::on_pushButtonEntrada_clicked( ) {
  QString folder = QFileDialog::getExistingDirectory( this, "Pasta de Entrada", QDir::homePath( ) );
  if( folder.isEmpty( ) ) {
    return;
  }
  else {
    ui->lineEditEntrada->setText( folder );
  }
}

void MainWindow::on_pushButtonSaida_clicked( ) {
  QString folder = QFileDialog::getExistingDirectory( this, "Pasta de Saída", QDir::homePath( ) );
  if( folder.isEmpty( ) ) {
    return;
  }
  else {
    ui->lineEditSaida->setText( folder );
  }
}

void MainWindow::on_pushButtonLogo_clicked( ) {
  QString fileName =
    QFileDialog::getOpenFileName( this, "Logo", QDir::homePath( ), "Imagens PNG (*.png);; Outros  (*)" );
  if( fileName.isEmpty( ) ) {
    return;
  }
  else {
    ui->lineEditLogo->setText( fileName );
  }
}

void MainWindow::on_pushButtonStart_clicked( ) {
  ipm = new IPModule( ui->lineEditEntrada->text( ), ui->lineEditLogo->text( ), ui->lineEditSaida->text( ) );
  progress = new QProgressDialog( "Processando...", "Cancelar", 0, ipm->numFiles( ), this );
  progress->setWindowModality( Qt::WindowModal );

  progress->show( );
  QThread *thread = new QThread;
  ipm->moveToThread( thread );
  connect( thread, &QThread::started, ipm, &IPModule::run );
  connect( ipm, &IPModule::updateProgress, progress, &QProgressDialog::setValue );
  connect( progress, &QProgressDialog::canceled, ipm, &IPModule::cancel );
  connect( ipm, &IPModule::finished, thread, &QThread::quit );
  connect( ipm, &IPModule::finished, progress, &QProgressDialog::done );
  connect( ipm, &IPModule::finished, this, &MainWindow::finished );
  connect( ipm, &IPModule::finished, thread, &QThread::deleteLater );
  connect( ipm, &IPModule::finished, ipm, &IPModule::deleteLater );
  connect( ipm, &IPModule::finished, progress, &QProgressDialog::deleteLater );

  thread->start( );
}

void MainWindow::finished( int result ) {
  if( result == 0 ) {
    ui->statusBar->showMessage( "Operação terminada com sucesso!", 2000 );
  }
}
