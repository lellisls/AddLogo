#include "controller.h"
#include "mainwindow.h"

#include <QDebug>
#include <QThread>

Controller::Controller( QString input, QString logo_path, QString outputDir, MainWindow *parent ) :
  QObject( NULL ), output( outputDir ) {

  QStringList filters;
  files = QDir( input ).entryInfoList( filters, QDir::Files, QDir::Time );
  logo = QImage( logo_path ).scaledToWidth( 180, Qt::SmoothTransformation );
  int numThreads = QThread::idealThreadCount( );

  for( int nThd = 0; nThd < numThreads; ++nThd ) {
    QThread *thread = new QThread( this );
    threads.append( thread );

    IPModule *ipm = new IPModule( files, nThd, numThreads, logo, output );
    workers.append( ipm );

    ipm->moveToThread(thread);

    connect( thread, &QThread::started, ipm, &IPModule::run );
    connect( ipm, &IPModule::updateProgress, thread, &QThread::quit );
    connect( ipm, &IPModule::updateProgress, parent, &MainWindow::updateTheProgress );
  }
}

int Controller::size( ) {
  return( files.size( ) );
}

void Controller::run( ) {
  int numThreads = threads.size();
  qDebug( ) << "======== Starting threads ========";

  for( int nThd = 0; nThd < numThreads; ++nThd ) {
    threads[ nThd ]->start( );
  }
  for( int nThd = 0; nThd < numThreads; ++nThd ) {
    threads[ nThd ]->exit();
    threads[ nThd ]->wait();
  }
  qDebug( ) << " ==== Finished threads ==== ";
  emit finished( 0 );
}

void Controller::cancel( ) {
  qDebug( ) << "======== CANCEL ========";
  foreach( IPModule * ipm, workers ) {
    ipm->cancel( );
  }
}
