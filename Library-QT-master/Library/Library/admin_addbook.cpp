#include "admin_addbook.h"
#include "admin_adduser.h"
#include "admin_searchuser.h"
#include "admin_index.h"
#include"admin_searchbook.h"
#include"admin_searchuser.h"
#include"admin_classifyshow.h"
#include"admin_classify.h"
#include"filedb.h"
#include"dept.h"
#include"classifyMap.h"
#include"bookMap.h"
#include"Library.h"
#include"readexcel.h"
#include"admin_studentclassify.h"
#include<vector>
#include<QMessageBox>
#include<QFileDialog>
#include<QDateTime>
#include<QPixmap>
#include<QUrl>
#include <qtnetwork/qnetworkaccessmanager>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkReply>
#include<QEventLoop>
admin_addbook::admin_addbook(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	showFullScreen();
	ui.lineEdit->setEnabled(false);
	ui.lineEdit_2->setEnabled(false);
	ui.lineEdit_3->setEnabled(false);
	ui.lineEdit_4->setEnabled(false);
	ui.lineEdit_5->setEnabled(false);
	ui.lineEdit_6->setEnabled(false);
	ui.lineEdit_10->setEnabled(false);
	ui.lineEdit_13->setEnabled(false);
	ui.btnSearchuser->installEventFilter(this);
	ui.btnClassify->installEventFilter(this);
	ui.btnPersonal->installEventFilter(this);
	ui.btnAddMany->installEventFilter(this);
	ui.btnLogout->installEventFilter(this);
	ui.btnClassifyShow->installEventFilter(this);
	ui.btnAdd->installEventFilter(this);
	ui.btnCover->installEventFilter(this);
	filename = "images/default.png";
	img = new QPixmap;
	img->load(filename);
	ui.btnCover->setIcon(*img);
	ui.btnCover->setIconSize(QSize((*img).width(), (*img).height()));
	//�����޸�
	isChange = false;
}

admin_addbook::~admin_addbook()
{

}

