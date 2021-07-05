#include "Qt5DigitalImageProcessing.h"
#include <qdebug.h>
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
	该槽函数同时通过ui界面的方法关联了QAction对象actionOpen
*/
void Qt5DigitalImageProcessing::on_pushButtonSelectImage_clicked()
{
	ImageProcess imgProcess(this);
	QFileDialog fileDlg;
	QString title("selected images");
	QString filter("jpg(*.jpg);;png(*.png);;jpeg(*.jpeg);;all(*.*)");
	fileDlg.resize(QSize(300, 400));	//尝试修改标准对话框初始大小失败,应继承QDialog后封装
	_imgsPathList= QFileDialog::getOpenFileNames(this, title, QDir::currentPath(),filter );
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


/*
上一张图片按钮
	功能:利用成员_index保存图片位置信息
	实现算法: 
				%的妙用, 使用%实现 i 在0-n之间的循环 	i = (i++)%n, 其中 i from 0 to n-1
			1) 小label的图片显示问题:
					小label是紧跟第i张图片的即循环队伍里的 i+1 ,定位出当前的图片i, 然后把i看为新顺序的第一张,小label的顺序就是(i+1)%size和(i+2)%size
			2)文件转换成当前图片的顺序定位问题, 即如何根据当前坐标计算出前一张坐标的位置且不出界, 类似一个循环
					向后定位很简单,只需要 (i+1)%size就能可以实现 下一张的定位, 位置后移 1
					向前定位需要: 只需要( i+n-1)%n 就可以得到前一张的位置,且不出界 , 即位置前移 1 
*/
void Qt5DigitalImageProcessing::on_pushButtonPreImg_clicked()
{
	ImageProcess imgProcess(this);
	int imgNum = _imgsPathList.size();
	if (imgNum>= 3)
	{
		//先定位出当前图片的位置, 即使用公式 (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;
		qDebug() << "_index = " << _index << endl; //测试通过

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
	else if (imgNum == 2)
	{
		//先定位出当前图片的位置, 即使用公式 (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;

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

void Qt5DigitalImageProcessing::on_pushButtonNextImg_clicked()
{
	ImageProcess imgProcess(this);
	int imgNum = _imgsPathList.size();
	if (imgNum >= 3)
	{
		//先定位出当前图片的位置, 向后移x位,只需要 (i+x)%n
		_index = (_index + 1) % imgNum;
		qDebug() << "_index = " << _index << endl; //测试通过

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
	else if (imgNum == 2)
	{
		//先定位出当前图片的位置, 同理
		_index = (_index + 1) % imgNum;

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

//当前图片向左旋转(大Label)
void Qt5DigitalImageProcessing::on_pushButtonTurnLeft_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QMatrix matrix;
		matrix.rotate(-90);
		QImage images(ui.labelShow->pixmap()->toImage()); 
		//pixmap()返回QPixmap对象,QPixmap::toImage()返回QImage对象
		images = images.transformed(matrix, Qt::FastTransformation);
		//图片在选择和显示时已通过ImageProcess::imageCenter()调整过尺寸,不再调整
		ui.labelShow->setPixmap(QPixmap::fromImage(images));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE", QMessageBox::Ok);
	}
}

//当前图片向右旋转
void Qt5DigitalImageProcessing::on_pushButtonTurnRight_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QMatrix matrix;
		matrix.rotate(90);
		QImage images(ui.labelShow->pixmap()->toImage());
		//pixmap()返回QPixmap对象,QPixmap::toImage()返回QImage对象
		images = images.transformed(matrix, Qt::FastTransformation);
		//图片在选择和显示时已通过ImageProcess::imageCenter()调整过尺寸,不再调整
		ui.labelShow->setPixmap(QPixmap::fromImage(images));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE", QMessageBox::Ok);
	}
}

//当前Label图片水平镜像
void Qt5DigitalImageProcessing::on_pushButtonMirrorHorizontal_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QImage images(ui.labelShow->pixmap()->toImage());
		images = images.mirrored(true, false);
		ui.labelShow->setPixmap(QPixmap::fromImage(images));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE", QMessageBox::Ok);
	}
}

//当前Label图片垂直镜像
void Qt5DigitalImageProcessing::on_pushButtonMirrorVertical_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QImage images(ui.labelShow->pixmap()->toImage());
		images = images.mirrored(false, true);
		ui.labelShow->setPixmap(QPixmap::fromImage(images));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE", QMessageBox::Ok);
	}
}
