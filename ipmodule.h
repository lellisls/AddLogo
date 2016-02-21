#ifndef IPMODULE_H
#define IPMODULE_H

#include <QDir>
#include <QFileInfoList>
#include <QObject>
#include <qimage.h>

class IPModule : public QObject {
  Q_OBJECT
public:
  explicit IPModule( const QFileInfoList &files,
                     int numThread,
                     int numThreads,
                     QImage logo,
                     QString saida,
                     QObject *parent = 0 );
signals:
  void updateProgress( int value );
  void error( QString err );
  void finished( int result );

public slots:
  void run( );
  void cancel( );

private:
  QFileInfoList files;
  int numThread;
  int numThreads;
  QImage logo;
  QDir outDir;
  bool canceled;
};

#endif /* IPMODULE_H */