bool admin_addbook::eventFilter(QObject *obj, QEvent *event)
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

	if (obj == ui.btnSearchuser && event->type() == QEvent::MouseButtonPress) {
		admin_searchuser *rec = new admin_searchuser;
		this->close();
		rec->show();
	}
	if (obj == ui.btnAddMany && event->type() == QEvent::MouseButtonPress) {
		AddMany();
	}
	if (obj == ui.btnPersonal && event->type() == QEvent::MouseButtonPress) {
		admin_index *rec = new admin_index;
		this->close();
		rec->show();
	}
	if (obj == ui.btnClassifyShow && event->type() == QEvent::MouseButtonPress) {
		admin_classifyshow *rec = new admin_classifyshow;
		connect(rec, SIGNAL(UpdateClassSignal(QString)), this, SLOT(UpdateClassSlot(QString)));
		rec->show();
	}

	if (obj == ui.btnClassify && event->type() == QEvent::MouseButtonPress) {
		admin_classify *rec = new admin_classify;
		this->close();
		rec->show();
	}
	if (obj == ui.btnCover && event->type() == QEvent::MouseButtonPress) {
		openFileDiag();
	}
	if (obj == ui.btnAdd && event->type() == QEvent::MouseButtonPress) {
		QString bookName = ui.etName->text();
		QString bookClass = ui.btnClassifyShow->text();
		QString bookISBN = ui.etISBN->text();
		QString bookAuthor = ui.etAuthor->text();
		QString bookPublish = ui.etPublish->text();
		QString bookCount = ui.etCount->text();
		QString bookScore = ui.etScore->text();
		QRegExp rx("[1-9][0-9]+");
		QRegExpValidator v(rx, 0);
		int pos = 0;
		int res = v.validate(bookISBN, pos);
		if (!res) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("ISBN������Ҫ��"), QMessageBox::Ok);
			return true;
		}
		pos = 0;
		int res3 = v.validate(bookCount, pos);
		if (!res3) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("�����������Ҫ��"), QMessageBox::Ok);
			return true;
		}
		QRegExp rx2("^\\d+(\\.\\d+)?$");
		QRegExpValidator v2(rx2, 0);
		int pos2 = 0;
		int res2 = v2.validate(bookScore, pos2);
		if (!res2) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("���ֲ�����Ҫ��"), QMessageBox::Ok);
			return true;
		}
		if (!bookName.length() || !bookClass.length()
			|| !bookISBN.length() || !bookAuthor.length()
			|| !bookISBN.length() || !bookISBN.length()
			|| !bookPublish.length() || !bookCount.length()
			|| !bookScore.length()) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("����д����"), QMessageBox::Ok);
			return true;
		}
		if (bookName.length()>50 || bookClass.length()>50
			|| bookISBN.length()>15 || bookAuthor.length()>50
			|| bookPublish.length()>50 || bookCount.length()>10
			|| bookScore.length()>10) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("��д����̫��"), QMessageBox::Ok);
			return true;
		}
		if (bookClass == QString::fromLocal8Bit("��ѡ�����")) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("����ѡ�����"), QMessageBox::Ok);
			return true;
		}
		QByteArray ba1 = bookName.toLocal8Bit();
		char *name = ba1.data();
		QByteArray ba2 = bookClass.toLocal8Bit();
		char *classs = ba2.data();
		QByteArray ba3 = bookISBN.toLocal8Bit();
		char *ISBN = ba3.data();
		QByteArray ba4 = bookAuthor.toLocal8Bit();
		char *author = ba4.data();
		QByteArray ba5 = bookPublish.toLocal8Bit();
		char *publish = ba5.data();

		int countt = bookCount.toInt();
		float score = bookScore.toFloat();
		if (countt >= 100) {
			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("�����鼮��治�ܳ���100��"), QMessageBox::Ok);
			return true;
		}

		QByteArray ba6 = filename.toLocal8Bit();
		char *cover = ba6.data();
		

		if (!isChange) {
			//�ж�ISBN�Ƿ��ظ�
			Book book;
			vector<string>VALUES;
			vector<Book>resBook;
			VALUES.push_back("one");
			VALUES.push_back("ISBN");
			book.setISBN(ISBN);
			FileDB::select("book", book, VALUES, resBook);

			if (resBook.size() > 0) {
				QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("ISBN�������鱾�ظ����������"), QMessageBox::Ok);
				return true;
			}
			//��ȡclassId
			Classify cla;
			vector<Classify>resCla;
			cla.setName(classs);
			VALUES.pop_back();
			VALUES.push_back("name");
			FileDB::select("classify", cla, VALUES, resCla);

			//����book��
			book.setAuthor(author);
			book.setClassifyId(resCla[0].id);
			book.setCount(countt);
			book.setCover(cover);
			book.setISBN(ISBN);
			book.setName(name);
			book.setNowCount(countt);
			book.setPublish(publish);
			book.setScore(score);
			(*img).save(filename);

			resBook.push_back(book);
			FileDB::insert("book", resBook);


			//��ȡ�²������id��ά��classifyMap
			resBook.clear();
			VALUES.pop_back();
			VALUES.push_back("ISBN");
			FileDB::select("book", book, VALUES, resBook);
			ClassifyMap clamap;
			clamap.setBookId(resBook[0].id);
			clamap.setClassifyId(resCla[0].id);
			vector<ClassifyMap>resClsmap;
			resClsmap.push_back(clamap);
			FileDB::insert("classifyMap", resClsmap);

			//ά��bookMap
			BookMap bookmap;
			vector<BookMap>resBookmap;
			for (int i = 1; i <= countt; i++) {
				bookmap.setBookId((resBook[0].id));
				bookmap.setBookNum(i);
				bookmap.setIsOut(0);
				resBookmap.push_back(bookmap);
			}
			FileDB::insert("bookMap", resBookmap);

			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("��ӳɹ�"), QMessageBox::Ok);
		}
		//�޸�
		else {
			//��ȡclassId
			vector<string>VALUES;
			Classify cla;
			vector<Classify>resCla;
			cla.setName(classs);
			VALUES.push_back("one");
			VALUES.push_back("name");
			FileDB::select("classify", cla, VALUES, resCla);

			Book book;
			//����book��
			book.setAuthor(author);
			book.setClassifyId(resCla[0].id);
			book.setCount(countt);
			book.setCover(cover);
			book.setISBN(ISBN);
			book.setName(name);
			book.setNowCount(countt);
			book.setPublish(publish);
			book.setScore(score);
			(*img).save(filename);
			Book Sbook;
			Sbook.setId(BookId);
			VALUES.pop_back();
			VALUES.push_back("id");
			FileDB::update("book", Sbook, book, VALUES);
			//ά��classifyMap
			ClassifyMap clamap;
			vector<ClassifyMap>resClamap;
			clamap.setBookId(BookId);
			VALUES.pop_back();
			VALUES.push_back("bookId");
			//ȫ��ɾ��
			FileDB::Delete("classifyMap", clamap, VALUES);
			//�������
			clamap.setBookId(BookId);
			clamap.setClassifyId(resCla[0].id);
			resClamap.push_back(clamap);
			FileDB::insert("classifyMap", resClamap);

			QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("�޸ĳɹ�"), QMessageBox::Ok);
		}

	}

	else {
		return QWidget::eventFilter(obj, event);
	}

	return false;
}

