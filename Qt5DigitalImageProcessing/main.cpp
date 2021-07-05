#pragma execution_character_set("UTF-8")

#include <QApplication>
#include "Qt5DigitalImageProcessing.h"
#include "ImageProcess.h"
/*该设计有缺陷:
		处理的图片是经过QLabel修改过尺寸的缩放图片,并非原图片
		保存的图片是经过QLabel修改过的缩放图片,非原图片
		水印可以暂时显示,消除水印方式只能通过上下张按钮,也是基于QLabel修改过的缩放图片覆盖的水印,非原图

	设计改进方向:
		将处理对象更改为原图, 完成后再将结果缩放到QLabel中
*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Qt5DigitalImageProcessing w;
	w.show();
	return a.exec();
}
