/********************************************************************************
** Form generated from reading UI file 'admin_adduser.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMIN_ADDUSER_H
#define UI_ADMIN_ADDUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_admin_adduser
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QFrame *frame_2;
    QFrame *frame_8;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout;
    QPushButton *btnPersonal;
    QPushButton *btnInformationchange;
    QPushButton *btnSearchbook;
    QPushButton *btnClassify;
    QPushButton *btnLogout;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_4;
    QPushButton *pushButton_2;
    QPushButton *btnAddMany;
    QFrame *frame_5;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *etName;
    QLineEdit *etCode;
    QLineEdit *etMail;
    QRadioButton *radioMan;
    QRadioButton *radioWoman;
    QLineEdit *lineEdit_4;
    QPushButton *btnDept;
    QPushButton *btnHeadIcon;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnAdd;

    void setupUi(QWidget *admin_adduser)
    {
        if (admin_adduser->objectName().isEmpty())
            admin_adduser->setObjectName(QStringLiteral("admin_adduser"));
        admin_adduser->resize(1021, 722);
        horizontalLayout = new QHBoxLayout(admin_adduser);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(admin_adduser);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(300, 700));
        frame->setStyleSheet(QLatin1String("\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(51, 153, 255,1), stop:1 rgba(52, 246, 177,1));"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(70, 30, 171, 176));
        frame_2->setMinimumSize(QSize(0, 0));
        frame_2->setBaseSize(QSize(0, 100));
        frame_2->setStyleSheet(QLatin1String("image: url(:/Library/images/logo2.png);\n"
"background:transparent;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_8 = new QFrame(frame);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setGeometry(QRect(0, 230, 301, 491));
        frame_8->setStyleSheet(QStringLiteral("background:transparent;"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame_8);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        btnPersonal = new QPushButton(frame_8);
        btnPersonal->setObjectName(QStringLiteral("btnPersonal"));
        btnPersonal->setMinimumSize(QSize(0, 100));
        btnPersonal->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color:white;\n"
"background:transparent;\n"
"cursor:pointer;"));

        verticalLayout->addWidget(btnPersonal);

        btnInformationchange = new QPushButton(frame_8);
        btnInformationchange->setObjectName(QStringLiteral("btnInformationchange"));
        btnInformationchange->setMinimumSize(QSize(0, 100));
        btnInformationchange->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color:white;\n"
"background:transparent;\n"
"cursor:pointer;\n"
"background-color:rgba(255,255,255,0.4);"));

        verticalLayout->addWidget(btnInformationchange);

        btnSearchbook = new QPushButton(frame_8);
        btnSearchbook->setObjectName(QStringLiteral("btnSearchbook"));
        btnSearchbook->setMinimumSize(QSize(0, 100));
        btnSearchbook->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color:white;\n"
"background:transparent;\n"
"cursor:pointer;\n"
""));

        verticalLayout->addWidget(btnSearchbook);

        btnClassify = new QPushButton(frame_8);
        btnClassify->setObjectName(QStringLiteral("btnClassify"));
        btnClassify->setMinimumSize(QSize(0, 100));
        btnClassify->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color:white;\n"
"background:transparent;\n"
"cursor:pointer;"));

        verticalLayout->addWidget(btnClassify);

        btnLogout = new QPushButton(frame_8);
        btnLogout->setObjectName(QStringLiteral("btnLogout"));
        btnLogout->setMinimumSize(QSize(0, 100));
        btnLogout->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color:white;\n"
"background:transparent;\n"
"cursor:pointer;\n"
""));

        verticalLayout->addWidget(btnLogout);


        verticalLayout_6->addLayout(verticalLayout);


        horizontalLayout->addWidget(frame);

        frame_3 = new QFrame(admin_adduser);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setStyleSheet(QStringLiteral("background:white;"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame_4 = new QFrame(frame_3);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy1);
        frame_4->setMaximumSize(QSize(16777215, 70));
        frame_4->setStyleSheet(QString::fromUtf8("border:2px solid #cccccc;\n"
"font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-top:0;\n"
"border-left:0;\n"
"border-right:0;\n"
"margin-left:20px;\n"
"margin-right:20px;"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        pushButton_2 = new QPushButton(frame_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 10, 191, 51));
        pushButton_2->setStyleSheet(QLatin1String("border:none;\n"
""));
        btnAddMany = new QPushButton(frame_4);
        btnAddMany->setObjectName(QStringLiteral("btnAddMany"));
        btnAddMany->setGeometry(QRect(470, 30, 131, 18));
        btnAddMany->setStyleSheet(QLatin1String("color:#4695d2;\n"
"border:none;\n"
"font-size:18px;"));

        verticalLayout_2->addWidget(frame_4);

        frame_5 = new QFrame(frame_3);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        lineEdit = new QLineEdit(frame_5);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(70, 70, 61, 31));
        lineEdit->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:none;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        lineEdit_2 = new QLineEdit(frame_5);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(70, 120, 61, 31));
        lineEdit_2->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:none;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        lineEdit_3 = new QLineEdit(frame_5);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(70, 170, 61, 31));
        lineEdit_3->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:none;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        lineEdit_5 = new QLineEdit(frame_5);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(70, 290, 61, 31));
        lineEdit_5->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:none;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        lineEdit_6 = new QLineEdit(frame_5);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(70, 380, 61, 31));
        lineEdit_6->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:none;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        etName = new QLineEdit(frame_5);
        etName->setObjectName(QStringLiteral("etName"));
        etName->setGeometry(QRect(130, 70, 201, 31));
        etName->setStyleSheet(QString::fromUtf8("border:1px solid #cccccc;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        etCode = new QLineEdit(frame_5);
        etCode->setObjectName(QStringLiteral("etCode"));
        etCode->setGeometry(QRect(130, 120, 201, 31));
        etCode->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:1px solid #cccccc;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        etMail = new QLineEdit(frame_5);
        etMail->setObjectName(QStringLiteral("etMail"));
        etMail->setGeometry(QRect(130, 290, 251, 31));
        etMail->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:1px solid #cccccc;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        radioMan = new QRadioButton(frame_5);
        radioMan->setObjectName(QStringLiteral("radioMan"));
        radioMan->setGeometry(QRect(140, 220, 41, 31));
        radioMan->setCheckable(true);
        radioMan->setChecked(true);
        radioWoman = new QRadioButton(frame_5);
        radioWoman->setObjectName(QStringLiteral("radioWoman"));
        radioWoman->setGeometry(QRect(200, 220, 41, 31));
        lineEdit_4 = new QLineEdit(frame_5);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(70, 220, 61, 31));
        lineEdit_4->setStyleSheet(QString::fromUtf8("color:#808080;\n"
"border:none;\n"
"font-size:18px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"text-align:left;"));
        btnDept = new QPushButton(frame_5);
        btnDept->setObjectName(QStringLiteral("btnDept"));
        btnDept->setGeometry(QRect(130, 170, 201, 31));
        btnDept->setStyleSheet(QLatin1String("color:#4695d2;\n"
"border:none;\n"
"background:white;\n"
"text-size:80px;"));
        btnHeadIcon = new QPushButton(frame_5);
        btnHeadIcon->setObjectName(QStringLiteral("btnHeadIcon"));
        btnHeadIcon->setGeometry(QRect(120, 340, 131, 101));

        verticalLayout_2->addWidget(frame_5);

        frame_6 = new QFrame(frame_3);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        sizePolicy1.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy1);
        frame_6->setMaximumSize(QSize(16777215, 60));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_6);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnAdd = new QPushButton(frame_6);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btnAdd->sizePolicy().hasHeightForWidth());
        btnAdd->setSizePolicy(sizePolicy2);
        btnAdd->setStyleSheet(QString::fromUtf8("color:white;\n"
"border:none;\n"
"font-size:20px;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(51, 153, 255,1), stop:1 rgba(52, 246, 177,1));\n"
"padding-top:5;\n"
"padding-bottom:5;"));

        horizontalLayout_2->addWidget(btnAdd);


        verticalLayout_2->addWidget(frame_6);


        horizontalLayout->addWidget(frame_3);


        retranslateUi(admin_adduser);

        QMetaObject::connectSlotsByName(admin_adduser);
    } // setupUi

    void retranslateUi(QWidget *admin_adduser)
    {
        admin_adduser->setWindowTitle(QApplication::translate("admin_adduser", "admin_adduser", Q_NULLPTR));
        btnPersonal->setText(QApplication::translate("admin_adduser", "\344\270\252\344\272\272\344\270\255\345\277\203", Q_NULLPTR));
        btnInformationchange->setText(QApplication::translate("admin_adduser", "\350\264\246\345\217\267\347\256\241\347\220\206", Q_NULLPTR));
        btnSearchbook->setText(QApplication::translate("admin_adduser", "\345\233\276\344\271\246\347\256\241\347\220\206", Q_NULLPTR));
        btnClassify->setText(QApplication::translate("admin_adduser", "\345\210\206\347\261\273\347\256\241\347\220\206", Q_NULLPTR));
        btnLogout->setText(QApplication::translate("admin_adduser", "\351\200\200\345\207\272\347\231\273\345\275\225", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("admin_adduser", "\346\267\273\345\212\240/\344\277\256\346\224\271\345\255\246\347\224\237", Q_NULLPTR));
        btnAddMany->setText(QApplication::translate("admin_adduser", "\346\211\271\351\207\217\346\267\273\345\212\240", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("admin_adduser", "\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("admin_adduser", "\345\255\246\345\217\267\357\274\232", Q_NULLPTR));
        lineEdit_3->setText(QApplication::translate("admin_adduser", "\344\270\223\344\270\232\357\274\232", Q_NULLPTR));
        lineEdit_5->setText(QApplication::translate("admin_adduser", "\351\202\256\347\256\261\357\274\232", Q_NULLPTR));
        lineEdit_6->setText(QApplication::translate("admin_adduser", "\345\244\264\345\203\217\357\274\232", Q_NULLPTR));
        etName->setText(QString());
        radioMan->setText(QApplication::translate("admin_adduser", "\347\224\267", Q_NULLPTR));
        radioWoman->setText(QApplication::translate("admin_adduser", "\345\245\263", Q_NULLPTR));
        lineEdit_4->setText(QApplication::translate("admin_adduser", "\346\200\247\345\210\253\357\274\232", Q_NULLPTR));
        btnDept->setText(QApplication::translate("admin_adduser", "\350\257\267\351\200\211\346\213\251\344\270\223\344\270\232", Q_NULLPTR));
        btnHeadIcon->setText(QApplication::translate("admin_adduser", "PushButton", Q_NULLPTR));
        btnAdd->setText(QApplication::translate("admin_adduser", "\347\241\256\350\256\244\346\217\220\344\272\244", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class admin_adduser: public Ui_admin_adduser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMIN_ADDUSER_H
