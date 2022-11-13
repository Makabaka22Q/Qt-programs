#include "student_borrowdetail.h"
#include"student_index.h"
#include"bookConfig.h"
#include"book.h"
#include"filedb.h"
#include"classify.h"
#include"library.h"
#include<vector>
#include<string>
#include<QMessageBox>
#include <QTextCodec>
#include<QDateTime>
#include"record.h"
#include"student.h"
#include"userConfig.h"
#include"bookConfig.h"
#include"student_update.h"
#include"student_searchbook.h"
#include"bookMap.h"
#include <qtnetwork/qnetworkaccessmanager>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkReply>
#include"recommendBuffer.h"
using namespace std;

student_borrowdetail::student_borrowdetail(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.etBookNameHead->setEnabled(false);
	ui.etBookName->setEnabled(false);
	ui.etAuthor->setEnabled(false);
	ui.etPublish->setEnabled(false);
	ui.etISBN->setEnabled(false);
	ui.etClassify->setEnabled(false);
	ui.etBookNameHead->setEnabled(false);
	ui.etBookName->setEnabled(false);
	ui.etAuthorHead->setEnabled(false);
	ui.etPublishHead->setEnabled(false);
	ui.etISBNHead->setEnabled(false);
	ui.btnLogout->installEventFilter(this);
	ui.etClassifyHead->setEnabled(false);
	ui.btnPersonal->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnInformationchange->installEventFilter(this);
	ui.btnBorrowagain->installEventFilter(this);
	ui.btnReturnbook->installEventFilter(this);
	InitThisPage();
}

student_borrowdetail::~student_borrowdetail()
{

}

void student_borrowdetail::InitThisPage() {
	Book book;
	BookMap bookmap;
	Record record;
	vector<string>VALUES;
	vector<Book>resBook;
	vector<BookMap>resBookMap;
	vector<Record>resRecord;
	VALUES.push_back("one");
	VALUES.push_back("id");

	bookmap.setId(bookConfig::bookNo);
	FileDB::select("bookMap", bookmap, VALUES, resBookMap);
	book.setId(resBookMap[0].bookId);

	FileDB::select("book", book, VALUES, resBook);

	vector<string>VALUES_2;
	VALUES_2.push_back("one");
	VALUES_2.push_back("bookId");
	record.setBookId(bookConfig::bookNo);
	FileDB::select("record", record, VALUES_2, resRecord);
	int i;
	for (i = 0; i < resRecord.size(); i++) {
		if (resRecord[i].type != 2 && resRecord[i].type != 4) {
			break;
		}
	}

	ui.etDate->setText(chartoqs(resRecord[i].time));
	ui.etBookNameHead->setText(chartoqs(resBook[0].name));
	ui.etBookName->setText(chartoqs(resBook[0].name));
	ui.etAuthor->setText(chartoqs(resBook[0].author));
	ui.etPublish->setText(chartoqs(resBook[0].publish));
	ui.etISBN->setText(chartoqs(resBook[0].ISBN));
	ui.etDate->setText(chartoqs(resRecord[i].time));

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
	ui.lbCover->setPixmap(pixmap);
}

