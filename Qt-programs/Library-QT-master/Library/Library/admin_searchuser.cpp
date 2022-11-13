#include "admin_searchuser.h"
#include "admin_index.h"
#include"admin_searchbook.h"
#include"admin_searchuser.h"
#include"admin_classify.h"
#include"admin_studentclassify.h" 
#include"studentClassifyConfig.h"
#include"admin_adduser.h"
#include"classifyConfig.h"
#include"book.h"
#include"classify.h"
#include"classifyMap.h"
#include"admin_classifyshow.h"
#include"filedb.h"
#include"record.h"
#include"library.h"
#include<QSignalMapper>
#include<QMessageBox>
#include <QTextCodec>
admin_searchuser::admin_searchuser(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.lineEdit->setEnabled(false);
	ui.lineEdit_2->setEnabled(false);
	ui.lineEdit_4->setEnabled(false);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnClassify->installEventFilter(this);
	ui.btnAdd->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnLastPage->installEventFilter(this);
	ui.btnFirstPage->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnLastPage->installEventFilter(this);
	ui.btnNextPage->installEventFilter(this);
	ui.btnSearch->installEventFilter(this);
	ui.btnTheLast->installEventFilter(this);
	ui.tableWidget->setColumnCount(7);
	ui.btnLastPage->setEnabled(false);
	ui.btnFirstPage->setEnabled(false);
	//���ӷ����ź�
	connect(ui.radioMajor, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlot()));
	//��������radio�ź�
	connect(ui.radioName, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlotOther()));
	connect(ui.radioCode, SIGNAL(clicked(bool)), this, SLOT(radioBtnSlotOther()));

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableWidget->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableWidget->setFrameShape(QFrame::NoFrame);//�����ޱ߿�
	ui.tableWidget->setShowGrid(false); //���ò���ʾ������

	if (studentClassifyConfig::isCheck) {
		SearchData();
		ui.radioMajor->setChecked(true);
		ui.txtSearch->setText(QString::fromLocal8Bit("��ǰרҵ��") + QString::fromLocal8Bit(studentClassifyConfig::classifyName));
		ui.txtSearch->setReadOnly(true);
		classifyConfig::isCheck = 0;
	}
	else {
		ui.radioName->setChecked(true);
	}
}

admin_searchuser::~admin_searchuser()
{

}

bool admin_searchuser::eventFilter(QObject *obj, QEvent *event)
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
			Library *rec = new Library;
			this->close();
			rec->show();
			event->accept();  //�����˳��źţ������˳�
		}
	}

	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		admin_searchbook *rec = new admin_searchbook;
		this->close();
		rec->show();
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		admin_index *rec = new admin_index;
		this->close();
		rec->show();
	}
	if (obj == ui.btnAdd && event->type() == QEvent::MouseButtonPress) {
		admin_adduser *rec = new admin_adduser;
		this->close();
		rec->show();
	}
	if (obj == ui.btnClassify && event->type() == QEvent::MouseButtonPress) {
		admin_classify *rec = new admin_classify;
		this->close();
		rec->show();
	}

	else {
		return QWidget::eventFilter(obj, event);
	}

	return false;
}
void admin_searchuser::FirstPage() {
	PageIndex = 1;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_searchuser::TheLastPage() {
	PageIndex = PageCount;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_searchuser::SearchData() {
	if (studentClassifyConfig::isCheck) {
		//����רҵ��ѯѧ��
		vector<string>VALUES;
		Student student;
		VALUES.push_back("one");
		VALUES.push_back("dept");
		student.setDept(studentClassifyConfig::classifyName);
		FileDB::select("student", student, VALUES, DataTable);
		studentClassifyConfig::isCheck = 0;
	}
	else {

		QString str = ui.txtSearch->toPlainText();
		QByteArray ba = str.toLocal8Bit();
		char *name = ba.data();
		DataTable.clear();
		//ʲôҲ�����룬��ѯȫ��ͼ��
		if (str.length() == 0) {
			Student book;
			vector<string>VALUES;
			VALUES.push_back("all");
			FileDB::select("student", book, VALUES, DataTable);
		}
		else {
			//����������ѯ
			if (ui.radioName->isChecked())
				FileDB::selectLike("student", "username", name, DataTable);
			//�������߲�ѯ
			else if (ui.radioCode->isChecked())
				FileDB::selectLike("student", "usercode", name, DataTable);
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

void admin_searchuser::DataBind() {
	for (int i = ui.tableWidget->rowCount(); i > 0; i--) {
		ui.tableWidget->removeRow(0);
	}
	int currentPageBegin = (PageIndex - 1)*PageLength;
	int currentPageEnd = (currentPageBegin + PageLength) < DataTable.size() ? (currentPageBegin + PageLength) : DataTable.size();
	for (int i = currentPageBegin; i < currentPageEnd; i++) {
		ui.tableWidget->insertRow(i - currentPageBegin);
		ui.tableWidget->setRowHeight(i - currentPageBegin, 50);
		//�����û�������ʹ�����
		QTableWidgetItem *temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].username));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 0, temp);
		//�����û�ѧ�Ų�ʹ�����
		temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].usercode));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 1, temp);
		//�����û�רҵ��ʹ�����
		temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].dept));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 2, temp);
		//��ӡ����顱��ť�������¼�
		QPushButton *btn = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 3, btn);
		btn->setText(QString::fromLocal8Bit("�޸�"));
		btn->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(btn, DataTable[i].id);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnClickedChange(int)));

		QPushButton *btn3 = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 4, btn3);
		btn3->setText(QString::fromLocal8Bit("��������"));
		btn3->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		QSignalMapper* signalMapper3 = new QSignalMapper(this);
		connect(btn3, SIGNAL(clicked()), signalMapper3, SLOT(map()));
		signalMapper3->setMapping(btn3, DataTable[i].id);
		connect(signalMapper3, SIGNAL(mapped(int)), this, SLOT(OnBtnClickedReset(int)));

		QPushButton *btn4 = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 5, btn4);
		btn4->setText(QString::fromLocal8Bit("���Ƿ��"));
		btn4->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		QSignalMapper* signalMapper4 = new QSignalMapper(this);
		connect(btn4, SIGNAL(clicked()), signalMapper4, SLOT(map()));
		signalMapper4->setMapping(btn4, DataTable[i].id);
		connect(signalMapper4, SIGNAL(mapped(int)), this, SLOT(OnBtnClickedClear(int)));

		QPushButton *btn2 = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 6, btn2);
		btn2->setText(QString::fromLocal8Bit("ɾ��"));
		btn2->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		QSignalMapper* signalMapper2 = new QSignalMapper(this);
		connect(btn2, SIGNAL(clicked()), signalMapper2, SLOT(map()));
		signalMapper2->setMapping(btn2, DataTable[i].id);
		connect(signalMapper2, SIGNAL(mapped(int)), this, SLOT(OnBtnClickedDelete(int)));
	}
}

