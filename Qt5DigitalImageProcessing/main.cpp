#pragma execution_character_set("UTF-8")

#include <QApplication>
#include "Qt5DigitalImageProcessing.h"
#include "ImageProcess.h"
/*�������ȱ��:
		�����ͼƬ�Ǿ���QLabel�޸Ĺ��ߴ������ͼƬ,����ԭͼƬ
		�����ͼƬ�Ǿ���QLabel�޸Ĺ�������ͼƬ,��ԭͼƬ
		ˮӡ������ʱ��ʾ,����ˮӡ��ʽֻ��ͨ�������Ű�ť,Ҳ�ǻ���QLabel�޸Ĺ�������ͼƬ���ǵ�ˮӡ,��ԭͼ

	��ƸĽ�����:
		������������Ϊԭͼ, ��ɺ��ٽ�������ŵ�QLabel��
*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Qt5DigitalImageProcessing w;
	w.show();
	return a.exec();
}
