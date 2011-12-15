#include <QApplication>
#include "PointSetProcessingWidget.h"

int main(int argc, char* argv[])
{
  QApplication app( argc, argv );

  PointSetProcessingWidget* pointSetProcessingWidget;
  if(argc < 2)
    {
    pointSetProcessingWidget = new PointSetProcessingWidget;
    }
  else
    {
    pointSetProcessingWidget = new PointSetProcessingWidget(argv[1]);
    }

  pointSetProcessingWidget->show();

  return app.exec();
}