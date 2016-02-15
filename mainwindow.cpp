#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfoList>
#include <QPainter>
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
  QStringList filters;
  QFileInfoList files = QDir( ui->lineEditEntrada->text( ) ).entryInfoList( filters, QDir::Files, QDir::Time );
  QDir outDir( ui->lineEditSaida->text( ) );
  QImage logo = QImage( ui->lineEditLogo->text( ) ).scaledToWidth( 120, Qt::SmoothTransformation );
  int numFiles = files.size( );
  if( numFiles == 0 ) {
    ui->statusBar->showMessage( "Nenhum arquivo encontrado.", 2000 );
    return;
  }
  QProgressDialog progress( "Copying files...", "Abort Copy", 0, numFiles, this );
  progress.setWindowModality( Qt::WindowModal );
  progress.show( );
  progress.setValue( 0 );
  for( int i = 0; i < numFiles; i++ ) {
    QImage img = QImage( files[ i ].absoluteFilePath( ), 0 ).scaled( 1024, 1024, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    QImage imageWithOverlay = QImage( img.size( ), QImage::Format_ARGB32_Premultiplied );
    QPainter painter( &imageWithOverlay );

    painter.setCompositionMode( QPainter::CompositionMode_Source );
    painter.fillRect( imageWithOverlay.rect( ), Qt::transparent );

    painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
    painter.drawImage( 0, 0, img);

    painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
    painter.drawImage( img.width() - logo.width(),  img.height() - logo.height(), logo );

    painter.end( );
/*
 *    QPixmap pixmap = QPixmap::fromImage( img );
 *    pixmap.toImage( ).save( outDir.absoluteFilePath( files[ i ].fileName( ) ) );
 */
    imageWithOverlay.save( outDir.absoluteFilePath( files[ i ].fileName( ) ) );
    progress.setValue( i );
    if( progress.wasCanceled( ) ) {
      break;
    }
  }
  progress.setValue( numFiles );
  ui->statusBar->showMessage( "Operação terminada.", 2000 );
}
