#ifndef FloatSlider_H
#define FloatSlider_H

#include <QtDesigner/QDesignerExportWidget>
class QDoubleValidator;

#include "LabeledSlider.h"

// This slider should always be set from min = 0 to max = 100.
// This then lets us interpret the difference between the min and max
// as a percentage.
class QDESIGNER_WIDGET_EXPORT FloatSlider : public LabeledSlider
{
Q_OBJECT
Q_PROPERTY( double minValueText READ minValueText WRITE setMinValueText DESIGNABLE true STORED true)
Q_PROPERTY( double maxValueText READ maxValueText WRITE setMaxValueText DESIGNABLE true STORED true)

signals:
  void valueChanged(float);

public:
  FloatSlider(QWidget *parent = 0);

  float GetValue();
  void setMinimum(const float);
  void setMaximum(const float);

  virtual double minValueText() const;
  virtual void setMinValueText(const double minValue_in);
  virtual double maxValueText() const;
  virtual void setMaxValueText(const double maxValue_in);

public slots:
  void slot_horizontalSlider_valueChanged(int value);
  virtual void on_txtMin_textChanged( const QString & text );
  virtual void on_txtMax_textChanged( const QString & text );

protected:
  void SetCurrentValueLabel();

  QDoubleValidator* Validator;

};

#endif
