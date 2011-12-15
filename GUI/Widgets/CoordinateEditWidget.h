#ifndef CoordinateEditWidget_H
#define CoordinateEditWidget_H

#include <QtDesigner/QDesignerExportWidget>
class QDoubleValidator;

#include "ui_CoordinateEditWidget.h"

class QDESIGNER_WIDGET_EXPORT CoordinateEditWidget : public QWidget, public Ui::CoordinateEditWidget
{
Q_OBJECT
Q_PROPERTY( double xText READ xText WRITE setXText DESIGNABLE true STORED true)
Q_PROPERTY( double yText READ yText WRITE setYText DESIGNABLE true STORED true)
Q_PROPERTY( double zText READ zText WRITE setZText DESIGNABLE true STORED true)

signals:

  void coordinateChanged(QVector<double> coord);

public:
  CoordinateEditWidget(QWidget *parent = 0);

  QVector<double> GetCoordinate();

  // The following are for the Designer interface
  double xText() const;
  void setXText(const double x);

  double yText() const;
  void setYText(const double y);

  double zText() const;
  void setZText(const double z);

public slots:

  void on_txtX_textChanged( const QString & text );
  void on_txtY_textChanged( const QString & text );
  void on_txtZ_textChanged( const QString & text );

protected:

  QDoubleValidator* Validator;

};

#endif
