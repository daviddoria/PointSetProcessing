#include <iostream>

#include "LabeledSlider.h"

LabeledSlider::LabeledSlider(QWidget *parent) : QWidget(parent)
{
  setupUi(this);

  //this->txtMin->setText(QString::number(this->horizontalSlider->minimum()));
  //this->txtMax->setText(QString::number(this->horizontalSlider->maximum()));

  // This sets the label text, which will be displayed and used to compute the internal values.
  this->setMinValueText(0);
  this->setMaxValueText(100);

  this->horizontalSlider->setMinimum(this->txtMin->text().toInt());
  this->horizontalSlider->setMaximum(this->txtMax->text().toInt());

  // Position the slider in the center of the range
  this->horizontalSlider->setValue((this->txtMax->text().toInt() - this->txtMin->text().toInt())/2);

  // Set the value label
  this->lblCurrent->setText(QString::number(this->horizontalSlider->value()));

  this->Validator = new QIntValidator(0, 10000, this);
  this->txtMin->setValidator(this->Validator);
  this->txtMax->setValidator(this->Validator);

  connect(this->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_horizontalSlider_valueChanged(int)));
  //connect(this->txtMin, SIGNAL(textChanged(const QString&)), this, SLOT(slot_rangeChanged()));
  //connect(this->txtMax, SIGNAL(textChanged(const QString&)), this, SLOT(slot_rangeChanged()));
}

void LabeledSlider::SetCurrentValueLabel()
{
  this->lblCurrent->setText(QString::number(this->horizontalSlider->value()));
}

unsigned int LabeledSlider::value()
{
  return this->horizontalSlider->value();
}

void LabeledSlider::on_txtMin_textChanged( const QString & text )
{
  this->horizontalSlider->setMinimum(text.toInt());
  SetCurrentValueLabel();
  emit valueChanged(value());
}

void LabeledSlider::on_txtMax_textChanged( const QString & text )
{
  this->horizontalSlider->setMaximum(text.toInt());
  SetCurrentValueLabel();
  emit valueChanged(value());
}

void LabeledSlider::setMinimum(const unsigned int value)
{
  this->horizontalSlider->setMinimum(value);
  SetCurrentValueLabel();
}

void LabeledSlider::setMaximum(const unsigned int value)
{
  this->horizontalSlider->setMaximum(value);
  SetCurrentValueLabel();
}

void LabeledSlider::slot_horizontalSlider_valueChanged(int value)
{
  //std::cout << "LabeledSlider::on_horizontalSlider_valueChanged" << std::endl;
  this->lblCurrent->setText(QString::number(value));
  emit valueChanged(value);
}

QString LabeledSlider::name()
{
  return this->lblName->text();
}

void LabeledSlider::setName(const QString& name)
{
  lblName->setText(name);
}

int LabeledSlider::minValueText() const
{
  return this->txtMin->text().toInt();
}

void LabeledSlider::setMinValueText(const int minValueText)
{
  this->txtMin->setText(QString::number(minValueText));
  SetCurrentValueLabel();
}

int LabeledSlider::maxValueText() const
{
  return this->txtMax->text().toInt();
}

void LabeledSlider::setMaxValueText(const int maxValueText)
{
  this->txtMax->setText(QString::number(maxValueText));
  SetCurrentValueLabel();
}
