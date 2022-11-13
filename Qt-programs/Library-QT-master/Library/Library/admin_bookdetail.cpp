#include "admin_bookdetail.h"
#include"filedb.h"
#include<QSignalMapper>
#include"bookConfig.h"
#include"bookMap.h"
#include"record.h"
#include"student.h"
#include"library.h"
#include"book.h"
#include<QPushButton>
#include<QMessageBox>
admin_bookdetail::admin_bookdetail(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	show();
	ui.tableWidget->setColumnCount(5);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭	
	ui.tableWidget->verticalHeader()->setVisible(false); //�����кŲ��ɼ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//��������Ӧ
	ui.tableWidget->setShowGrid(false);
	//// ���ñ�ͷ���ɼ�����Ҫ���QHeadViewͷ�ļ� 
	//ui.tableWidget->horizontalHeader()->setVisible(false);// ˮƽ���ɼ� 
	//ui.tableWidget->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
	DataBind();
}

admin_bookdetail::~admin_bookdetail()
{
}
void admin_bookdetail::DataBind() {
	BookMap bookmap;
	vector<string>VALUES;
	vector<string>VALUESGG;
	vector<BookMap>resBookmap;
	VALUES.push_back("one");
	VALUES.push_back("bookId");
	VALUESGG.push_back("one");
	VALUESGG.push_back("bookId");
	bookmap.setBookId(bookConfig::bookId);
	FileDB::select("bookMap", bookmap, VALUES, DataTable);
	
	Record record;
	vector<Record>resRecord;
	for (int i = 0; i < DataTable.size(); i++) {
		ui.tableWidget->insertRow(i);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromLocal8Bit("����") + QString::number(i + 1, 10)));
		//δ�����
		if (!DataTable[i].isOut) {
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("δ���")));
			QPushButton *btn = new QPushButton;
			ui.tableWidget->setCellWidget(i, 4, btn);

			QSignalMapper* signalMapper = new QSignalMapper(this);
			connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
			signalMapper->setMapping(btn, DataTable[i].id);
			connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnClicked(int)));
			btn->setStyleSheet(
				"color:#4695d2;"
				"border:none;"
				"background:white;"
				"text-size:20px;"
			);
			btn->setText(QString::fromLocal8Bit("ɾ��"));
		}
		//���
		else {
			record.setBookId(DataTable[i].id);
			resRecord.clear();
			FileDB::select("record", record, VALUESGG, resRecord);
			if (resRecord.size() > 0) {
				int j;
				for (j = 0; i < resRecord.size(); j++) {
					if (resRecord[j].type != 4)break;
				}
				//if (resRecord[0].type == 0)
				ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("�ѽ��")));
				//else if (resRecord[0].type == 1)
					//ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("����δ��")));
				//else
					//ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("��ԤԼ")));

				Student student;
				vector<Student>resStudent;
				student.setId(resRecord[j].studentId);
				VALUES.pop_back();
				VALUES.push_back("id");
				FileDB::select("student", student, VALUES, resStudent);
				ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromLocal8Bit(resStudent[0].usercode)));
				ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit(resRecord[j].time)));
			}
		}
	}
}

void admin_bookdetail::OnBtnClicked(int id)
{
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, QString::fromLocal8Bit("�˳�����"),
		QString(QString::fromLocal8Bit("ȷ��ɾ���Ȿ����?")),
		QMessageBox::Yes | QMessageBox::No);
	if (button == QMessageBox::No) {
		
	}
	else if (button == QMessageBox::Yes) {
		//ɾ��bookMap
		BookMap bookmap;
		bookmap.setId(id);
		vector<string>VALUES;
		vector<BookMap>resBookmap;
		VALUES.push_back("one");
		VALUES.push_back("id");
		FileDB::select("bookMap", bookmap, VALUES, resBookmap);
		FileDB::Delete("bookMap", bookmap, VALUES);
		//����book����
		Book book;
		book.setId(resBookmap[0].bookId);
		vector<Book>resBook;
		FileDB::select("book", book, VALUES, resBook);

		resBook[0].setCount(resBook[0].count - 1);
		resBook[0].setNowCount(resBook[0].nowCount - 1);
		//���������0����ɾ���Ȿ��
		if (resBook[0].count == 0) {
			FileDB::Delete("book", book, VALUES);
		}
		else {
			FileDB::update("book", book, resBook[0], VALUES);
		}
		//���źţ���ʾɾ���ɹ����ص�����֮ǰ�Ľ���
		emit DeleteSuccess(resBookmap[0].bookId);
		this->close();
	}
}

