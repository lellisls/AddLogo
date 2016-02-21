#ifndef CONTROLLER_H
#define CONTROLLER_H


#include "ipmodule.h"

#include <QFileInfoList>
#include <QObject>
#include <qprogressdialog.h>
class MainWindow;

class Controller : public QObject {
  Q_OBJECT
public:
  explicit Controller( QString input, QString logo_path, QString output, MainWindow *parent );
  int size( );
signals:
  void error( QString err );
  void finished( int result );
public slots:
  void run( );
  void cancel( );
private:
  QFileInfoList files;
  QImage logo;
  QString output;
  QVector< QThread* > threads;
  QVector< IPModule* > workers;
};

#endif /* CONTROLLER_H */
