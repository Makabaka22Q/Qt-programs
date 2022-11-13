#include "student_updatepassword.h"
#include "student_update.h"
#include<string>
#include"filedb.h"
#include"student.h"
#include"userConfig.h"
#include"library.h"
#include<QMessageBox>
#include <QTextCodec>
#include"student_index.h"
#include"student_searchbook.h"
#include<vector>
#include"userConfig.h"
#include"student_repay.h"
#include"recommendBuffer.h"
student_updatePassword::student_updatePassword(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.lineEdit->setEnabled(false);
	ui.lineEdit_2->setEnabled(false);
	ui.lineEdit_3->setEnabled(false);
	ui.btnInformationchange->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.pushButton->installEventFilter(this);
	ui.pushButton_3->installEventFilter(this);
	ui.pushButton_4->installEventFilter(this);
}

student_updatePassword::~student_updatePassword()
{

}

bool student_updatePassword::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj == ui.pushButton && event->type() == QEvent::MouseButtonPress) {
		student_update *rec = new student_update;
		this->close();
		rec->show();
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		student_index *rec = new student_index;
		this->close();
		rec->show();
	}
	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		student_searchBook *rec = new student_searchBook;
		this->close();
		rec->show();
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
	if (obj == ui.pushButton_3 && event->type() == QEvent::MouseButtonPress) {
		student_repay *rec = new student_repay;
		this->close();
		rec->show();
		return true;
	}
	if (obj == ui.pushButton_4 && event->type() == QEvent::MouseButtonPress) {
		//�޸�����
		if (ui.etPassword->text() == userConfig::password) {
			string newpassword = qstostr(ui.etNewpassword->text());
			if (newpassword.length() >50) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��д����̫��"), QMessageBox::Ok);
				return true;
			}
			if (newpassword.length() ==0) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����Ϊ��"), QMessageBox::Ok);
				return true;
			}
			string newpassword_2 = qstostr(ui.etNewpassword_2->text());
			if (newpassword!="") {
				if (newpassword == newpassword_2) {
					//��ȡ������
					char* thenewpassword;
					QByteArray ba = ui.etNewpassword->text().toLatin1();
					thenewpassword = ba.data();
					//��ȡ�û�ԭ����������Ϣ
					Student student;
					vector<string>VALUES_1;
					vector<Student>res;
					VALUES_1.push_back("one");
					VALUES_1.push_back("id");
					student.setId(userConfig::id);
					FileDB::select("student", student, VALUES_1, res);
					//�趨Ҫ�޸ĵ��û�
					Student primarystudent;
					vector<string>VALUES_2;
					VALUES_2.push_back("one");
					VALUES_2.push_back("id");
					primarystudent.setId(userConfig::id);
					//�޸��û�
					//Student newstudent;
					//newstudent.setUsercode(res[0].usercode);
					//newstudent.setUsername(res[0].username);
					//newstudent.setPassword(thenewpassword);//�޸�����
					//newstudent.setSex(res[0].sex);
					//newstudent.setDept(res[0].dept);
					//newstudent.setIcon(res[0].icon);
					//newstudent.setMail(res[0].mail);
					//newstudent.setMoney(res[0].money);
					res[0].setPassword(thenewpassword);
					int judge = FileDB::update("student", primarystudent, res[0], VALUES_2);

					if (judge>0) {

						QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�޸ĳɹ�"), QMessageBox::Ok);
						return true;
					}
					else {
						QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("δ֪����"), QMessageBox::Ok);
						return true;
					}
				}
				else{
					QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��������������벻һ��"), QMessageBox::Ok);
					return true;
				}
			}
			else {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�����벻��Ϊ��"), QMessageBox::Ok);
				return true;
			}
		}
		else {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�������벻��ȷ"), QMessageBox::Ok);
			return true;
		}
	}
	return false;
}

QString student_updatePassword::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string student_updatePassword::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}
