#include "student_bookdetail.h"
#include"student_index.h"
#include"bookConfig.h"
#include"book.h"
#include"filedb.h"
#include"classify.h"
#include<vector>
#include<string>
#include<QMessageBox>
#include <QTextCodec>
#include"library.h"
#include<QDateTime>
#include"record.h"
#include"student.h"
#include"userConfig.h"
#include"bookConfig.h"
#include"bookMap.h"
#include"student_update.h"
#include"student_searchbook.h"
#include <qtnetwork/qnetworkaccessmanager>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkReply>
#include"recommendBuffer.h"
using namespace std;

student_bookDetail::student_bookDetail(QWidget *parent)
	: QWidget(parent)
{
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	ui.setupUi(this);
	showFullScreen();
	ui.etBookNameHead->setEnabled(false);
	ui.etBookName->setEnabled(false);
	ui.etAuthor->setEnabled(false);
	ui.etPublish->setEnabled(false);
	ui.btnLogout->installEventFilter(this);
	ui.etISBN->setEnabled(false);
	ui.etClassify->setEnabled(false);
	ui.etBookNameHead->setEnabled(false);
	ui.etBookName->setEnabled(false);
	ui.etAuthorHead->setEnabled(false);
	ui.etPublishHead->setEnabled(false);
	ui.etISBNHead->setEnabled(false);
	ui.etClassifyHead->setEnabled(false);
	ui.btnPersonal->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnInformationchange->installEventFilter(this);
	ui.btnBorrow->installEventFilter(this);
	ui.btnOrder->installEventFilter(this);
	InitThisPage();
}

student_bookDetail::~student_bookDetail()
{

}

void student_bookDetail::InitThisPage() {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	Book book;
	vector<string>VALUES;
	vector<Book>resBook;
	VALUES.push_back("one");
	VALUES.push_back("id");
	book.setId(bookConfig::bookId);
	FileDB::select("book", book, VALUES, resBook);
	if (resBook.size() > 0) {
		ui.etBookNameHead->setText(chartoqs(resBook[0].name));
		ui.etBookName->setText(chartoqs(resBook[0].name));
		ui.etAuthor->setText(chartoqs(resBook[0].author));
		ui.etPublish->setText(chartoqs(resBook[0].publish));
		ui.etISBN->setText(chartoqs(resBook[0].ISBN));
		Classify classify;
		vector<Classify>resClassify;
		classify.setId(resBook[0].classifyId);
		FileDB::select("classify", classify, VALUES, resClassify);
		ui.etClassify->setText(chartoqs(resClassify[0].name));
		ui.etScore->setText(QString("%1").arg(resBook[0].score));
		ui.etCount->setText(QString::number(resBook[0].count));
		ui.etNowCount->setText(QString::number(resBook[0].nowCount));

		//����ͼƬ
		QPixmap pixmap;
		if (strstr(resBook[0].cover, "images")) {
			pixmap.load(resBook[0].cover);
		}
		else {
			QUrl url(resBook[0].cover);
			QNetworkAccessManager manager;
			QEventLoop loop;
			// qDebug() << "Reading picture form " << url;
			QNetworkReply *reply = manager.get(QNetworkRequest(url));
			//���������������ɺ��˳����¼�ѭ��
			QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
			//�������¼�ѭ��
			loop.exec();
			QByteArray jpegData = reply->readAll();

			pixmap.loadFromData(jpegData);
		}
		//�ı�ͼƬ��С
		pixmap = pixmap.scaled(280, 380, Qt::KeepAspectRatio);
		ui.lbCover->setPixmap(pixmap);
	}
	else {
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("δ�ҵ��鱾"), QMessageBox::Ok);
	}
}

