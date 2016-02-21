#include "ipmodule.h"
#include "qimgorient.h"
#include <QDebug>
#include <QPainter>

IPModule::IPModule( QString inputFolder, QString logo_path, QString outputFolder, QObject *parent ) :
  QObject( parent ),
  outDir( outputFolder ) {
  QStringList filters;
  files = QDir( inputFolder ).entryInfoList( filters, QDir::Files, QDir::Time );
  logo = QImage( logo_path ).scaledToWidth( 120, Qt::SmoothTransformation );
}

int IPModule::numFiles( ) {
  return( files.size( ) );
}

void IPModule::run( ) {
  int nFiles = files.size( );
  canceled = false;
  for( int i = 0; i < nFiles; i++ ) {

    QImage img = QImgOrient::loadImage( files[ i ].absoluteFilePath( ) );
    if(!img.isNull()){
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

      break;
    }
  }
  qDebug( ) << "FINISHED";
  emit finished( 0 );
}

void IPModule::cancel( ) {
  canceled = true;
}