void admin_addbook::openFileDiag() {
	filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼ��"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty()) {
		return;
	}
	else {
		img = new QPixmap;
		if (!(img->load(filename))) {
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ͼ��ʧ��"),
				QString::fromLocal8Bit("��ͼ��ʧ��!"));
		}
		QDateTime time = QDateTime::currentDateTime();   //��ȡ��ǰʱ��  
		int timeT = time.toTime_t();   //����ǰʱ��תΪʱ���
		filename = "images/" + QString::number(timeT, 10) + ".jpg";
		ui.btnCover->setIcon(*img);
		ui.btnCover->setIconSize(QSize((*img).width(), (*img).height()));
		return;
	}
}
void admin_addbook::UpdateClassSlot(QString id) {
	ui.btnClassifyShow->setText(id);
}

void admin_addbook::InitBook(int id) {
	//�޸�
	BookId = id;
	isChange = true;
	ui.etISBN->setReadOnly(true);
	Book book;
	book.setId(id);
	vector<string>VALUES;
	VALUES.push_back("one");
	VALUES.push_back("id");
	FileDB::select("book", book, VALUES, resBook);
	ui.etName->setText(QString::fromLocal8Bit( resBook[0].name));
	ui.etAuthor->setText(QString::fromLocal8Bit(resBook[0].author));
	ui.etISBN->setText(QString::fromLocal8Bit(resBook[0].ISBN));
	ui.etPublish->setText(QString::fromLocal8Bit(resBook[0].publish));
	ui.etCount->setText(QString::number(resBook[0].count));
	ui.etScore->setText(QString("%1").arg(resBook[0].score));
	ui.etCount->setReadOnly(true);
	Classify classify;
	vector<Classify>resClassify;
	classify.setId(resBook[0].classifyId);
	FileDB::select("classify", classify, VALUES, resClassify);
	ui.btnClassifyShow->setText(QString::fromLocal8Bit(resClassify[0].name));

	
	filename = QString::fromLocal8Bit(resBook[0].cover);
	if (!strstr(resBook[0].cover, "images")) {
		QNetworkAccessManager manager;
		QEventLoop loop;
		QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(filename)));
		//���������������ɺ��˳����¼�ѭ��
		QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
		//�������¼�ѭ��
		loop.exec();
		QByteArray jpegData = reply->readAll();
		QPixmap pixmap;
		pixmap.loadFromData(jpegData);
		//�ı�ͼƬ��С
		ui.btnCover->setIcon(pixmap);
		ui.btnCover->setIconSize(QSize((pixmap).width(), (pixmap).height()));
	}
	else {
		img = new QPixmap;
		img->load(filename);
		//�ı�ͼƬ��С
		ui.btnCover->setIcon(*img);
		ui.btnCover->setIconSize(QSize((*img).width(), (*img).height()));
	}
}

