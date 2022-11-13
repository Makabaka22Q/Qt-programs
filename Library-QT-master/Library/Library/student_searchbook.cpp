#include "student_searchbook.h"
#include<string>
#include"book.h"
#include"filedb.h"
#include"student_index.h"
#include"student_update.h"
#include"classifyConfig.h"
#include"qlabel.h"
#include"library.h"
#include"classifyMap.h"
#include"bookConfig.h"
#include"student_bookdetail.h"
#include"student_classify.h"
#include<QSignalMapper>
#include<QMessageBox>
#include <qtnetwork/qnetworkaccessmanager>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkReply>
#include"recommendBuffer.h"
student_searchBook::student_searchBook(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.btnInformationchange->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnFirstPage->installEventFilter(this);
	ui.btnLastPage->installEventFilter(this);
	ui.btnNextPage->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnSearch->installEventFilter(this);
	ui.btnTheLast->installEventFilter(this);
	ui.tableWidget->setColumnCount(7);
	ui.btnLastPage->setEnabled(false);
	ui.btnFirstPage->setEnabled(false);
	ui.btnTheLast->setEnabled(false);
	ui.btnNextPage->setEnabled(false);
	//���ӷ����ź�
	connect(ui.radioClassify, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlot()));
	//��������radio�ź�
	connect(ui.radioName, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlotOther()));
	connect(ui.radioISBN, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlotOther()));
	connect(ui.radioAuthor, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlotOther()));
	QStringList Header;
	Header << QString(u8"����") << QString(u8"����") << QString(u8"����") << QString(u8"������") << QString(u8"�ݲ�����") << QString(u8"�ɽ豾��") << QString(u8"�鿴����");
	ui.tableWidget->setHorizontalHeaderLabels(Header);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableWidget->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableWidget->setFrameShape(QFrame::NoFrame);//�����ޱ߿�
	ui.tableWidget->setShowGrid(false); //���ò���ʾ������
	if (classifyConfig::isCheck) {
		SearchData();
		ui.radioClassify->setChecked(true);
		Classify classify;
		vector<string>VALUES;
		vector<Classify>res;
		VALUES.push_back("one");
		VALUES.push_back("id");
		classify.setId(classifyConfig::classifyId);
		FileDB::select("classify", classify, VALUES, res);
		ui.txtSearch->setText(strtoqs("��ǰ���ࣺ" )+ strtoqs(res[0].name));
		ui.txtSearch->setReadOnly(true);
		classifyConfig::isCheck = 0;
	}
	else {
		ui.radioName->setChecked(true);
	}
	
}

student_searchBook::~student_searchBook()
{

}
//�򿪷���ҳ��
void student_searchBook::radioBtnSlot() {
	if (!classifyConfig::isCheck) {
		classifyConfig::isCheck = 1;
		student_classify *rec = new student_classify;
		this->hide();
		rec->show();
	}
}

//�ָ��ɱ༭
void student_searchBook::radioBtnSlotOther() {
	classifyConfig::isCheck = 0;
	ui.txtSearch->setText("");
	ui.txtSearch->setReadOnly(false);
}
bool student_searchBook::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui.btnLastPage) {
		if (event->type() == QEvent::MouseButtonPress) {
			if (!(PageIndex == 1))
				LastPage();
			return true;
		}
		else {
			return false;
		}
	}

	else if (obj == ui.btnNextPage) {
		if (event->type() == QEvent::MouseButtonPress) {
			if (!(PageIndex == PageCount))
				NextPage();
			return true;
		}
		else {
			return false;
		}
	}

	else if (obj == ui.btnSearch) {
		if (event->type() == QEvent::MouseButtonPress) {
			SearchData();
			return true;
		}
		else {
			return false;
		}
	}

	else if (obj == ui.btnFirstPage) {
		if (event->type() == QEvent::MouseButtonPress) {
			FirstPage();
			return true;
		}
		else {
			return false;
		}
	}

	else if (obj == ui.btnTheLast) {
		if (event->type() == QEvent::MouseButtonPress) {
			TheLastPage();
			return true;
		}
		else {
			return false;
		}
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

	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		student_index *rec = new student_index;
		this->close();
		rec->show();

	}
	if (obj == ui.btnInformationchange && event->type() == QEvent::MouseButtonPress) {
		student_update *rec = new student_update;
		this->close();
		rec->show();

	}

	else {
		return QWidget::eventFilter(obj, event);
	}

	return false;
}

