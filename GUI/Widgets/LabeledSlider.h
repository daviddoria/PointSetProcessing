#ifndef LabeledSlider_H
#define LabeledSlider_H

#include <QSlider>
#include <QtDesigner/QDesignerExportWidget>

#include "ui_LabeledSlider.h"

class QIntValidator;

class QDESIGNER_WIDGET_EXPORT LabeledSlider : public QWidget, public Ui::LabeledSlider
{
Q_OBJECT
//Q_PROPERTY( float minValue READ minValue WRITE setMinValue DESIGNABLE true) // NOTE: Float does not work in Designer! Use double instead.

// Q_PROPERTY( QString name READ name WRITE setName DESIGNABLE true STORED false)
// Q_PROPERTY( int minValueText READ minValueText WRITE setMinValueText DESIGNABLE true STORED false)
// Q_PROPERTY( int maxValueText READ maxValueText WRITE setMaxValueText DESIGNABLE true STORED false)

// Even though we don't have an ivar to maintain the state directly, STORED must be true or the values set in designer are reset when the widget is actually instantiated
Q_PROPERTY( QString name READ name WRITE setName DESIGNABLE true STORED true)
Q_PROPERTY( int minValueText READ minValueText WRITE setMinValueText DESIGNABLE true STORED true)
Q_PROPERTY( int maxValueText READ maxValueText WRITE setMaxValueText DESIGNABLE true STORED true)

signals:
  void valueChanged(int);

public slots:
  virtual void slot_horizontalSlider_valueChanged(int); // We can't use the named slot autoconnect here because we want to connect to a different slot in a subclass
  virtual void on_txtMin_textChanged( const QString & text );
  virtual void on_txtMax_textChanged( const QString & text );
  //virtual void slot_rangeChanged();

public:
  LabeledSlider(QWidget *parent = 0);

  unsigned int value();

  void setMinimum(const unsigned int);
  void setMaximum(const unsigned int);

  // The following functions are to support the widget in the Designer
  QString name();
  void setName(const QString& name);

  int minValueText() const;
  void setMinValueText(const int minValueText);

  int maxValueText() const;
  void setMaxValueText(const int maxValueText);

protected:
  QIntValidator* Validator;

  void SetCurrentValueLabel();

};

#endif
