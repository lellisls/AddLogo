#include "ipmodule.h"
#include "qimgorient.h"
#include <QDebug>
#include <QPainter>

IPModule::IPModule( const QFileInfoList &files,
                    int numThread,
                    int numThreads,
                    QImage logo,
                    QString outputFolder,
                    QObject *parent ) :
  QObject( parent ),
  files( files ),
  numThread( numThread ),
  numThreads( numThreads ),
  logo( logo ),
  outDir( outputFolder ) {
}

void IPModule::run( ) {
  int nFiles = files.size( );
  canceled = false;
  qDebug( ) << "===== Thread: " << numThread << "/" << numThreads << " =====";

  for( int i = numThread; i < nFiles; i += numThreads ) {
    qDebug( ) << "Pos: " << i << "/" << nFiles;

    QImage img = QImgOrient::loadImage( files.at( i ).absoluteFilePath( ) );
    if( !img.isNull( ) ) {
      img = img.scaled( 1024, 1024, Qt::KeepAspectRatio, Qt::SmoothTransformation );
      QImage imageWithOverlay = QImage( img.size( ), QImage::Format_ARGB32_Premultiplied );
      QPainter painter( &imageWithOverlay );
      painter.setCompositionMode( QPainter::CompositionMode_Source );
      painter.fillRect( imageWithOverlay.rect( ), Qt::transparent );

      painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
      painter.drawImage( 0, 0, img );

      painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
      painter.drawImage( img.width( ) - logo.width( ) - 20, img.height( ) - logo.height( ), logo );

      painter.end( );


      imageWithOverlay.save( outDir.absoluteFilePath( files[ i ].fileName( ) ) );
    }
    emit updateProgress( i );
    if( canceled ) {
      qDebug( ) << "===== Thread forced to finish! =====";
      emit( finished( 1 ) );
      return;
    }
  }
  qDebug( ) << "===== Thread: " << numThread << "/" << numThreads << " finished! =====";
  emit( finished( 0 ) );
}

void IPModule::cancel( ) {
  canceled = true;
}