bool student_bookDetail::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj == ui.btnBorrow && event->type() == QEvent::MouseButtonPress) {
		Student student;
		vector<string>VALUES0;
		vector<Student>resStudent;
		VALUES0.push_back("one");
		VALUES0.push_back("id");
		student.setId(userConfig::id);
		FileDB::select("student", student, VALUES0, resStudent);
		if (resStudent[0].money > 50) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("Ƿ��������ޣ����ȹ黹Ƿ��"), QMessageBox::Ok);
			return true;
		}

		//������ļ�¼
		vector<Record>entity;
		Record record;
		record.setStudentId(userConfig::id);
		VALUES0.pop_back();
		VALUES0.push_back("studentId");
		FileDB::select("record", record, VALUES0, entity);
		int sum = 0;
		for (int i = 0; i < entity.size(); i++) {
			if (entity[i].type == 0 || entity[i].type == 3 || entity[i].type == 2 || entity[i].type == 1) {
				sum++;
			}
		}

		if (sum >= 4) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��ǰ�����鼮,��ԤԼ�鼮�Ѵ�����4�������ɼ�������"), QMessageBox::Ok);
			return true;
		}
		vector<string>VALUES;
		vector<BookMap>allBooks;
		BookMap bookmap;
		bookmap.setBookId(bookConfig::bookId);
		bookmap.setIsOut(0);
		VALUES.push_back("one");
		VALUES.push_back("bookId");
		VALUES.push_back("isOut");
		FileDB::select("bookMap", bookmap, VALUES, allBooks);
		if (allBooks.size() > 0) {

			//���¿ɽ豾��
			Book book;
			vector<string>VALUES_3;
			vector<Book>resBook;
			VALUES_3.push_back("one");
			VALUES_3.push_back("id");
			book.setId(bookConfig::bookId);
			FileDB::select("book", book, VALUES_3, resBook);

			Book lbook;
			lbook.setId(bookConfig::bookId);
			resBook[0].setNowCount(resBook[0].nowCount - 1);
			FileDB::update("book", lbook, resBook[0], VALUES_3);

			//����bookMap
			BookMap lbookMap, nbookMap;
			lbookMap.setId(allBooks[0].id);
			nbookMap.setBookId(allBooks[0].bookId);
			nbookMap.setBookNum(allBooks[0].bookNum);
			nbookMap.setIsOut(1);
			FileDB::update("bookMap", lbookMap, nbookMap, VALUES_3);

			//������ļ�¼
			vector<Record>entity;
			Record record;
			record.setStudentId(userConfig::id);
			record.setBookId(allBooks[0].id);

			QDateTime dt = QDateTime::currentDateTime();
			QDateTime afterOneMonthDateTime = dt.addMonths(1);
			QString currentDate = afterOneMonthDateTime.toString("yyyy-MM-dd");

			char* ch1;
			QByteArray ba = currentDate.toLatin1();
			ch1 = ba.data();
			record.setTime(ch1);

			record.setType(0);
			record.setMoney(0);
			entity.push_back(record);
			int res = FileDB::insert("record", entity);
			if (res > 0) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ɹ�"), QMessageBox::Ok);
				student_index *rec = new student_index;
				rec->show();
				this->close();
				return true;
			}
			else {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ʧ�ܣ�δ֪����"), QMessageBox::Ok);
				return true;
			}
		}
		else {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ʧ�ܣ�û�пɽ豾"), QMessageBox::Ok);
			return true;
		}
	}
	if (obj == ui.btnLogout && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::StandardButton button;
		button = QMessageBox::question(this, chartoqs("�˳�����"),
			QString(chartoqs("ȷ���˳�����?")),
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

	if (obj == ui.btnOrder && event->type() == QEvent::MouseButtonPress) {
		Book book;
		vector<string>VALUES_3;
		vector<Book>resBook;
		VALUES_3.push_back("one");
		VALUES_3.push_back("id");
		book.setId(bookConfig::bookId);
		FileDB::select("book", book, VALUES_3, resBook);
		if (resBook[0].nowCount>0) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�������пɽ豾���޷�ԤԼ"), QMessageBox::Ok);
			return true;
		}
		vector<Record>entity0;
		vector<string>VALUES0;
		Record record0;
		record0.setStudentId(userConfig::id);
		VALUES0.push_back("one");
		VALUES0.push_back("studentId");
		FileDB::select("record", record0, VALUES0, entity0);
		int sum = 0;
		for (int i = 0; i < entity0.size(); i++) {
			if (entity0[i].type == 0 || entity0[i].type == 3 || entity0[i].type == 2 ||entity0[i].type == 1 ) {
				sum++;
			}
		}
		if (sum >= 4) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��ǰ�����鼮,��ԤԼ�鼮�Ѵ�����4�������ɼ���ԤԼ"), QMessageBox::Ok);
			return true;
		}
		else {//���Խ���ԤԼ
			Student student;
			vector<string>VALUES;
			vector<Student>resStudent;
			VALUES.push_back("one");
			VALUES.push_back("id");
			student.setId(userConfig::id);
			FileDB::select("student", student, VALUES, resStudent);
			if (resStudent[0].money > 50) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("Ƿ��������ޣ����ȹ黹Ƿ��"), QMessageBox::Ok);
				return true;
			}


			//vector<BookMap>allBooks;
			//searchBookMap(bookConfig::bookId, allBooks);
			//if (allBooks.size() > 0) {

			//	//����bookMap
			//	BookMap lbookMap, nbookMap;
			//	lbookMap.setId(allBooks[0].id);
			//	nbookMap.setBookId(allBooks[0].bookId);
			//	nbookMap.setBookNum(allBooks[0].bookNum);
			//	nbookMap.setIsOut(2);
			//	FileDB::update("bookMap", lbookMap, nbookMap, VALUES_3);

			//������ļ�¼
			vector<Record>entity;
			Record record;
			record.setStudentId(userConfig::id);
			record.setBookId(bookConfig::bookId);

			/*QDateTime dt;
			QTime time;
			QDate date;
			dt.setTime(time.currentTime());
			dt.setDate(date.currentDate());*/

			//��¼ԤԼʱ���Ӧ������ԤԼ��ʱ��
			QDateTime dt = QDateTime::currentDateTime();
			QDateTime afterFiveDaysDateTime = dt.addDays(5);
			QString currentDate = afterFiveDaysDateTime.toString("yyyy-MM-dd");
			//��������ת����Ӧ��������ʱ���¼�ļ�
			char* ch1;
			QByteArray ba = currentDate.toLatin1();
			ch1 = ba.data();
			record.setTime(ch1);

			record.setType(2);
			record.setMoney(0);
			entity.push_back(record);
			int res = FileDB::insert("record", entity);
			if (res > 0) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("ԤԼ�ɹ�"), QMessageBox::Ok);
				student_searchBook *rec = new student_searchBook;
				rec->show();
				this->close();
				return true;
			}
			else {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("ԤԼʧ�ܣ�δ֪����"), QMessageBox::Ok);
				return true;
			}
		}
		return true;
	}

	if (obj == ui.btnInformationchange && event->type() == QEvent::MouseButtonPress) {
		student_update *rec = new student_update;
		rec->show();
		this->close();
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		student_index *rec = new student_index;
		rec->show();
		this->close();
	}
	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		student_searchBook *rec = new student_searchBook;
		rec->show();
		this->close();
	}
	return false;
}

QString student_bookDetail::chartoqs(char *p) {
	string s = p;
	return QString(QString::fromLocal8Bit(s.c_str()));
}

QString student_bookDetail::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string student_bookDetail::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}
