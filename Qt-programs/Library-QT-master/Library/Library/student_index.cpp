#include "student_index.h"
#include<string>
#include<iostream>
#include<QDateTime>
#include<QMessageBox>
#include <QTextCodec> 
#include "qlabel.h"
#include"filedb.h"
#include"library.h"
#include"book.h"
#include<vector>
#include"userConfig.h"
#include"student.h"
#include"record.h"
#include"userConfig.h"
#include"student_borrow.h"
#include"student_searchbook.h"
#include"student_bookdetail.h"
#include"bookConfig.h"
#include"student_update.h"
#include"bookMap.h"
#include"student_borrowdetail.h"
#include"record.h"
#include"classifyMap.h"
#include"classify.h"
#include"recommendBuffer.h"
#include<map>
#include<QSignalMapper>
#include <QUrl>
#include <QCloseEvent>
#include <qtnetwork/qnetworkaccessmanager>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkReply>
#include"recommendBuffer.h"
using namespace std;	

student_index::student_index(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.tableOrder->setColumnCount(5);
	ui.lineEdit->setEnabled(false);
	ui.lineEdit_2->setEnabled(false);
	ui.etSnumber->setText(userConfig::username);
	ui.etSname->setEnabled(false);
	ui.etDebt->setEnabled(false);
	ui.etSdept->setEnabled(false);
	ui.etSnumber->setEnabled(false);
	ui.etBorrownumber->setEnabled(false);
	ui.btnPersonal->installEventFilter(this);
	ui.btnInformationchange->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnBorrowmore->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.tableBorrow->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableBorrow->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableBorrow->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableBorrow->setFrameShape(QFrame::NoFrame);//�����ޱ߿�
	ui.tableBorrow->setShowGrid(false); //���ò���ʾ������

	ui.tableOrder->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableOrder->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableOrder->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableOrder->setFrameShape(QFrame::NoFrame);//�����ޱ߿�
	ui.tableOrder->setShowGrid(false); //���ò���ʾ������
	ui.tableOrder->horizontalHeader()->setVisible(false);// ˮƽ���ɼ� 
	ui.tableOrder->insertRow(0);
	ui.tableOrder->setRowHeight(0, 150);
	InitThisPage();
	

	if (!recommendBuffer::isPostBack) {
		myThread = new Thread;
		connect(myThread, SIGNAL(UpdateSignal()),
			this, SLOT(UpdateSlot()));
		myThread->start();
	}
	else {
		InitRecommendPic();
	}
}

student_index::~student_index()
{

}

bool student_index::eventFilter(QObject *obj, QEvent *event) {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	if (obj ==ui.btnBorrowmore && event->type() == QEvent::MouseButtonPress) {
		student_borrow *rec = new student_borrow;
		rec->show();
		this->close();
	}
	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		student_searchBook *rec = new student_searchBook;
		this->close();
		rec->show();
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
	if (obj == ui.btnInformationchange && event->type() == QEvent::MouseButtonPress) {
		student_update *rec = new student_update;
		this->close();
		rec->show();
	}
	return false;
}

QString student_index::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string student_index::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}

void student_index::addItemContent(int row, int column, QString content)
{
	QTableWidgetItem *item = new QTableWidgetItem(content);
	ui.tableBorrow->setItem(row, column, item);
}

void student_index::addItemContentOrder(int row, int column, QString content)
{
	QTableWidgetItem *item = new QTableWidgetItem(content);
	ui.tableOrder->setItem(row, column, item);
}

