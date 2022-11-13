#include "admin_classify.h"
#include "admin_searchbook.h"
#include "admin_index.h"
#include"admin_searchuser.h" 
#include"admin_classify.h"
#include"classify.h"
#include"library.h"
#include"filedb.h"
#include"admin_addclassify.h"
#include"classifyMap.h"
#include<QSignalMapper>
#include<QMessageBox>
#include<QTextCodec>
admin_classify::admin_classify(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.btnSearchuser->installEventFilter(this);
	ui.btnSearchbook->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnLastPage->installEventFilter(this);
	ui.btnAdd->installEventFilter(this);
	ui.btnFirstPage->installEventFilter(this);
	ui.btnLastPage->installEventFilter(this);
	ui.btnNextPage->installEventFilter(this);
	ui.btnSearch->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnTheLast->installEventFilter(this);
	ui.tableWidget->setColumnCount(3);
	ui.btnLastPage->setEnabled(false);
	ui.btnFirstPage->setEnabled(false);
	
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableWidget->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableWidget->setFrameShape(QFrame::NoFrame);//�����ޱ߿�
	ui.tableWidget->setShowGrid(false); //���ò���ʾ������
}

admin_classify::~admin_classify()
{

}

bool admin_classify::eventFilter(QObject *obj, QEvent *event)
{

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
	//if (obj == ui.btnLogout && event->type() == QEvent::MouseButtonPress) {
	//	QMessageBox::StandardButton button;
	//	button = QMessageBox::question(this, QString::fromLocal8Bit("�˳�����"),
	//		QString(QString::fromLocal8Bit("ȷ���˳�����?")),
	//		QMessageBox::Yes | QMessageBox::No);
	//	if (button == QMessageBox::No) {
	//		event->ignore();  //�����˳��źţ������������
	//	}
	//	else if (button == QMessageBox::Yes) {
	//		Library *rec = new Library;
	//		this->close();
	//		rec->show();
	//		event->accept();  //�����˳��źţ������˳�
	//	}
	//}

	if (obj == ui.btnSearchuser && event->type() == QEvent::MouseButtonPress) {
		admin_searchuser *rec = new admin_searchuser;
		this->close();
		rec->show();
	}
	if (obj == ui.btnAdd && event->type() == QEvent::MouseButtonPress) {
		admin_addclassify *rec = new admin_addclassify;
		rec->show();
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		admin_index *rec = new admin_index;
		this->close();
		rec->show();
	}
	if (obj == ui.btnSearchbook && event->type() == QEvent::MouseButtonPress) {
		admin_searchbook *rec = new admin_searchbook;
		this->close();
		rec->show();
	}

	else {
		return QWidget::eventFilter(obj, event);
	}

	return false;
}
void admin_classify::FirstPage() {
	PageIndex = 1;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_classify::TheLastPage() {
	PageIndex = PageCount;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_classify::SearchData() {
	QString str = ui.txtSearch->toPlainText();
	QByteArray ba = str.toLocal8Bit();
	char *name = ba.data();
	DataTable.clear();
	//ʲôҲ�����룬��ѯȫ��ͼ��
	if (str.length() == 0) {
		vector<string>VALUES;
		VALUES.push_back("all");
		Classify classify;
		FileDB::select("classify", classify, VALUES, DataTable);
	}
	else {
		FileDB::selectLike("classify", "name", name, DataTable);
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

void admin_classify::DataBind() {
	for (int i = ui.tableWidget->rowCount(); i > 0; i--) {
		ui.tableWidget->removeRow(0);
	}
	int currentPageBegin = (PageIndex - 1)*PageLength;
	int currentPageEnd = (currentPageBegin + PageLength) < DataTable.size() ? (currentPageBegin + PageLength) : DataTable.size();
	for (int i = currentPageBegin; i < currentPageEnd; i++) {
		ui.tableWidget->insertRow(i - currentPageBegin);
		ui.tableWidget->setRowHeight(i - currentPageBegin, 50);
		QTableWidgetItem *temp = new QTableWidgetItem(QString::fromLocal8Bit(DataTable[i].name));
		temp->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i - currentPageBegin, 0, temp);
		//��ӡ����顱��ť�������¼�
		QPushButton *btn = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 1, btn);
		btn->setText(QString::fromLocal8Bit("�޸�"));
		btn->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(btn, QString::fromLocal8Bit(DataTable[i].name));
		connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(OnBtnClicked(QString)));

		QPushButton *btn2 = new QPushButton;
		ui.tableWidget->setCellWidget(i - currentPageBegin, 2, btn2);
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

void admin_classify::NextPage() {
	if (PageIndex >= PageCount)return;
	if (PageIndex == 1)
		ui.btnLastPage->setEnabled(true);
	PageIndex++;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_classify::LastPage() {
	if (PageIndex <= 1)return;
	if (PageIndex == PageCount)
		ui.btnNextPage->setEnabled(true);
	PageIndex--;
	ui.etIndexPage->setText(QString::number(PageIndex, 10));
	DataBind();
}

void admin_classify::OnBtnClickedDelete(int id){
	QTextCodec * BianMa = QTextCodec::codecForName("GBK");
	ClassifyMap classify;
	classify.setClassifyId(id);
	vector<ClassifyMap>resClassify;
	vector<string>VALUES;
	VALUES.push_back("one");
	VALUES.push_back("classifyId");
	FileDB::select("classifyMap", classify, VALUES, resClassify);
	if (resClassify.size() > 0) {
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("�˷�������������ڣ�����ɾ��"), QMessageBox::Ok);
		return ;
	}
	else {
		Classify cla;
		cla.setId(id);
		VALUES.pop_back();
		VALUES.push_back("id");
		FileDB::Delete("classify", cla, VALUES);
		QMessageBox::information(NULL, BianMa->toUnicode(""), BianMa->toUnicode("ɾ���ɹ�"), QMessageBox::Ok);
		DataTable.clear();
		VALUES.clear();
		VALUES.push_back("all");
		FileDB::select("classify", cla, VALUES, DataTable);
		DataBind();
		return;
	}
}

void admin_classify::OnBtnClicked(QString id) {
	admin_addclassify *rec = new admin_addclassify;
	connect(this, SIGNAL(emitChangeClassify(QString)), rec, SLOT(InitClassify(QString)));
	emit emitChangeClassify(id);
	rec->show();
	//this->close();
}
