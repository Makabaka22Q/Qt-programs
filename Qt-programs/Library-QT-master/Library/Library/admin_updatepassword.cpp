#include "admin_updatepassword.h"
#include<QTextCodec>
#include "admin_index.h"
#include "admin_searchbook.h"
#include "admin_searchuser.h"
#include"filedb.h"
#include"admin.h"
#include"library.h"
#include "userConfig.h"
#include"admin_classify.h"
#include<QMessageBox>
admin_updatepassword::admin_updatepassword(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.lineEdit->setEnabled(false);
	ui.lineEdit_13->setEnabled(false);
	ui.lineEdit_14->setEnabled(false);
	ui.etSname->setEnabled(false);
	ui.btnPersonal->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnSearchuser->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnClassify->installEventFilter(this);
	ui.btnSubmit->installEventFilter(this);
	ui.pushButton_6->installEventFilter(this);
	InitThisPage();
}

string qstostr(const QString &s) {
	return string((const char*)s.toLocal8Bit());
}
bool admin_updatepassword::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj == ui.pushButton_6 && event->type() == QEvent::MouseButtonPress) {
		admin_index *rec = new admin_index;
		rec->show();
		this->close();
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
			Library *rec = new Library;
			this->close();
			rec->show();
			event->accept();  //�����˳��źţ������˳�
		}
	}
	if (obj == ui.btnSearchuser && event->type() == QEvent::MouseButtonPress) {
		admin_searchuser *rec = new admin_searchuser;
		this->close();
		rec->show();
	}
	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		admin_searchbook *rec = new admin_searchbook;
		this->close();
		rec->show();
	}
	if (obj == ui.btnClassify && event->type() == QEvent::MouseButtonPress) {
		admin_classify *rec = new admin_classify;
		this->close();
		rec->show();
	}
	if (obj == ui.btnSubmit && event->type() == QEvent::MouseButtonPress) {
		if (ui.etPassword->text() == userConfig::password) {
			string newpassword = qstostr(ui.etNewpassword->text());
			if (newpassword.length() >50) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��д����̫��"), QMessageBox::Ok);
				return true;
			}
			if (newpassword.length() ==0) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("���벻��Ϊ��"), QMessageBox::Ok);
				return true;
			}
			string newpassword_2 = qstostr(ui.etNewpassword_2->text());
			if (newpassword != "") {
				if (newpassword == newpassword_2) {
					//��ȡ������
					char* thenewpassword;
					QByteArray ba = ui.etNewpassword->text().toLatin1();
					thenewpassword = ba.data();
					//��ȡ�û�ԭ����������Ϣ
					Admin admin;
					vector<string>VALUES_1;
					vector<Admin>res;
					VALUES_1.push_back("one");
					VALUES_1.push_back("id");
					admin.setId(userConfig::id);
					FileDB::select("admin", admin, VALUES_1, res);
					//�趨Ҫ�޸ĵ��û�
					Admin primarystudent;
					vector<string>VALUES_2;
					VALUES_2.push_back("one");
					VALUES_2.push_back("id");
					primarystudent.setId(userConfig::id);
					
					res[0].setPassword(thenewpassword);
					int judge = FileDB::update("admin", primarystudent, res[0], VALUES_2);

					if (judge>0) {

						QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�޸ĳɹ�"), QMessageBox::Ok);
						return true;
					}
					else {
						QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("δ֪����"), QMessageBox::Ok);
						return true;
					}
				}
				else {
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

admin_updatepassword::~admin_updatepassword()
{

}

void admin_updatepassword::InitThisPage() {
	Admin admin;
	admin.setId(userConfig::id);
	vector<string>VALUES;
	VALUES.push_back("one");
	vector<Admin>resAdmin;
	FileDB::select("admin", admin, VALUES, resAdmin);
	ui.etSname->setText(QString::fromLocal8Bit("��ӭ����") + QString::fromLocal8Bit(resAdmin[0].account));
}