void admin_addbook::AddMany() {
	QMessageBox::information(NULL, QString::fromLocal8Bit(""), QString::fromLocal8Bit("��ѡ��Excel�ļ�,�ļ���������ISBN�����������ߡ������硢�ݲ����������֡����������ź�"), QMessageBox::Ok);
	QString excelPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��Exccel"), "", tr("Excel (*.xls *.xlsx)"));
	if (excelPath.isEmpty()) {
		return;
	}
	ReadExcel read;
	excelPath.replace(QString("/"), QString("\\"));
	read.openExcel(excelPath);
	int col, row;
	read.getInfo(row, col);
	Book book;
	
	int sumBooks = 0;
	
	for (int i = 1; i <= row; i++) {

			string ISBN = read.getCellData(i, 1).toLocal8Bit();
			if (!checkISBN(ISBN))continue;
			book.setISBN((char*)ISBN.data());
			string cla= read.getCellData(i, 7).toLocal8Bit();
			int clasres = checkClass(cla);
			if (clasres ==-1)continue;
			book.setClassifyId(clasres);
			string name = read.getCellData(i, 2).toLocal8Bit();
			string author = read.getCellData(i, 3).toLocal8Bit();
			string publish = read.getCellData(i, 4).toLocal8Bit();
			if (name.length() == 0 || name.length() >= 50)continue;
			if (author.length() == 0 || name.length() >= 50)continue;
			if (publish.length() == 0 || name.length() >= 50)continue;
			string nummm = read.getCellData(i, 5).toLocal8Bit();
			if (!checkCount(nummm))continue;
			int numC = read.getCellData(i, 5).toInt();
			string scoreee = read.getCellData(i, 6).toLocal8Bit();
			if (!checkScore(scoreee))continue;
			float score = read.getCellData(i, 6).toFloat();
			book.setName((char*)name.data());
			string cover = "images/defaultBook.jpg";
			book.setAuthor((char*)author.data());
			book.setCover((char*)cover.data());
			book.setPublish((char*)publish.data());
			book.setScore(score);
			book.setNowCount(numC);
			book.setCount(numC);
			vector<Book>resBookk;
			resBookk.push_back(book);
			FileDB::insert("book", resBookk);
			vector<string>VALUES;
			//��ȡ�²������id��ά��classifyMap
			resBookk.clear();
			VALUES.push_back("one");
			VALUES.push_back("ISBN");
			FileDB::select("book", book, VALUES, resBookk);
			ClassifyMap clamap;
			clamap.setBookId(resBookk[0].id);
			clamap.setClassifyId(clasres);

			vector<ClassifyMap>resClsmap;
			resClsmap.push_back(clamap);
			FileDB::insert("classifyMap", resClsmap);

			//ά��bookMap
			BookMap bookmap;
			vector<BookMap>resBookmap;
			for (int k = 1; k<= numC; k++) {
				bookmap.setBookId((resBookk[0].id));
				bookmap.setBookNum(k);
				bookmap.setIsOut(0);
				resBookmap.push_back(bookmap);
			}
			FileDB::insert("bookMap", resBookmap);
			sumBooks++;	
	}
	QString success = QString::number(sumBooks);
	QMessageBox::information(NULL, QString::fromLocal8Bit(""), success + QString::fromLocal8Bit("��ͼ�鵼��ɹ�"), QMessageBox::Ok);
}

bool admin_addbook::checkISBN(string ISBN) {
	if (ISBN.length() == 0 || ISBN.length() >= 14)return false;
	QRegExp rx("[1-9][0-9]+");
	QRegExpValidator v(rx, 0);
	int pos = 0;
	int res = v.validate(QString::fromLocal8Bit(ISBN.data()), pos);
	if (!res) {
		return false;
	}
	//�ж�ISBN�Ƿ��ظ�
	Book book;
	vector<string>VALUES;
	vector<Book>resBook;
	VALUES.push_back("one");
	VALUES.push_back("ISBN");
	book.setISBN((char*)ISBN.data());
	FileDB::select("book", book, VALUES, resBook);

	if (resBook.size() > 0) {
		return false;
	}
	else {
		return true;
	}
}

bool admin_addbook:: checkCount(string Count) {
	if (Count.length() == 0 || Count.length() >= 14)return false;
	QRegExp rx("[1-9][0-9]+");
	QRegExpValidator v(rx, 0);
	int pos = 0;
	int res3 = v.validate(QString::fromLocal8Bit(Count.data()), pos);
	if (!res3) {
		return false;
	}
	QString count= QString::fromStdString(Count);
	int countt = count.toInt();
	if (countt >= 100) {
		return false;
	}
	else {
		return true;
	}
}

int admin_addbook:: checkClass(string cla) {
	//�ж�ISBN�Ƿ��ظ�
	Classify book;
	vector<string>VALUES;
	vector<Classify>resBook;
	VALUES.push_back("one");
	VALUES.push_back("name");
	book.setName((char*)cla.data());
	FileDB::select("classify", book, VALUES, resBook);

	if (resBook.size() > 0) {
		return resBook[0].id;
	}
	else {
		return -1;
	}
}

bool admin_addbook:: checkScore(string Score) {
	if (Score.length() == 0 || Score.length() >= 30)return false;
	QRegExp rx2("^\\d+(\\.\\d+)?$");
	QRegExpValidator v2(rx2, 0);
	int pos2 = 0;
	int res2 = v2.validate(QString::fromLocal8Bit(Score.data()), pos2);
	if (!res2) {
		return false;
	}
	else {
		return true;
	}
}