void student_index::InitThisPage() {
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");

	//��ѯ���ѧ���Ļ�����Ϣ��������ҳ��
	Student student;
	vector<string>VALUES;
	vector<Student>resStudent;
	VALUES.push_back("one");
	VALUES.push_back("id");
	student.setId(userConfig::id);
	FileDB::select("student", student, VALUES, resStudent);
	ui.etSname->setText(BianMa->toUnicode(resStudent[0].username));
	ui.etSdept->setText(BianMa->toUnicode(resStudent[0].dept));
	//����ͷ��
	//�ӱ��ؼ���
	if (strstr(resStudent[0].icon, "images")) {
		recommendBuffer::headUrlLocal = resStudent[0].icon;
	}
	else {
		QUrl url(resStudent[0].icon);
		recommendBuffer::headUrl = url;
	}
	/*
	 *���¸��½��ı�
	*/
	//��ѯ���ѧ���Ľ����¼
	Record record;
	vector<Record>resRecord;
	vector<Record>resRecordNormal;
	vector<Record>resRecordExceed;
	vector<Record>resRecordOrder;
	VALUES.pop_back();
	VALUES.push_back("studentId");
	record.setStudentId(resStudent[0].id);
	FileDB::select("record", record, VALUES, resRecord);

	QDateTime now = QDateTime::currentDateTime();
	for (int i = 0; i < resRecord.size(); i++) {
		//����
		if (resRecord[i].type == 0||resRecord[i].type == 3) resRecordNormal.push_back(resRecord[i]);
		//����
		if (resRecord[i].type == 1) resRecordExceed.push_back(resRecord[i]);
		//ԤԼ
		if (resRecord[i].type == 2) {
			//ɾ�����ڵ�ԤԼ��¼
			QDateTime then = QDateTime::fromString(resRecord[i].time, "yyyy-MM-dd");
			int span = then.secsTo(now);
			if (span > 0) {
				Record rec;
				VALUES.pop_back();
				VALUES.push_back("id");
				rec.setId(resRecord[i].id);
				FileDB::Delete("record", rec, VALUES);
			}
			//û�г��ڣ����ؽ���
			else {
				resRecordOrder.push_back(resRecord[i]);
			}
		}
	}
	//������Ϣ
	resRecord.clear();
	for (int i = 0; i < resRecordNormal.size(); i++) {
		resRecord.push_back(resRecordNormal[i]);
	}
	for (int i = 0; i < resRecordExceed.size(); i++) {
		resRecord.push_back(resRecordExceed[i]);
	}
	for (int i = 0; i < resRecordOrder.size(); i++) {
		resRecord.push_back(resRecordOrder[i]);
	}
	

	//ÿ���ĵ�ÿһ������ص������
	string borrowNumber = "(";
	char c[20];
	itoa(resRecordNormal.size()+ resRecordExceed.size()+ resRecordOrder.size(), c, 10);
	borrowNumber += c;
	borrowNumber += ")";
	ui.etBorrownumber->setText(strtoqs(borrowNumber));
	QStringList header;
	header << strtoqs("����") << strtoqs("����") << strtoqs("������") << strtoqs("״̬") << strtoqs("�鿴����");
	ui.tableBorrow->setHorizontalHeaderLabels(header);
	ui.tableBorrow->horizontalHeader()->setStretchLastSection(true);
	ui.tableBorrow->setEditTriggers(QAbstractItemView::NoEditTriggers);
	int sum_row = ui.tableBorrow->rowCount();
	ui.tableBorrow->removeRow(sum_row);


	Book book;
	BookMap bookmap;
	vector<Book>resBook;
	vector<BookMap>resBookMap;
	//���м��ص����ı���

	//��һ�μ���ʱ���Ķ���
	float money = 0;
	vector<string>VALBOOK;
	VALBOOK.push_back("one");
	VALBOOK.push_back("id");
	//���ؽ��ĵ�δ���ڵ���Ŀ��ע���ʱ������һ���ֱ�Ϊ���ڣ�
	for (int i = 0; i < resRecord.size(); i++) {
		//����Ƿ���
		if (resRecord[i].type == 0 || resRecord[i].type == 3) {
			QDateTime then = QDateTime::fromString(resRecord[i].time, "yyyy-MM-dd");
			int span = then.secsTo(now);
			if (span > 0) {
				Record rec;
				rec.setId(resRecord[i].id);
				int day = span / (60 * 60 * 24);
				money += day;
				resRecord[i].type = 1;
				FileDB::update("record", rec, resRecord[i], VALBOOK);
			}
		}
		VALUES.clear();
		VALUES.push_back("one");
		VALUES.push_back("id");
		resBookMap.clear();
		resBook.clear();
		//��ԤԼ��
		if (resRecord[i].type != 2) {
			//��ѯbookMap����bookId
			bookmap.setId(resRecord[i].bookId);
			FileDB::select("bookMap", bookmap, VALUES, resBookMap);
			//��ѯbook��������
			book.setId(resBookMap[0].bookId);
			FileDB::select("book", book, VALUES, resBook);
		}
		//ԤԼ��
		else {
			//��ѯbook��������
			book.setId(resRecord[i].bookId);
			FileDB::select("book", book, VALUES, resBook);
		}
		int row = ui.tableBorrow->rowCount();
		ui.tableBorrow->insertRow(i);
		
		addItemContent(i, 0, chartoqs(resBook[0].name));
		addItemContent(i, 1, chartoqs(resBook[0].author));
		addItemContent(i, 2, chartoqs(resBook[0].publish));
		if(resRecord[i].type==0 || resRecord[i].type == 3)
			addItemContent(i, 3, chartoqs("����"));
		else if(resRecord[i].type == 1)
			addItemContent(i, 3, chartoqs("����"));
		else
			addItemContent(i, 3, chartoqs("ԤԼ"));
		//��ӡ����顱��ť�������¼�
		QPushButton *btn = new QPushButton;
		ui.tableBorrow->setCellWidget(i, 4, btn);
		btn->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);

		if (resRecord[i].type != 2) {
			btn->setText(strtoqs("����"));
			QSignalMapper* signalMapper = new QSignalMapper(this);
			connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
			signalMapper->setMapping(btn, resRecord[i].bookId);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnClicked(int)));
		}
		else {
			btn->setText(strtoqs("ȡ��ԤԼ"));
			QSignalMapper* signalMapper = new QSignalMapper(this);
			connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
			signalMapper->setMapping(btn, resRecord[i].id);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnClickedCancel(int)));
		}
	}
	
	if (!recommendBuffer::isPostBack) {
		//����Ƿ�ѽ��
		if (money > resStudent[0].money) {
			resStudent[0].setMoney(money);
			student.setId(resStudent[0].id);
			FileDB::update("student", student, resStudent[0], VALUES);
		}
	}
	QString total = QString("%1").arg(resStudent[0].money);
	ui.etDebt->setText(chartoqs("Ƿ���") + total + chartoqs("Ԫ"));



	vector<string>VALUES_2;
	//�����Ƽ���Ŀ
	int maxClassifyId = 0;
	if (!recommendBuffer::isPostBack) {
		map<int, int>classifyMap;
		Classify classify;
		ClassifyMap classifymap;
		vector<ClassifyMap>resClMap;
		resBook.clear();
		resBookMap.clear();
		VALUES.clear();
		VALUES.push_back("one");
		VALUES.push_back("id");
		VALUES_2.push_back("one");
		VALUES_2.push_back("bookId");
		for (int i = 0; i < resRecord.size(); i++) {
			resBook.clear();
			resBookMap.clear();
			bookmap.setId(resRecord[i].bookId);
			FileDB::select("bookMap", bookmap, VALUES, resBookMap);
			classifymap.setBookId(resBookMap[0].bookId);
			FileDB::select("classifyMap", classifymap, VALUES_2, resClMap);
			int id = resClMap[0].classifyId;
			classifyMap[id]++;
			if (classifyMap[id] > classifyMap[maxClassifyId]) maxClassifyId = id;
		}

		//������ļ�¼Ϊ0��������Ƽ�
		if (resRecord.size() == 0) {
			srand((unsigned)time(NULL));
			vector<string>VALUES_3;
			VALUES_3.push_back("all");
			Classify classify;
			vector<Classify>resClssify;
			FileDB::select("classify", classify, VALUES_3, resClssify);
			int a = 0;
			int b = resClssify.size() - 1;
			int No = (rand() % (b - a + 1)) + a;
			maxClassifyId = resClssify[No].id;
		}

		VALUES.pop_back();
		VALUES.push_back("classifyId");
		book.setClassifyId(maxClassifyId);
		resBook.clear();
		FileDB::select("book", book, VALUES, resBook);

		for (int i = 0; i < 6 && i < resBook.size(); i++) {
			if (!strstr(resBook[i].cover, "images")) {
				recommendBuffer::urlBuffer.push_back(QString::fromUtf8(resBook[i].cover));
			}
			else {
				recommendBuffer::urlBufferLocal.push_back(QString::fromLocal8Bit(resBook[i].cover));
			}
			
			recommendBuffer::idBuffer.push_back(resBook[i].id);
		}
	}

}
void student_index::UpdateSlot() {
	InitRecommendPic();
}

