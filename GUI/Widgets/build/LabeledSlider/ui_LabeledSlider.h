/********************************************************************************
** Form generated from reading UI file 'LabeledSlider.ui'
**
** Created: Wed Dec 14 14:25:22 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABELEDSLIDER_H
#define UI_LABELEDSLIDER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LabeledSlider
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *lblName;
    QHBoxLayout *horizontalLayout;
    QLineEdit *txtMin;
    QSlider *horizontalSlider;
    QLineEdit *txtMax;
    QLabel *lblCurrent;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *LabeledSlider)
    {
        if (LabeledSlider->objectName().isEmpty())
            LabeledSlider->setObjectName(QString::fromUtf8("LabeledSlider"));
        LabeledSlider->resize(227, 98);
        verticalLayout = new QVBoxLayout(LabeledSlider);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        lblName = new QLabel(LabeledSlider);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblName);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        txtMin = new QLineEdit(LabeledSlider);
        txtMin->setObjectName(QString::fromUtf8("txtMin"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(txtMin->sizePolicy().hasHeightForWidth());
        txtMin->setSizePolicy(sizePolicy);
        txtMin->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(txtMin);

        horizontalSlider = new QSlider(LabeledSlider);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        sizePolicy.setHeightForWidth(horizontalSlider->sizePolicy().hasHeightForWidth());
        horizontalSlider->setSizePolicy(sizePolicy);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setValue(50);
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        txtMax = new QLineEdit(LabeledSlider);
        txtMax->setObjectName(QString::fromUtf8("txtMax"));
        sizePolicy.setHeightForWidth(txtMax->sizePolicy().hasHeightForWidth());
        txtMax->setSizePolicy(sizePolicy);
        txtMax->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(txtMax);


        verticalLayout->addLayout(horizontalLayout);

        lblCurrent = new QLabel(LabeledSlider);
        lblCurrent->setObjectName(QString::fromUtf8("lblCurrent"));
        lblCurrent->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblCurrent);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(LabeledSlider);

        QMetaObject::connectSlotsByName(LabeledSlider);
    } // setupUi

    void retranslateUi(QWidget *LabeledSlider)
    {
        LabeledSlider->setWindowTitle(QApplication::translate("LabeledSlider", "Form", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("LabeledSlider", "Name", 0, QApplication::UnicodeUTF8));
        txtMin->setText(QApplication::translate("LabeledSlider", "0", 0, QApplication::UnicodeUTF8));
        txtMax->setText(QApplication::translate("LabeledSlider", "100", 0, QApplication::UnicodeUTF8));
        lblCurrent->setText(QApplication::translate("LabeledSlider", "current", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LabeledSlider: public Ui_LabeledSlider {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABELEDSLIDER_H
