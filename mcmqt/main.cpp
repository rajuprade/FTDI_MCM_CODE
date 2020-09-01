#include <qapplication.h>
#include "mcmCom.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    mcmCOM w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
