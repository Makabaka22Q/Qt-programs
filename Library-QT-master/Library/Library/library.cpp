#include "library.h"
#include"ui_library.h"
#include"student_index.h"
#include"filedb.h"
#include"student.h"
#include<string>
#include<iostream>
#include<QMessageBox>
#include <QTextCodec>
#include"userConfig.h"
#include"admin.h"
#include"admin_index.h"
using namespace std;



Library::Library(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.btnLogin->installEventFilter(this);
}

Library::~Library()
{

}

bool Library::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	if (obj == ui.btnLogin && event->type()==QEvent::MouseButtonPress) {
		char* ch1;
		QByteArray ba = ui.etUsername->text().toLatin1();
		ch1 = ba.data();
		//�����Ƿ�Ϊѧ���û�
		Student student;
		vector<string>VALUES;
		vector<Student>res;
		VALUES.push_back("one");
		VALUES.push_back("usercode");
		student.setUsercode(ch1);
		FileDB::select("student", student, VALUES, res);
		if (res.size() > 0) {//��ѧ���û�
			char* ch2;
			QByteArray ba = ui.etPassword->text().toLatin1();
			ch2 = ba.data();
			if (!strcmp(ch2, res[0].password)) {
				userConfig::id = res[0].id;
				connect(ui.btnLogin, SIGNAL(clicked()), this, SLOT(ClickButton()));
			}
			else {//���벻��
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�û����������������"), QMessageBox::Ok);
			}
		}
		else {//�����Ƿ�Ϊ����Ա�û�
			Admin admin;
			VALUES.clear();
			vector<Admin>resAdmin;
			VALUES.push_back("one");
			VALUES.push_back("account");
			admin.setAccount(ch1);
			FileDB::select("admin", admin, VALUES, resAdmin);
			if (resAdmin.size() > 0) {//�ǹ���Ա�û�
				char* ch2;
				QByteArray ba = ui.etPassword->text().toLatin1();
				ch2 = ba.data();
				if (!strcmp(ch2, resAdmin[0].password)) {
					userConfig::id = resAdmin[0].id;
					connect(ui.btnLogin, SIGNAL(clicked()), this, SLOT(ClickButtonToAdmin()));
				}
				else {//���벻��
					QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�û����������������"), QMessageBox::Ok);
					return true;
				}
			}
			else {//�˺Ų���
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�û����������������"), QMessageBox::Ok);
				return true;
			}
		}
	}
	return false;
}
QString Library::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string Library::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}

void Library::ClickButton() {
	userConfig::username = ui.etUsername->text();
	userConfig::password = ui.etPassword->text();
	student_index *rec = new student_index;
	rec->show();
	this->close();
}

void Library::ClickButtonToAdmin() {
	userConfig::username = ui.etUsername->text();
	userConfig::password = ui.etPassword->text();
	admin_index *rec = new admin_index;
	rec->show();
	this->close();
}
