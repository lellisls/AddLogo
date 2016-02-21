#ifndef QIMGORIENT_H
#define QIMGORIENT_H

#include <QObject>

#include "windows.h"
#include "gdiplus.h"
#include "minmax.h"

#include <QImage>

using namespace Gdiplus;

class QImgOrient {
public:

  static QImage loadImage( QString fname );
};

#endif /* QIMGORIENT_H */
