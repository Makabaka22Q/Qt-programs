#pragma once
#include<qstring.h>
#include<QPixmap>

class recommendBuffer
{

public:

	//�Ƿ��һ�μ���
	static bool isPostBack;
	//ͼƬ����
	static QVector<QPixmap>picBuffer;
	static QVector<QPixmap>headBuffer;
	static QVector<QUrl>urlBuffer;
	static QVector<QString>urlBufferLocal;
	static QVector<int>idBuffer;
	static QUrl headUrl;
	static QString headUrlLocal;
	recommendBuffer();
	~recommendBuffer();
	static void Resert();
};