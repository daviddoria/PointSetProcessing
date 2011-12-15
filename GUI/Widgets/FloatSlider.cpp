#include <iostream>

#include "FloatSlider.h"

#include <QDoubleValidator>

//FloatSlider::FloatSlider(QWidget *parent) : LabeledSlider(parent)
FloatSlider::FloatSlider(QWidget *parent) : LabeledSlider(parent)
{
  // We always want the slider to act as a percentage, so that the value is equal to min + slider * max
  this->horizontalSlider->setMinimum(0);
  this->horizontalSlider->setMaximum(100);

  // This sets the label text, which will be displayed and used to compute the internal values.
  // This is done again in FloatSlider because the m_min/maxValueText type has changed in the subclass.
  this->setMinValueText(0);
  this->setMaxValueText(100);

//   this->MinValue = 0.0f;
//   this->txtMin->setText(QString::number(this->MinValue));
//
//   this->MaxValue = 10.0f;
//   this->txtMax->setText(QString::number(this->MaxValue));

  this->lblCurrent->setText(QString::number(GetValue()));

  this->Validator = new QDoubleValidator(0.0d, 10000.0d, 2, this);
  this->txtMin->setValidator(this->Validator);
  this->txtMax->setValidator(this->Validator);
}

float FloatSlider::GetValue()
{
  return this->txtMin->text().toFloat() + (this->txtMax->text().toFloat() - this->txtMin->text().toFloat()) * static_cast<float>(this->horizontalSlider->value()) / 100.0f;
}

void FloatSlider::setMinimum(const float value)
{
  this->txtMin->setText(QString::number(value));
  this->lblCurrent->setText(QString::number(GetValue()));
  emit valueChanged(GetValue());
}

void FloatSlider::setMaximum(const float value)
{
  this->txtMax->setText(QString::number(value));
  this->lblCurrent->setText(QString::number(GetValue()));
  emit valueChanged(GetValue());
}

void FloatSlider::slot_horizontalSlider_valueChanged(int value)
{
  //std::cout << "FloatSlider::SliderValueChanged" << std::endl;
  this->lblCurrent->setText(QString::number(GetValue()));
  emit valueChanged(GetValue());
}

void FloatSlider::on_txtMin_textChanged( const QString & text )
{
  this->lblCurrent->setText(QString::number(GetValue()));
  emit valueChanged(GetValue());
}

void FloatSlider::on_txtMax_textChanged( const QString & text )
{
  this->lblCurrent->setText(QString::number(GetValue()));
  emit valueChanged(GetValue());
}

double FloatSlider::minValueText() const
{
  return this->txtMin->text().toDouble();
}

void FloatSlider::setMinValueText(const double minValue)
{
  this->txtMin->setText(QString::number(minValue));
}

double FloatSlider::maxValueText() const
{
  return this->txtMax->text().toDouble();
}

void FloatSlider::setMaxValueText(const double maxValue)
{
  this->txtMax->setText(QString::number(maxValue));
}

void FloatSlider::SetCurrentValueLabel()
{
  this->lblCurrent->setText(QString::number(this->GetValue()));
}