void student_index::InitRecommendPic() {
	ui.lbHead->setPixmap(recommendBuffer::headBuffer[0]);
	for (int i = 0; i < recommendBuffer::picBuffer.size(); i++) {
		QPixmap pixmap = recommendBuffer::picBuffer[i];
		QPushButton *pBtn = new QPushButton;
		pBtn->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		pBtn->setIcon(pixmap);
		pBtn->setIconSize(QSize(pixmap.width(), pixmap.height()));
		ui.tableOrder->setCellWidget(0, i, pBtn);
		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(pBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(pBtn, recommendBuffer::idBuffer[i]);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnRecommendClicked(int)));
	}
	if (!recommendBuffer::isPostBack) {
		recommendBuffer::isPostBack = true;
	}
}


void student_index::ClickButton() {
}

void student_index::OnBtnClickedCancel(int id){
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	Record record;
	vector<string>VALUES;
	VALUES.push_back("one");
	VALUES.push_back("id");
	record.setId(id);
	int res = FileDB::Delete("record", record, VALUES);
	if (res > 0) {
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("ȡ���ɹ�"), QMessageBox::Ok);
		student_index *rec = new student_index;
		this->close();
		rec->show();
	}
	
}
void student_index::OnBtnRecommendClicked(int id){
	bookConfig::bookId = id;
	student_bookDetail *rec = new student_bookDetail;
	rec->showFullScreen();
	this->close();
}
void student_index::OnBtnClicked(int id)
{
	bookConfig::bookNo = id;
	student_borrowdetail *rec = new student_borrowdetail;
	rec->showFullScreen();
	this->close();
}

QString student_index::chartoqs(char *p) {
	string s = p;
	return QString(QString::fromLocal8Bit(s.c_str()));
}