#include <QProgressDialog>
#include <QApplication>

#include "ExampleWidget.h"

int main( int argc, char **argv )
{
  QApplication app(argc, argv);
  ExampleWidget exampleWidget;

  exampleWidget.show();

  return app.exec();
}
