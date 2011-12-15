#include <iostream>

#include "CoordinateEditWidget.h"

#include <QDoubleValidator>

// NOTE: You MUST call the parent constructor, or QtDesigner will crash.
CoordinateEditWidget::CoordinateEditWidget(QWidget *parent) : QWidget(parent)
{
  this->setupUi(this);

  this->Validator = new QDoubleValidator(0.0d, 10000.0d, 2, this);
  this->txtX->setValidator(this->Validator);
  this->txtY->setValidator(this->Validator);
  this->txtZ->setValidator(this->Validator);

  this->txtX->setText(QString::number(0.0));
  this->txtY->setText(QString::number(0.0));
  this->txtZ->setText(QString::number(0.0));
}

QVector<double> CoordinateEditWidget::GetCoordinate()
{
  QVector<double> coord(3);
  coord[0] = this->txtX->text().toDouble();
  coord[1] = this->txtY->text().toDouble();
  coord[2] = this->txtZ->text().toDouble();
  return coord;
}

void CoordinateEditWidget::on_txtX_textChanged( const QString & text )
{
  QVector<double> coord = GetCoordinate();
  emit coordinateChanged(coord);
}

void CoordinateEditWidget::on_txtY_textChanged( const QString & text )
{
  QVector<double> coord = GetCoordinate();
  emit coordinateChanged(coord);
}

void CoordinateEditWidget::on_txtZ_textChanged( const QString & text )
{
  //std::vector<double> coord = GetCoordinate();
  QVector<double> coord = GetCoordinate();
  emit coordinateChanged(coord);
}

double CoordinateEditWidget::xText() const
{
  return this->txtX->text().toDouble();
}

void CoordinateEditWidget::setXText(const double x)
{
  this->txtX->setText(QString::number(x));
}

double CoordinateEditWidget::yText() const
{
  return this->txtY->text().toDouble();
}

void CoordinateEditWidget::setYText(const double y)
{
  this->txtX->setText(QString::number(y));
}

double CoordinateEditWidget::zText() const
{
  return this->txtZ->text().toDouble();
}

void CoordinateEditWidget::setZText(const double z)
{
  this->txtX->setText(QString::number(z));
}
