#include "qimgorient.h"

/*
 * 1 - The 0th row is at the top of the visual image, and the 0th column is the visual left side. (Do Nothing)
 * 2 - The 0th row is at the visual top of the image, and the 0th column is the visual right side. (Hor flip)
 * 3 - The 0th row is at the visual bottom of the image, and the 0th column is the visual right side. (+180)
 * 4 - The 0th row is at the visual bottom of the image, and the 0th column is the visual left side. (ver flip)
 * 5 - The 0th row is the visual left side of the image, and the 0th column is the visual top. (-90 and flip ver)
 * 6 - The 0th row is the visual right side of the image, and the 0th column is the visual top. (+90)
 * 7 - The 0th row is the visual right side of the image, and the 0th column is the visual bottom. (+90 - flip ver)
 * 8 - The 0th row is the visual left side of the image, and the 0th column is the visual bottom. (-90)
 */

QImage QImgOrient::loadImage( QString fname ) {
  SHORT result = 0;
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );
  Image *image = Image::FromFile( fname.toStdWString( ).c_str( ) );
  UINT size = 0;
  UINT count = 0;
  image->GetPropertySize( &size, &count );
  PropertyItem *properties = ( PropertyItem* ) malloc( size );
  image->GetAllPropertyItems( size, count, properties );
  for( UINT i = 0; i < count; i++ ) {
    if( properties[ i ].id == PropertyTagOrientation ) {
      result = *( ( SHORT* ) ( properties[ i ].value ) );
    }
  }
  delete image;
  free( properties );
  GdiplusShutdown( gdiplusToken );

  QImage qimage( fname );
  QTransform transform;
  switch( result ) {
      case 1: {
      break;
    }
      case 2: {
      transform.scale( -1, 1 );
      break;
    }
      case 3: {
      transform.rotate( 180 );
      break;
    }
      case 4: {
      transform.scale( 1, -1 );
      break;
    }
      case 5: {
      transform.rotate( -90 );
      transform.scale( 1, -1 );
      break;
    }
      case 6: {
      transform.rotate( 90 );
      break;
    }
      case 7: {
      transform.rotate( 90 );
      transform.scale( 1, -1 );
      break;
    }
      case 8: {
      transform.rotate( -90 );
      break;
    }
  }
  if( !qimage.isNull( ) ) {
    return( qimage.transformed( transform ) );
  }
  else {
    return( QImage( ) );
  }
}
