#ifndef IPMODULE_H
#define IPMODULE_H

#include <QDir>
#include <QFileInfoList>
#include <QObject>
#include <qimage.h>

class IPModule : public QObject {
  Q_OBJECT
public:
  explicit IPModule( QString inputFolder, QString logo, QString saida, QObject *parent = 0 );
  int numFiles();
signals:
  void finished( int result );
  void updateProgress( int value );
  void error( QString err );

public slots:
  void run( );
  void cancel();

private:
  QFileInfoList files;
  QDir outDir;
  QImage logo;
  bool canceled;
};

#endif /* IPMODULE_H */
