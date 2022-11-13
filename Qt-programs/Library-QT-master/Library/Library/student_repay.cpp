#include "student_repay.h"
#include "student_updatepassword.h"
#include "student_update.h"
#include<string>
#include"filedb.h"
#include"library.h"
#include"student.h"
#include"userConfig.h"
#include <QTextCodec>
#include"student_index.h"
#include"student_searchbook.h"
#include<QMessageBox>
#include<vector>
#include"userConfig.h"
#include"recommendBuffer.h"
student_repay::student_repay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.lineEdit->setEnabled(false);
	ui.etRepay->setEnabled(false);
	ui.btnLogout->installEventFilter(this);
	ui.btnInformationchange->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.pushButton->installEventFilter(this);
	ui.pushButton_2->installEventFilter(this);
	ui.btnRepay->installEventFilter(this);
	InitThisPage();
}

student_repay::~student_repay()
{

}

bool student_repay::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj == ui.pushButton && event->type() == QEvent::MouseButtonPress) {
		student_update *rec = new student_update;
		this->close();
		rec->show();
		return true;
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		student_index *rec = new student_index;
		this->close();
		rec->show();
		return true;
	}
	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		student_searchBook *rec = new student_searchBook;
		this->close();
		rec->show();
		return true;
	}
	if (obj == ui.btnLogout && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::StandardButton button;
		button = QMessageBox::question(this, QString::fromLocal8Bit("�˳�����"),
			QString(QString::fromLocal8Bit("ȷ���˳�����?")),
			QMessageBox::Yes | QMessageBox::No);
		if (button == QMessageBox::No) {
			event->ignore();  //�����˳��źţ������������
		}
		else if (button == QMessageBox::Yes) {
			recommendBuffer::Resert();
			Library *rec = new Library;
			this->close();
			rec->show();
			event->accept();  //�����˳��źţ������˳�
		}
	}
	if (obj == ui.pushButton_2 && event->type() == QEvent::MouseButtonPress) {
		student_updatePassword *rec = new student_updatePassword;
		this->close();
		rec->show();
		return true;
	}
	if (obj == ui.btnRepay && event->type() == QEvent::MouseButtonPress) {
		if (ui.etRepay->text()!="0") {
			Student student;
			vector<string>VALUES;
			vector<Student>resStudent;
			VALUES.push_back("one");
			VALUES.push_back("id");
			student.setId(userConfig::id);
			FileDB::select("student", student, VALUES, resStudent);
			//Ƿ�������޸���Ϣ
			Student newstudent;
			newstudent.setUsercode(resStudent[0].usercode);
			newstudent.setUsername(resStudent[0].username);
			newstudent.setPassword(resStudent[0].password);
			newstudent.setSex(resStudent[0].sex);
			newstudent.setDept(resStudent[0].dept);
			newstudent.setIcon(resStudent[0].icon);
			newstudent.setMail(resStudent[0].mail);
			newstudent.setMoney(0);
			int res = FileDB::update("student", student, newstudent, VALUES);
			if (res > 0) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ɹ�"), QMessageBox::Ok);
				InitThisPage();
			}
			else {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ʧ��"), QMessageBox::Ok);
			}
			return true;
		}
		else{
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��ǰδǷ��"), QMessageBox::Ok);
		}
	}
	return false;
}

void student_repay::InitThisPage() {
	Student student;
	vector<string>VALUES;
	vector<Student>resStudent;
	VALUES.push_back("one");
	VALUES.push_back("id");
	student.setId(userConfig::id);
	FileDB::select("student", student, VALUES, resStudent);
	ui.etRepay->setText(QString("%1").arg(resStudent[0].money));
}
