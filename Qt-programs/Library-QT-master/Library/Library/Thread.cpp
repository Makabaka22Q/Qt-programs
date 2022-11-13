#include"Thread.h"
#include"recommendBuffer.h"
#include<QPixmap>
#include<QUrl>
#include <qtnetwork/qnetworkaccessmanager>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkRequest>
#include <qtnetwork/QNetworkReply>
#include<QEventLoop>
Thread::Thread(QObject *parent)
{
}

Thread::~Thread()
{

}

void Thread::run()
{
	if (!recommendBuffer::headUrlLocal.length()) {
		QNetworkAccessManager manager;
		QEventLoop loop;
		QNetworkReply *reply = manager.get(QNetworkRequest(recommendBuffer::headUrl));
		//���������������ɺ��˳����¼�ѭ��
		QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
		//�������¼�ѭ��
		loop.exec();
		QByteArray jpegData = reply->readAll();
		QPixmap pixmap;
		pixmap.loadFromData(jpegData);
		//�ı�ͼƬ��С
		pixmap = pixmap.scaled(110, 130, Qt::KeepAspectRatio);
		recommendBuffer::headBuffer.push_back(pixmap);
	}
	else {
		QPixmap pixmap;
		pixmap.load(recommendBuffer::headUrlLocal);
		//�ı�ͼƬ��С
		pixmap = pixmap.scaled(110, 130, Qt::KeepAspectRatio);
		recommendBuffer::headBuffer.push_back(pixmap);
	}

	for (int i = 0; i < recommendBuffer::urlBuffer.size(); i++) {
		if (!recommendBuffer::isPostBack) {
			QNetworkAccessManager manager;
			QEventLoop loop;
			QNetworkReply *reply = manager.get(QNetworkRequest(recommendBuffer::urlBuffer[i]));
			//���������������ɺ��˳����¼�ѭ��
			QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
			//�������¼�ѭ��
			loop.exec();
			QByteArray jpegData = reply->readAll();
			QPixmap pixmap;
			pixmap.loadFromData(jpegData);
			pixmap = pixmap.scaled(110, 130, Qt::KeepAspectRatio);
			recommendBuffer::picBuffer.push_back(pixmap);
		}
	}
	for (int i = 0; i < recommendBuffer::urlBufferLocal.size(); i++) {
		if (!recommendBuffer::isPostBack) {
			QPixmap pixmap;
			pixmap.load(recommendBuffer::urlBufferLocal[i]);
			pixmap = pixmap.scaled(110, 130, Qt::KeepAspectRatio);
			recommendBuffer::picBuffer.push_back(pixmap);
		}
	}
	emit UpdateSignal();
}
