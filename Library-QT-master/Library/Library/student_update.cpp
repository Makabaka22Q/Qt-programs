#include "student_update.h"
#include<string>
#include"filedb.h"
#include"student.h"
#include"student_updatepassword.h"
#include"userConfig.h"
#include"library.h"
#include <QTextCodec>
#include"student_index.h"
#include"student_searchbook.h"
#include<QMessageBox>
#include<QFileDialog>
#include<QDateTime>
#include"student_repay.h"
#include"recommendBuffer.h"
student_update::student_update(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.lineEdit->setEnabled(false);
	ui.lineEdit_2->setEnabled(false);
	ui.lineEdit_3->setEnabled(false);
	//ui.lineEdit_4->setEnabled(false);
	ui.lineEdit_5->setEnabled(false);
	ui.lineEdit_6->setEnabled(false);
	ui.lineEdit_7->setEnabled(false);
	ui.lineEdit_8->setEnabled(false);
	ui.lineEdit_9->setEnabled(false);
	ui.btnInformationchange->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnHeadIcon->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.pushButton_2->installEventFilter(this);
	ui.pushButton_3->installEventFilter(this);
	ui.pushButton_4->installEventFilter(this);
	isChangeIcon = false;
	InitThisPage();
}

student_update::~student_update()
{

}

bool student_update::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj == ui.pushButton_2 && event->type() == QEvent::MouseButtonPress) {
		student_updatePassword *rec = new student_updatePassword;
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
	if (obj == ui.pushButton_3 && event->type() == QEvent::MouseButtonPress) {
		student_repay *rec = new student_repay;
		this->close();
		rec->show();
		return true;
	}
	if (obj == ui.btnHeadIcon && event->type() == QEvent::MouseButtonPress) {
		openFileDiag();
		return true;
	}
	if (obj == ui.btnLogout && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::StandardButton button;
		button = QMessageBox::question(this, chartoqs("????????"),
			QString(chartoqs("?????????????")),
			QMessageBox::Yes | QMessageBox::No);
		if (button == QMessageBox::No) {
			event->ignore();  //??????????????????????????
		}
		else if (button == QMessageBox::Yes) {
			recommendBuffer::Resert();
			Library *rec = new Library;
			this->close();
			rec->show();
			event->accept();  //??????????????????????
		}
	}
	if (obj == ui.pushButton_4 && event->type() == QEvent::MouseButtonPress) {
		string newmail = qstostr(ui.lineEdit_10->text());
		if (newmail.length() >50) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("????????????"), QMessageBox::Ok);
			return true;
		}
		if (newmail.length()==0) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("????????"), QMessageBox::Ok);
			return true;
		}
		if (newmail!="") {
			string::size_type idx;
			idx = newmail.find("@");
			if (idx == string::npos) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("????????????????"), QMessageBox::Ok);
				return true;
			}
			else {
				//????????????
				//??????????
				char* thenewmail;
				QByteArray ba = ui.lineEdit_10->text().toLatin1();
				thenewmail = ba.data();
				
				//??????????????????????
				Student student;
				vector<string>VALUES_1;
				vector<Student>res;
				VALUES_1.push_back("one");
				VALUES_1.push_back("id");
				student.setId(userConfig::id);
				FileDB::select("student", student, VALUES_1, res);
				//????????????????
				Student primarystudent;
				vector<string>VALUES_2;
				VALUES_2.push_back("one");
				VALUES_2.push_back("id");
				primarystudent.setId(userConfig::id);
				res[0].setMail(thenewmail);
				if (isChangeIcon) {
					//??????????
					char* thenewicon;
					QByteArray ba2 = filename.toLatin1();
					thenewicon = ba2.data();
					res[0].setIcon(thenewicon);
					(*img).save(filename);
					recommendBuffer::headBuffer.clear();
					recommendBuffer::headBuffer.push_back(*img);
				}
				int judge = FileDB::update("student", primarystudent, res[0], VALUES_2);
				
				
				if (judge>0) {

					QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("????????"), QMessageBox::Ok);
					return true;
				}
			}
		}
		else {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("??????????????"), QMessageBox::Ok);
			return true;
		}
	}
	return false;
}
void student_update::openFileDiag() {
	filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("????????"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty()) {
		return;
	}
	else {
		img = new QPixmap;
		if (!(img->load(filename))) {
			QMessageBox::information(this,
				QString::fromLocal8Bit("????????????"),
				QString::fromLocal8Bit("????????????!"));
		}
		QDateTime time = QDateTime::currentDateTime();   //????????????  
		int timeT = time.toTime_t();   //????????????????????
		filename="images/"+ QString::number(timeT, 10)+".jpg";
		ui.btnHeadIcon->setIcon(*img);
		isChangeIcon = true;
		ui.btnHeadIcon->setIconSize(QSize((*img).width(), (*img).height()));
		return;
	}
}

void student_update::InitThisPage() {
	//????????????????
	Student student;
	vector<string>VALUES; 
	vector<Student>res; 
	VALUES.push_back("one"); 
	VALUES.push_back("id"); 
	student.setId(userConfig::id); 
	FileDB::select("student", student, VALUES, res);
	//??????????????????
	ui.lineEdit_7->setText(chartoqs(res[0].username));
	ui.lineEdit_8->setText(chartoqs(res[0].usercode));
	ui.lineEdit_9->setText(chartoqs(res[0].dept));
	ui.lineEdit_10->setText(chartoqs(res[0].mail));
	filename=strtoqs(res[0].icon);
	QPixmap pixmap = recommendBuffer::headBuffer[0];
	ui.btnHeadIcon->setText("");
	ui.btnHeadIcon->setStyleSheet(
		"color:#4695d2;"
		"border:none;"
		"background:white;"
		"text-size:20px;"
	);
	ui.btnHeadIcon->setIcon(pixmap);
	ui.btnHeadIcon->setIconSize(QSize(pixmap.width(), pixmap.height()));
}

QString student_update::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string student_update::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}

QString student_update::chartoqs(char *p) {
	string s = p;
	return QString(QString::fromLocal8Bit(s.c_str()));
}