bool student_borrowdetail::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj == ui.btnReturnbook && event->type() == QEvent::MouseButtonPress) {
		Record record;
		vector<string>VALUES;
		vector<Record>res;
		vector<Record>resTemp;
		VALUES.push_back("one");
		VALUES.push_back("bookId");
		record.setBookId(bookConfig::bookNo);
		FileDB::select("record", record, VALUES, resTemp);

		for (int i = 0; i < resTemp.size(); i++) {
			if (resTemp[i].type != 2 && resTemp[i].type != 4)
				res.push_back(resTemp[i]);
		}

		VALUES.pop_back();
		VALUES.push_back("id");
		vector<BookMap>allBooks;
		BookMap bookmap;
		bookmap.setId(bookConfig::bookNo);
		FileDB::select("bookMap", bookmap, VALUES, allBooks);


		//���Ȳ�ѯ�Ƿ�����ԤԼ������
		Record iforder;
		vector<string>VALUES_2;
		vector<Record>resiforder;
		VALUES_2.push_back("one");
		VALUES_2.push_back("bookId");
		VALUES_2.push_back("type");
		iforder.setBookId(allBooks[0].bookId);
		iforder.setType(2);
		FileDB::select("record", iforder, VALUES_2, resiforder);

		if (resiforder.size() == 0) {
			//���¿ɽ豾��
			Book book;
			vector<Book>resBook;
			book.setId(allBooks[0].bookId);
			FileDB::select("book", book, VALUES, resBook);

			Book lbook;
			lbook.setId(allBooks[0].bookId);
			resBook[0].setNowCount(resBook[0].nowCount + 1);
			FileDB::update("book", lbook, resBook[0], VALUES);


			//����bookMap
			BookMap lbookMap, nbookMap;
			lbookMap.setId(allBooks[0].id);
			nbookMap.setBookId(allBooks[0].bookId);
			nbookMap.setBookNum(allBooks[0].bookNum);
			nbookMap.setIsOut(0);
			FileDB::update("bookMap", lbookMap, nbookMap, VALUES);


			Record record_delete;
			vector<string>VALUES_2;
			VALUES_2.push_back("one");
			VALUES_2.push_back("id");
			record_delete.setId(res[0].id);
			res[0].setType(4);
			//���½��ļ�¼type=4,��ʾ�ѹ黹
			int res_2 = FileDB::update("record", record_delete, res[0], VALUES_2);
			if (res_2 > 0) {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ɹ�"), QMessageBox::Ok);
				student_index *rec = new student_index;
				rec->show();
				this->close();
				return true;
			}
			else {
				QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("δ֪����"), QMessageBox::Ok);
				return true;
			}
		}
		else {//����ԤԼ��
			  //ԤԼ��������˻��飬�Զ�����ʱ��˳����顣��Ĩ��ԤԼ�������޸Ļ��麯����
			  //ԤԼ����Ϊ5�죬���ں��Զ�Ĩ��ԤԼ��¼���޸Ļ��麯����ͳ��Ƿ��ĺ�����
			ReturnOrder();//�������ʱ��⵽��ԤԼ����ô˺���
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
	if (obj == ui.btnBorrowagain && event->type() == QEvent::MouseButtonPress) {

		//�����������
		Record record;
		vector<Record>resRecord;
		vector<string>VALUES_2;
		VALUES_2.push_back("one");
		VALUES_2.push_back("bookId");
		record.setBookId(bookConfig::bookNo);
		FileDB::select("record", record, VALUES_2, resRecord);
		//����Ƿ���
		int i;
		for (i = 0; i < resRecord.size(); i++) {
			if (resRecord[i].type != 4) {
				break;
			}
		}
		if (resRecord[i].type == 1) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("���ڣ���������"), QMessageBox::Ok);
			return true;
		}
		//����Ƿ��Ѿ������
		if (resRecord[i].type == 3) {
			QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�Ѿ�������������ٴ�����"), QMessageBox::Ok);
			return true;
		}
		QDateTime dt = QDateTime::fromString(resRecord[i].time, "yyyy-MM-dd");
		QDateTime afterOneMonthDateTime = dt.addMonths(1);
		QString currentDate = afterOneMonthDateTime.toString("yyyy-MM-dd");
		char* ch1;
		QByteArray ba = currentDate.toLatin1();
		ch1 = ba.data();
		resRecord[i].setTime(ch1);
		record.setType(resRecord[i].type);
		resRecord[i].setType(3);
		VALUES_2.push_back("type");
		ui.etDate->setText(chartoqs(resRecord[i].time));
		FileDB::update("record", record, resRecord[i], VALUES_2);
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ɹ�"), QMessageBox::Ok);
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

