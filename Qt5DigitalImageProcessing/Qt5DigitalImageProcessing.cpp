#include "Qt5DigitalImageProcessing.h"

Qt5DigitalImageProcessing::Qt5DigitalImageProcessing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	_playRate = 1;
	_delay = 0;
	ui.pushButtonPreImg->setDisabled(true);
	ui.pushButtonNextImg->setDisabled(true);


	_customMsgBox.setWindowTitle("About Software");
	_customMsgBox.addButton("OK", QMessageBox::ActionRole);
	_customMsgBox.setText("欢迎使用《多功能图像视频处理》软件！本软件具有简单的图像和视频处理功能。\n"
		"图像功能包括多选打开、旋转、镜像、灰度化、均值滤波、"
		"边缘检测、原图复合、伽马检测、二值化、色彩调整、亮度调整、对比度调整、饱和度调整等功能。\n"
		"视频功能包括暂停、播放、进度条、灰度化、边缘检测、平滑、二值化、局部马赛克、缩放等功能。\n");

	ui.statusBar->showMessage("Welcome", 2000);

	QLabel* permanentLabel = new QLabel(this);
	permanentLabel->setObjectName("status");			//创建永久的标签对象status,用于下方代码显示路径等
	permanentLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
	permanentLabel->setText("Welcome to using");

	ui.statusBar->addPermanentWidget(permanentLabel);
	ui.tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px;background:  transparent; }");

}

Qt5DigitalImageProcessing::~Qt5DigitalImageProcessing()
{
}

/*
	选择图片按钮
	功能: 加载图片,大label显示选择图片,小label第一个显示当前图片缩略图,其他小label显示其他图片缩略图
*/
void Qt5DigitalImageProcessing::on_pushButtonSelectImage_clicked()
{
	ImageProcess imgProcess(this);

	_imgsPathList= QFileDialog::getOpenFileNames(
		this, "选择图片", QDir::currentPath(), "jpg(*.jpg);;png(*.png);;jpeg(*.jpeg);;all(*.*)");

//加载图片个数大于3时:加载图片,并使小label可见
	if (_imgsPathList.size() >= 3 )		
	{
		//处理第一张图片,展示在大Label中
		_index = 0;
		QString srcDirPath = _imgsPathList.at(_index);  //获取路径列表中第1个
		QImage image(srcDirPath);
		//调整图片格式并显示在labelShow中
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //消除图片和label间隙
		//将当前图片展示在小Label中,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//非主图缩略图显示在小label中,即第2张图
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //该余数就是当前索引值
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);

		//非主图缩略图显示在小label中,即第3张图
		QString image3Path = _imgsPathList.at((_index + 2) % _imgsPathList.size());   //该余数就是当前索引值
		QImage image3(image3Path);
		QImage image3Center = imgProcess.imageCenter(&image3, ui.labelOther3);
		ui.labelOther3->setPixmap(QPixmap::fromImage(image3Center));
		ui.labelOther3->setAlignment(Qt::AlignCenter);

		//将上一张和下一张按钮使能,并显示小标签
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(true);


		//状态栏显示路径
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//标签对象status在构造函数中声明
		showImgPath->setText(srcDirPath);	//显示第一张即大图片地址
	}
	else if (_imgsPathList.size() == 1)
	{
		//处理第一张图片,展示在大Label中
		_index = 0;
		QString srcDirPath = _imgsPathList.at(_index);  //获取路径列表中第1个
		QImage image(srcDirPath);
		//调整图片格式并显示在labelShow中
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //消除图片和label间隙
		//将当前图片展示在小Label中,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//将上一张和下一张按钮功能关闭,并隐藏小标签
		ui.pushButtonNextImg->setDisabled(true);
		ui.pushButtonPreImg->setDisabled(true);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(false);
		ui.labelOther3->setVisible(false);


		//状态栏显示路径
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//标签对象status在构造函数中声明
		showImgPath->setText(srcDirPath);	//显示第一张即大图片地址
	}
	else if (_imgsPathList.size() ==2 )
	{
		// 处理第一张图片, 展示在大Label中
			_index = 0;
		QString srcDirPath = _imgsPathList.at(_index);  //获取路径列表中第1个
		QImage image(srcDirPath);
		//调整图片格式并显示在labelShow中
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //消除图片和label间隙
		//将当前图片展示在小Label中,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//非主图缩略图显示在小label中,即第2张图
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //该余数就是当前索引值
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);


		//将上一张和下一张按钮使能,并显示小标签
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(false);


		//状态栏显示路径
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//标签对象status在构造函数中声明
		showImgPath->setText(srcDirPath);	//显示第一张即大图片地址
	}
}