void student_searchBook::FirstPage() {
	PageIndex = 1;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void student_searchBook::TheLastPage() {
	PageIndex = PageCount;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void student_searchBook::SearchData() {
	if (classifyConfig::isCheck) {
		//���ݷ����ѯbookID
		ClassifyMap classifymap;
		classifymap.setClassifyId(classifyConfig::classifyId);
		vector<ClassifyMap>resClassifyMap;
		vector<string> VALUES;
		VALUES.push_back("one");
		VALUES.push_back("classifyId");
		FileDB::select("classifyMap", classifymap, VALUES, resClassifyMap);
		//����bookid��ѯ�鼮
		Book book;
		VALUES.pop_back();
		VALUES.push_back("id");
		for (int i = 0; i < resClassifyMap.size(); i++) {
			book.setId(resClassifyMap[i].bookId);
			FileDB::select("book", book, VALUES, DataTable);
		}
	}
	else {

		QString str = ui.txtSearch->toPlainText();
		QByteArray ba = str.toLocal8Bit();
		char *name = ba.data();
		DataTable.clear();
		//ʲôҲ�����룬��ѯȫ��ͼ��
		if (str.length() == 0) {
			Book book;
			vector<string>VALUES;
			VALUES.push_back("all");
			FileDB::select("book", book, VALUES, DataTable);
		}
		else {
			//����������ѯ
			if (ui.radioName->isChecked())
				FileDB::selectLike("book", "name", name, DataTable);
			//�������߲�ѯ
			else if (ui.radioAuthor->isChecked())
				FileDB::selectLike("book", "author", name, DataTable);
			//����ISBN��ѯ
			else if (ui.radioISBN->isChecked())
				FileDB::selectLike("book", "ISBN", name, DataTable);
		}
	}

	PageIndex = 1;
	PageCount = ceill(DataTable.size() / (double)PageLength);
	DataBind();
	/*ui.btnGo->setEnabled(true);*/
	ui.btnLastPage->setEnabled(false);
	if (PageIndex<PageCount)
		ui.btnNextPage->setEnabled(true);
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	ui.etCountPage->setText(QString::number(PageCount, 10));
	ui.btnFirstPage->setEnabled(true);
	ui.btnTheLast->setEnabled(true);
}

void student_searchBook::DataBind() {
	for (int i = ui.tableWidget->rowCount(); i > 0; i--) {
		ui.tableWidget->removeRow(0);
	}
	int currentPageBegin = (PageIndex - 1)*PageLength;
	int currentPageEnd = (currentPageBegin + PageLength) < DataTable.size() ? (currentPageBegin + PageLength) : DataTable.size();
	for (int i = currentPageBegin; i < currentPageEnd; i++) {
		ui.tableWidget->insertRow(i - currentPageBegin);
		ui.tableWidget->setRowHeight(i - currentPageBegin, 200);//��һ��
		//����ͼƬ
		QPixmap pixmap;
		if (strstr(DataTable[i].cover, "images")) {
			pixmap.load(QString::fromLocal8Bit(DataTable[i].cover));
		}
		else {
			QUrl url(DataTable[i].cover);
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
		pixmap = pixmap.scaled(110, 130, Qt::KeepAspectRatio);
		QLabel *label = new QLabel;
		label->setPixmap(pixmap);
		ui.tableWidget->setCellWidget(i - currentPageBegin, 0, label); 
	
		
		//�����������߳�����
		//����������ʹ�����
		QTableWidgetItem *temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].name));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 1, temp);
		//�������߲�ʹ�����
		temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].author));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 2, temp);
		//��������粢ʹ�����
		temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].publish));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 3, temp);
		//�����ܿ�沢ʹ�����
		temp = new QTableWidgetItem(QString::number(DataTable[i].count, 10));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 4, temp);
		//����ɽ豾����ʹ�����
		temp = new QTableWidgetItem(QString::number(DataTable[i].nowCount, 10));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 5, temp);
		//��ӡ����顱��ť�������¼�
		QPushButton *btn = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 6, btn);
		btn->setText(strtoqs("����"));
		btn->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(btn, DataTable[i].id);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnClicked(int)));
	}
}

void student_searchBook::NextPage() {
	if (PageIndex >= PageCount)return;
	if (PageIndex == 1)
		ui.btnLastPage->setEnabled(true);
	PageIndex++;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void student_searchBook::LastPage() {
	if (PageIndex <= 1)return;
	if (PageIndex == PageCount)
		ui.btnNextPage->setEnabled(true);
	PageIndex--;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}


QString student_searchBook::strtoqs(const string &s)

{

	return QString(QString::fromLocal8Bit(s.c_str()));

}

string student_searchBook::qstostr(const QString &s)

{

	return string((const char*)s.toLocal8Bit());

}

void student_searchBook::addItemContent(int row, int column, QString content)
{
	QTableWidgetItem *item = new QTableWidgetItem(content);
	ui.tableWidget->setItem(row, column, item);
}

void student_searchBook::OnBtnClicked(int id)
{
	bookConfig::bookId = id;
	student_bookDetail *rec = new student_bookDetail;
	rec->show();
	this->close();
}