void admin_searchuser::NextPage() {
	if (PageIndex >= PageCount)return;
	if (PageIndex == 1)
		ui.btnLastPage->setEnabled(true);
	PageIndex++;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_searchuser::LastPage() {
	if (PageIndex <= 1)return;
	if (PageIndex == PageCount)
		ui.btnNextPage->setEnabled(true);
	PageIndex--;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

//�򿪷���ҳ��
void admin_searchuser::radioBtnSlot() {
	if (!studentClassifyConfig::isCheck) {
		studentClassifyConfig::isCheck = 1;
		admin_studentclassify *rec = new admin_studentclassify;
		this->hide();
		rec->show();
	}
}

//�ָ��ɱ༭
void admin_searchuser::radioBtnSlotOther() {
	studentClassifyConfig::isCheck = 0;
	ui.txtSearch->setText("");
	ui.txtSearch->setReadOnly(false);
}

void admin_searchuser::OnBtnClickedDelete(int id)
{
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	Record record;
	record.setStudentId(id);
	vector<Record>resRecord;
	vector<string>VALUES;
	VALUES.push_back("one");
	VALUES.push_back("studentId");
	FileDB::select("record", record, VALUES, resRecord);
	int sum = 0;
	for (int i = 0; i < resRecord.size(); i++) {
		if (resRecord[i].type != 4) {
			sum++;
		}
	}
	if (sum > 0) {
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("��ѧ���������ڽ裬����ɾ��"), QMessageBox::Ok);
		return;
	}
	else {
		Student cla;
		cla.setId(id);
		VALUES.pop_back();
		VALUES.push_back("id");
		FileDB::Delete("student", cla, VALUES);
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("ɾ���ɹ�"), QMessageBox::Ok);
		DataTable.clear();
		VALUES.clear();
		VALUES.push_back("all");
		FileDB::select("student", cla, VALUES, DataTable);
		DataBind();
		return;
	}
}
void admin_searchuser::OnBtnClickedReset(int id)
{
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	Student cla;
	cla.setId(id);
	vector<string>VALUES;
	vector<Student>resStudent;
	VALUES.push_back("one");
	VALUES.push_back("id");
	FileDB::select("student", cla, VALUES, resStudent);
	resStudent[0].setPassword("111111");
	FileDB::update("student", cla, resStudent[0], VALUES);
	QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("���óɹ���������Ϊ111111"), QMessageBox::Ok);
}

void admin_searchuser::OnBtnClickedClear(int id)
{
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	Student cla;
	cla.setId(id);
	vector<string>VALUES;
	vector<Student>resStudent;
	VALUES.push_back("one");
	VALUES.push_back("id");
	FileDB::select("student", cla, VALUES, resStudent);
	resStudent[0].setMoney(0);
	FileDB::update("student", cla, resStudent[0], VALUES);
	QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("����ɹ�"), QMessageBox::Ok);
}

void admin_searchuser::OnBtnClickedChange(int id)
{
	admin_adduser *rec = new admin_adduser;
	connect(this, SIGNAL(emitChange(int)), rec, SLOT(InitStudent(int)));
	emit emitChange(id);
	rec->show();
	this->close();
}
