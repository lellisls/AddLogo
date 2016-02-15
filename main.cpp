#include "mainwindow.h"
#include <QApplication>

int main( int argc, char *argv[] ) {
  QApplication a( argc, argv );
  QCoreApplication::setOrganizationName( "Lellis" );
  QCoreApplication::setApplicationName( "Add logo" );

  MainWindow w;
  w.show( );

  return( a.exec( ) );
}
