#include "controller.h"
#include "ipmodule.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfoList>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QProgressDialog>

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
  controller = new Controller( ui->lineEditEntrada->text( ),
                               ui->lineEditLogo->text( ),
                               ui->lineEditSaida->text( ),
                               this );
  canceled = false;
  progress = new QProgressDialog( "Iniciando...", "Cancelar", 0, controller->size( ) - 1, this );
  progress->setWindowModality( Qt::WindowModal );
/*  progress->show( ); */
  QThread *thread = new QThread( this );
  controller->moveToThread( thread );
  connect( thread, &QThread::started, controller, &Controller::run );
  connect( controller, &Controller::finished, this, &MainWindow::finished );
  connect( controller, &Controller::finished, thread, &QThread::quit );
  connect( controller, &Controller::finished, thread, &QThread::deleteLater );
/*  connect( progress, &QProgressDialog::canceled, controller, &Controller::cancel ); */
  connect( progress, &QProgressDialog::canceled, this, &MainWindow::cancel );

  thread->start( );

  ui->pushButtonStart->setEnabled( false );
}

void MainWindow::finished( int ) {
  if( canceled ) {
    ui->statusBar->showMessage("A operação foi cancelada!", 3000);
  }else{
    QMessageBox msgBox;
    msgBox.setText( tr( "Operação terminada com sucesso!" ) );
    msgBox.exec( );
  }
  ui->pushButtonStart->setEnabled( true );
}

void MainWindow::updateTheProgress( int value ) {
  if( progress && !canceled ) {
    value = progress->value( ) + 1;
    progress->setValue( value );
    progress->setLabelText( tr( "Processando %1 de %2" ).arg( value + 1 ).arg( progress->maximum( ) + 1 ) );
  }
}

void MainWindow::cancel( ) {
  canceled = true;
  controller->cancel( );
  ui->pushButtonStart->setEnabled( true );
}