void student_borrowdetail::ReturnOrder() {
	int theFirstOrderId = 0;//��¼�ºϷ�ԤԼ��¼������ԤԼ��¼��id
	int min_time = 0;
	//��ȡ������5���ԤԼ��¼
	//��ӳ���bookMap�в�ѯ�����id(��ʱʹ����ԭʼ�Ĳ��ҷ�ʽ)
	BookMap bookmap;
	vector<string>VALUES;
	vector<BookMap>resBookMap;
	VALUES.push_back("one");
	VALUES.push_back("id");
	bookmap.setId(bookConfig::bookNo);
	FileDB::select("bookMap", bookmap, VALUES, resBookMap);

	//���˻���ʱ����������id��type == 2����ѯ��ԤԼ������ɾ����¼��Ȼ��������
	Record record;
	VALUES.clear();
	vector<Record>resRecord;
	VALUES.push_back("one");
	VALUES.push_back("bookId");
	VALUES.push_back("type");
	record.setBookId(resBookMap[0].bookId);
	record.setType(2);
	FileDB::select("record", record, VALUES, resRecord);
	//����ʱ��,��ɾ�����ڵ�ԤԼ��¼
	QDateTime now = QDateTime::currentDateTime();
	for (int i = 0; i < resRecord.size(); i++) {
		QDateTime then = QDateTime::fromString(resRecord[i].time, "yyyy-MM-dd");
		int span = then.secsTo(now);
		if (span > 0) {
			if (resRecord[i].type == 2) {
				Record rec;
				VALUES.clear();
				VALUES.push_back("one");
				VALUES.push_back("id");
				rec.setId(resRecord[i].id);
				FileDB::Delete("record", rec, VALUES);
			}
		}
		else {//��¼�ºϷ�ԤԼ��¼������ԤԼ��¼
			if (span < min_time) {
				min_time = span;
				theFirstOrderId = i;
			}
		}
	}

	//ϵͳ�Զ�������ԤԼ��ͬѧ����
	//��������¼
	vector<Record>entity;
	Record record_2;
	record_2.setStudentId(resRecord[theFirstOrderId].studentId);
	record_2.setBookId(resBookMap[0].id);

	QDateTime dt = QDateTime::currentDateTime();
	QDateTime afterOneMonthDateTime = dt.addMonths(1);
	QString currentDate = afterOneMonthDateTime.toString("yyyy-MM-dd");

	char* ch1;
	QByteArray ba = currentDate.toLatin1();
	ch1 = ba.data();
	record_2.setTime(ch1);

	record_2.setType(0);
	record_2.setMoney(0);
	entity.push_back(record_2);
	FileDB::insert("record", entity);
	//ɾ��ԤԼ��¼
	Record deletethis;
	VALUES.clear();
	VALUES.push_back("one");
	VALUES.push_back("id");
	deletethis.setId(resRecord[theFirstOrderId].id);
	FileDB::Delete("record", deletethis, VALUES);

	//ɾ�������¼
	Record record_3;
	VALUES.clear();
	vector<Record>res;
	VALUES.push_back("one");
	VALUES.push_back("bookId");
	record_3.setBookId(bookConfig::bookNo);
	FileDB::select("record", record_3, VALUES, res);
	Record record_delete;
	vector<string>VALUES_2;
	VALUES_2.push_back("one");
	VALUES_2.push_back("id");
	int i;
	for (i = 0; i < res.size(); i++) {
		//����BUG
		if (res[i].type != 4 && res[i].type != 2) {
			break;
		}
	}
	record_delete.setId(res[i].id);
	res[i].setType(4);
	int res_2 = FileDB::update("record", record_delete,res[i],VALUES_2);
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (res_2 > 0) {
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ɹ�"), QMessageBox::Ok);
		student_index *rec = new student_index;
		rec->show();
		this->close();
	}
	else {
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("δ֪����"), QMessageBox::Ok);
	}
}

QString student_borrowdetail::chartoqs(char *p) {
	string s = p;
	return QString(QString::fromLocal8Bit(s.c_str()));
}

QString student_borrowdetail::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string student_borrowdetail::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}