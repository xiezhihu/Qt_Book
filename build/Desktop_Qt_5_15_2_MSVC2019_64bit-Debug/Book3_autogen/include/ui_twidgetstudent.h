/********************************************************************************
** Form generated from reading UI file 'twidgetstudent.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TWIDGETSTUDENT_H
#define UI_TWIDGETSTUDENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TWidgetStudent
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QPushButton *btnPerson;
    QPushButton *btnMessage;
    QPushButton *btnquery;
    QPushButton *btnQuit;
    QSpacerItem *verticalSpacer_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labIcon;
    QVBoxLayout *verticalLayout_3;
    QLabel *labName;
    QLabel *labid;
    QLabel *labDebt;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QFrame *line;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QTableView *tableView;
    QWidget *tab_3;
    QWidget *tab_4;
    QWidget *tab_2;

    void setupUi(QWidget *TWidgetStudent)
    {
        if (TWidgetStudent->objectName().isEmpty())
            TWidgetStudent->setObjectName(QString::fromUtf8("TWidgetStudent"));
        TWidgetStudent->resize(640, 410);
        horizontalLayout = new QHBoxLayout(TWidgetStudent);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(TWidgetStudent);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 99, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        btnPerson = new QPushButton(groupBox_2);
        btnPerson->setObjectName(QString::fromUtf8("btnPerson"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnPerson->sizePolicy().hasHeightForWidth());
        btnPerson->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(14);
        btnPerson->setFont(font);
        btnPerson->setAutoFillBackground(false);
        btnPerson->setCheckable(true);
        btnPerson->setChecked(true);
        btnPerson->setAutoRepeat(false);
        btnPerson->setAutoExclusive(true);
        btnPerson->setAutoDefault(false);
        btnPerson->setFlat(true);

        verticalLayout->addWidget(btnPerson);

        btnMessage = new QPushButton(groupBox_2);
        btnMessage->setObjectName(QString::fromUtf8("btnMessage"));
        sizePolicy.setHeightForWidth(btnMessage->sizePolicy().hasHeightForWidth());
        btnMessage->setSizePolicy(sizePolicy);
        btnMessage->setFont(font);
        btnMessage->setCheckable(true);
        btnMessage->setAutoExclusive(true);
        btnMessage->setFlat(true);

        verticalLayout->addWidget(btnMessage);

        btnquery = new QPushButton(groupBox_2);
        btnquery->setObjectName(QString::fromUtf8("btnquery"));
        sizePolicy.setHeightForWidth(btnquery->sizePolicy().hasHeightForWidth());
        btnquery->setSizePolicy(sizePolicy);
        btnquery->setFont(font);
        btnquery->setCheckable(true);
        btnquery->setAutoExclusive(true);
        btnquery->setFlat(true);

        verticalLayout->addWidget(btnquery);

        btnQuit = new QPushButton(groupBox_2);
        btnQuit->setObjectName(QString::fromUtf8("btnQuit"));
        sizePolicy.setHeightForWidth(btnQuit->sizePolicy().hasHeightForWidth());
        btnQuit->setSizePolicy(sizePolicy);
        btnQuit->setFont(font);
        btnQuit->setCheckable(true);
        btnQuit->setAutoExclusive(true);
        btnQuit->setFlat(true);

        verticalLayout->addWidget(btnQuit);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 1);

        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer_2 = new QSpacerItem(20, 99, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 2);
        verticalLayout_2->setStretch(2, 1);

        horizontalLayout->addWidget(groupBox);

        tabWidget = new QTabWidget(TWidgetStudent);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_6 = new QVBoxLayout(tab);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labIcon = new QLabel(groupBox_3);
        labIcon->setObjectName(QString::fromUtf8("labIcon"));
        labIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(labIcon);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labName = new QLabel(groupBox_3);
        labName->setObjectName(QString::fromUtf8("labName"));
        labName->setFont(font);

        verticalLayout_3->addWidget(labName);

        labid = new QLabel(groupBox_3);
        labid->setObjectName(QString::fromUtf8("labid"));
        labid->setFont(font);

        verticalLayout_3->addWidget(labid);

        labDebt = new QLabel(groupBox_3);
        labDebt->setObjectName(QString::fromUtf8("labDebt"));
        labDebt->setFont(font);

        verticalLayout_3->addWidget(labDebt);


        horizontalLayout_2->addLayout(verticalLayout_3);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        horizontalSpacer = new QSpacerItem(287, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        horizontalLayout_3->setStretch(0, 2);
        horizontalLayout_3->setStretch(1, 3);

        verticalLayout_6->addWidget(groupBox_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_5->addWidget(label);

        line = new QFrame(tab);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line);


        verticalLayout_6->addLayout(verticalLayout_5);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(groupBox_4);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout_4->addWidget(tableView);


        verticalLayout_6->addWidget(groupBox_4);

        verticalLayout_6->setStretch(0, 2);
        verticalLayout_6->setStretch(2, 4);
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 5);

        retranslateUi(TWidgetStudent);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TWidgetStudent);
    } // setupUi

    void retranslateUi(QWidget *TWidgetStudent)
    {
        TWidgetStudent->setWindowTitle(QCoreApplication::translate("TWidgetStudent", "Form", nullptr));
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QString());
        btnPerson->setText(QCoreApplication::translate("TWidgetStudent", "\344\270\252\344\272\272\344\270\255\345\277\203", nullptr));
        btnMessage->setText(QCoreApplication::translate("TWidgetStudent", "\344\277\241\346\201\257\344\277\256\346\224\271", nullptr));
        btnquery->setText(QCoreApplication::translate("TWidgetStudent", "\345\233\276\344\271\246\346\237\245\350\257\242", nullptr));
        btnQuit->setText(QCoreApplication::translate("TWidgetStudent", "\351\200\200\345\207\272\347\231\273\345\275\225", nullptr));
        groupBox_3->setTitle(QString());
        labIcon->setText(QCoreApplication::translate("TWidgetStudent", "icon", nullptr));
        labName->setText(QCoreApplication::translate("TWidgetStudent", "\345\247\223\345\220\215", nullptr));
        labid->setText(QCoreApplication::translate("TWidgetStudent", "ID", nullptr));
        labDebt->setText(QCoreApplication::translate("TWidgetStudent", "\346\254\240\346\254\276\351\207\221\351\242\235", nullptr));
        label->setText(QCoreApplication::translate("TWidgetStudent", "\345\275\223\345\211\215\345\200\237\351\230\205", nullptr));
        groupBox_4->setTitle(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("TWidgetStudent", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("TWidgetStudent", "\351\241\265", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("TWidgetStudent", "\351\241\265", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("TWidgetStudent", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TWidgetStudent: public Ui_TWidgetStudent {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWIDGETSTUDENT_H
