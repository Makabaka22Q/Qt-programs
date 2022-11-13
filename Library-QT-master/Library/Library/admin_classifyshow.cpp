#include "admin_classifyshow.h"
#include"classify.h"
#include"classifyConfig.h" 
#include "admin_searchbook.h"
#include "admin_index.h"
#include"admin_searchuser.h"
#include"admin_classify.h"
#include"filedb.h"
#include"library.h"
#include<QMessageBox>
#include<QTextCodec>
#include<QSignalMapper>
admin_classifyshow::admin_classifyshow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(5);
	//classifyConfig::isCheck = 0;
	ui.btnSearchuser->installEventFilter(this);
	ui.btnClassify->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableWidget->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableWidget->setShowGrid(false);
	// ���ñ�ͷ���ɼ�����Ҫ���QHeadViewͷ�ļ� 
	ui.tableWidget->horizontalHeader()->setVisible(false);// ˮƽ���ɼ� 
	ui.tableWidget->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
	DataBind();
}

bool admin_classifyshow::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui.btnLogout && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("����ѡ�����"), QMessageBox::Ok);
	}
	if (obj == ui.btnSearchuser && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("����ѡ�����"), QMessageBox::Ok);
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("����ѡ�����"), QMessageBox::Ok);
	}
	if (obj == ui.btnClassify && event->type() == QEvent::MouseButtonPress) {
		QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("����ѡ�����"), QMessageBox::Ok);
	}
	else {
		return QWidget::eventFilter(obj, event);
	}

	return false;
}

void admin_classifyshow::DataBind() {
	//��������
	Classify classify;
	vector<string>VALUES;
	VALUES.push_back("all");
	FileDB::select("classify", classify, VALUES, DataTable);

	for (int i = ui.tableWidget->rowCount(); i > 0; i--) {
		ui.tableWidget->removeRow(0);
	}
	int lineCount = -1;

	for (int i = 0; i < DataTable.size(); i++) {
		if (i % 5 == 0) {
			lineCount++;
			ui.tableWidget->insertRow(lineCount);
			ui.tableWidget->setRowHeight(lineCount, 40);//��һ��
		}
		QPushButton *btn = new QPushButton;

		ui.tableWidget->setCellWidget(lineCount, i % 5, btn);

		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(btn, DataTable[i].id);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnClicked(int)));
		btn->setStyleSheet(
			"color:#4695d2;"
			"border:none;"
			"background:white;"
			"text-size:20px;"
		);
		btn->setText(QString::fromLocal8Bit(DataTable[i].name));
	}
}

void admin_classifyshow::OnClicked(int id)
{
	if (classifyConfig::isCheck) {
		//classifyConfig::isCheck = 1;
		classifyConfig::classifyId = id;
		admin_searchbook *rec = new admin_searchbook;
		rec->show();
		this->close();
	}
	else {
		Classify cla;
		cla.setId(id);
		vector<string>VALUES;
		VALUES.push_back("one");
		VALUES.push_back("id");
		vector<Classify>resCla;
		FileDB::select("classify", cla, VALUES, resCla);
		emit UpdateClassSignal(QString::fromLocal8Bit( resCla[0].name));
		this->close();
	}
}

admin_classifyshow::~admin_classifyshow()
{
}
