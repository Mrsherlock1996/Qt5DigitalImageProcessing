#include "Qt5DigitalImageProcessing.h"
#include <qdebug.h>


Qt5DigitalImageProcessing::Qt5DigitalImageProcessing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	_playRate = 1;
	_delay = 0;
	this->resize(QSize(1440, 720));

	_imgProcess = new ImageProcess; 
	_thread = new QThread;
	_imgProcess->moveToThread(_thread);
	_thread->start();

	//关联发射信号和任务类成员函数
	/* 信号和槽函数重载时,需要static_cast<>()指出具体函数
	connect(		&newspaper,
						static_cast<void (Newspaper:: *)(const QString, const QDate)>(&Newspaper::newPaper),
						&reader,
						static_cast<void (Reader:: *)(const QString, const QDate)>(&Reader::receiveNewspaper)
						);
*/
	connect(this, &Qt5DigitalImageProcessing::sendToGray,
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage*, QImage*)>( &ImageProcess::gray));

	connect(this, &Qt5DigitalImageProcessing::sendToSetRGB, 
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage* , int , int , int , QImage* )>(&ImageProcess::setRGB));

	connect(this, &Qt5DigitalImageProcessing::sendToAdjustContrast,
		_imgProcess, static_cast<void(ImageProcess::*)(const  QImage* , int , QImage* )>(&ImageProcess::adjustContrast));

	connect(this, &Qt5DigitalImageProcessing::sendToImageCenter, 
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage* , const QLabel* , QImage *)>(&ImageProcess::imageCenter));

	connect(this, &Qt5DigitalImageProcessing::sendToAdjustSaturation, 
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage* , int , QImage* )>(&ImageProcess::adjustSaturation));

	connect(this, &Qt5DigitalImageProcessing::sendToAdjustLuminanceAndContrast,
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage* , int , int , QImage* )>(&ImageProcess::adjustLuminanceAndContrast));

	connect(this, &Qt5DigitalImageProcessing::sendToEdge, 
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage* , QImage* )>(&ImageProcess::edge));
	
	connect(this, &Qt5DigitalImageProcessing::sendToThresholdImg,
		_imgProcess, static_cast<void(ImageProcess::*)(const QImage* , int , int , QImage* )>(&ImageProcess::thresholdImg));
	
	connect(this, &Qt5DigitalImageProcessing::sendToMeanFilter,
		_imgProcess, static_cast<void(ImageProcess::*)(QImage*, QImage* )>(&ImageProcess::meanFilter));



	initial();		//初始化各种组件

	QLabel* permanentLabel = new QLabel(this);
	permanentLabel->setObjectName("status");			//创建永久的标签对象status,用于下方代码显示路径等
	permanentLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
	permanentLabel->setText("Welcome to using");

	ui.statusBar->addPermanentWidget(permanentLabel);
	//ui.tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px;background:  transparent; }");
}

Qt5DigitalImageProcessing::~Qt5DigitalImageProcessing()
{
}

/*
选择图片按钮
	功能: 加载图片,大label显示选择图片,小label第一个显示当前图片缩略图,其他小label显示其他图片缩略图
	该槽函数同时通过ui界面的方法关联了QAction对象actionOpen
	且选择图片后使能亮度和对比度滑动条和饱和度滑动条使能
*/
void Qt5DigitalImageProcessing::on_pushButtonSelectImage_clicked()
{
	ImageProcess imgProcess(this);
	QFileDialog fileDlg;
	QString title("selected images");
	QString filter("jpg(*.jpg);;png(*.png);;jpeg(*.jpeg);;all(*.*)");
	fileDlg.resize(QSize(300, 400));	//尝试修改标准对话框初始大小失败,应继承QDialog后封装
	_imgsPathList= QFileDialog::getOpenFileNames(this, title, QDir::currentPath(),filter );
	//选择图片后自动使能亮度,对比度,饱和度,R,G,B滑动条
	(_imgsPathList.size() > 0) ?
		(ui.horizontalSliderLuminance->setDisabled(false),
			ui.horizontalSliderContrast->setDisabled(false),
			ui.horizontalSliderSaturation->setDisabled(false),
			ui.horizontalSliderB->setDisabled(false),
			ui.horizontalSliderG->setDisabled(false),
			ui.horizontalSliderR->setDisabled(false)) :
			(ui.horizontalSliderLuminance->setDisabled(true),
				ui.horizontalSliderContrast->setDisabled(true),
				ui.horizontalSliderSaturation->setDisabled(true),
				ui.horizontalSliderB->setDisabled(true),
				ui.horizontalSliderG->setDisabled(true),
				ui.horizontalSliderR->setDisabled(true));

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
		_originPath = srcDirPath;	//labelShow图片路径保存起来,imageprocess时用
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
		_originPath = srcDirPath;// labelShow图片路径保存起来, imageprocess时用
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
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用
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

//初始化函数,初始化组件状态
void Qt5DigitalImageProcessing::initial()
{
	//默认关闭如下按钮,初始化
//关闭前后张按钮
	ui.pushButtonPreImg->setDisabled(true);
	ui.pushButtonNextImg->setDisabled(true);
	//关闭水印
	ui.checkBoxWaterMark->setCheckState(Qt::Unchecked);
	//关闭二值化区域组件
	ui.checkBoxThreshold->setCheckState(Qt::Unchecked);
	ui.horizontalSliderThrMax->setDisabled(true);
	ui.horizontalSliderThrMin->setDisabled(true);
	ui.labelThreshMaxNum->setDisabled(true);
	ui.labelThreshMinNum->setDisabled(true);

	//初始化亮度对比度滑动条并将其关闭,加载图片后打开
	ui.horizontalSliderLuminance->setRange(0, 255 * 2);  //实际滑动范围为-255,255,调用时valueL-255
	ui.horizontalSliderContrast->setRange(0, 255 * 2);
	ui.horizontalSliderLuminance->setValue(0);
	ui.horizontalSliderContrast->setValue(0);
	ui.horizontalSliderContrast->setDisabled(true);
	ui.horizontalSliderLuminance->setDisabled(true);
	//初始化亮度对比度标签值
	ui.labelLuminanceNum->setNum(0);
	ui.labelContrastNum->setNum(0);

	//初始化饱和度滑动条,并将其关闭,加载图片后打开
	ui.horizontalSliderSaturation->setValue(0);
	ui.horizontalSliderSaturation->setDisabled(true);
	ui.horizontalSliderSaturation->setRange(0, 99);
	//初始化饱和度标签值
	ui.labelSaturationNum->setNum(0);

	//初始化RGB滑动条, 并将其关闭,加载图片后打开
	ui.horizontalSliderB->setValue(0);
	ui.horizontalSliderB->setDisabled(true);
	ui.horizontalSliderG->setValue(0);
	ui.horizontalSliderG->setDisabled(true);
	ui.horizontalSliderR->setValue(0);
	ui.horizontalSliderR->setDisabled(true);
	//初始化RGB标签值
	ui.labelColorBNum->setNum(0);
	ui.labelColorGNum->setNum(0);
	ui.labelColorRNum->setNum(0);
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
/*
void Qt5DigitalImageProcessing::on_pushButtonPreImg_clicked()
{
	//使用子线程处理
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
		//QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		QImage centerImage;

		emit	 sendToImageCenter(&image, ui.labelShow,&centerImage);
	
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //消除图片和label间隙
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用

		//将当前图片展示在小Label中,
		//QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		QImage  other1Image;

		emit	 sendToImageCenter(&image, ui.labelShow, &other1Image);

		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//非主图缩略图显示在小label中,即第2张图
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //该余数就是当前索引值
		QImage image2(image2Path);
		//QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		QImage image2Center;
		emit	 sendToImageCenter(&image, ui.labelShow,& image2Center);

		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);

		//非主图缩略图显示在小label中,即第3张图
		QString image3Path = _imgsPathList.at((_index + 2) % _imgsPathList.size());   //该余数就是当前索引值
		QImage image3(image3Path);
		//QImage image3Center = imgProcess.imageCenter(&image3, ui.labelOther3);
		QImage image3Center;
		emit	 sendToImageCenter(&image, ui.labelShow, &image3Center);

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
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用

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
*/
void Qt5DigitalImageProcessing::on_pushButtonPreImg_clicked()
{
	ImageProcess imgProcess(this);
	int imgNum = _imgsPathList.size();
	if (imgNum >= 3)
	{
		//先定位出当前图片的位置, 即使用公式 (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;
		qDebug() << "_index = " << _index << endl; //测试通过

		QString srcDirPath = _imgsPathList.at(_index);  //获取路径列表中第1个
		QImage image(srcDirPath);

		//调整图片格式并显示在labelShow中
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);

		//emit	 sendToImageCenter(&image, ui.labelShow, &centerImage);

		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //消除图片和label间隙
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用

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
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用

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
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用

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
		_originPath = srcDirPath;//labelShow图片路径保存起来,imageprocess时用

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
旋转镜像的对象都是labelShow的pixmap()中的image,非原图
*/
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


//水印功能暂不指定手动添加水印图片,可以改进
//水印增加预览功能,选中则显示有水印的图片,消除水印只需要切换上下张图片
//保存时水印被选中则保存带水印图片
//水印覆盖目标为labelShow的image非原图
void Qt5DigitalImageProcessing::on_checkBoxWaterMark_clicked()
{

	if (ui.checkBoxWaterMark->isChecked())	//水印按钮选中, 添加水印
	{	
		if (ui.labelShow->pixmap() != nullptr)	//已加载图片
		{
			QImage image = ui.labelShow->pixmap()->toImage();  
			QImage wmImage("images/waterMark.png");//指定水印图片

			int wmWidth = wmImage.width();
			int wmHeight = wmImage.height();
			int r, g, b;
			//从图片左上角0,0点开始将水印图片覆盖到图片中
			for (int y = 0; y < wmHeight; y++)
			{
				for (int x = 0; x < wmWidth; x++)
				{
					QColor color(QColor(wmImage.pixel(x, y)));
					r = color.red();
					g = color.green();
					b = color.blue();
					if (r == 0 && b == 0 && g == 0)
					{
						image.setPixelColor(x, y, qRgb(0, 0, 0));
					}
					else
					{
						image.setPixelColor(x, y, QColor(qRgb(r,g,b)));
					}
				}
			}
			ui.labelShow->setPixmap(QPixmap::fromImage(image));
		}
		else//未加载图片
		{
			QMessageBox::warning(nullptr, "WARNING", "NO IMAGES!", QMessageBox::Ok);
		}
	
	}
	else
	{
		if (ui.labelShow->pixmap() != nullptr)
		{
			//不太方便实现,暂时不实现消除水印功能,仅通过上下张等按钮实现
			//QMessageBox::warning(nullptr, "WARNING", "Dismark image please push 上一张 or 下一张 buttons", QMessageBox::Ok);
		}
		else//未加载图片
		{
			QMessageBox::warning(nullptr, "WARNING", "NO IMAGES!", QMessageBox::Ok);
		}
	}
	
}



//保存当前label图片(需跳出对话框指定路径)
//这里处理的图片也只是label图片,非原始图片图片
void  Qt5DigitalImageProcessing::on_pushButtonSaveImg_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QString savePath;
		QFileDialog file;
		savePath = file.getSaveFileName(this, "Select a path to save image", QDir::currentPath(), "png(*.png);;jpg(*.jpg);;jpeg(*.jpeg);;gif(*.gif);;all(*.*)");
		QImage image = ui.labelShow->pixmap()->toImage();
		bool saveOk;
		saveOk = image.save(savePath);
		if (saveOk)
		{
			//ui.statusBar->showMessage(QString("save image OK at " + savePath));
			QMessageBox::information(this, "SAVE OK", QString("save image OK at \n" + savePath), QMessageBox::Ok);
		}
		else
		{
			//ui.statusBar->showMessage("Sorry, Image save ERROR!");
			QMessageBox::warning(this, "NOT SAVE", "SORRY! Image save ERROR!", QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE!", QMessageBox::Ok);
	}
}
/*Digital Image Processing 
显示原图, 灰度化,边缘检测,均值滤波
gamma变换, 二值化,对比度,饱和度,rgb数值调整,图片比例,亮度调整
*/

//显示原图
void Qt5DigitalImageProcessing::on_pushButtonShowOriginImg_clicked()
{
	if (_originPath != nullptr)
	{
		ImageProcess imgProcess(this);
		QImage img(_originPath);
		img = imgProcess.imageCenter(&img, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(img));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE!", QMessageBox::Ok);
	}
}

//灰度化
void Qt5DigitalImageProcessing::on_pushButtonCvtGray_clicked()
{
	if (_originPath != nullptr)
	{
		ImageProcess imgProcess(this);
		QImage image(_originPath);
		QImage gray = imgProcess.gray(&image);
		gray= imgProcess.imageCenter(&gray, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(gray));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else 
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE!", QMessageBox::Ok);
	}
}

//边缘检测
void Qt5DigitalImageProcessing::on_pushButtonEdgeDetection_clicked()
{
	if (_originPath != nullptr)
	{
		ImageProcess imgProcess(this);
		QImage image(_originPath);
		QImage edgeImg = imgProcess.edge(&image);
		edgeImg = imgProcess.imageCenter(&edgeImg, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(edgeImg));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE!", QMessageBox::Ok);
	}
}
//均值滤波
void Qt5DigitalImageProcessing::on_pushButtonMeanFilter_clicked()
{
	if (_originPath != nullptr)
	{
		ImageProcess imgProcess(this);
		QImage image(_originPath);
		QImage meanFilterImg = imgProcess.meanFilter(&image);
		meanFilterImg = imgProcess.imageCenter(&meanFilterImg, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(meanFilterImg));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE!", QMessageBox::Ok);
	}

}

//二值化开关
//只有当checbox使能时, 才能调节min和max值,否则滑块禁止移动
//当checbox使能时,初始化splider数值为默认值0,范围255
//程序启动时默认关闭
void Qt5DigitalImageProcessing::on_checkBoxThreshold_clicked()
{
	if (!_originPath.isNull()) 
	{
		if (ui.checkBoxThreshold->checkState() == 2)//被选中
		{
			//使能显示标签
			ui.labelThreshMax->setDisabled(false);
			ui.labelThreshMin->setDisabled(false);
			ui.labelThreshMaxNum->setDisabled(false);
			ui.labelThreshMinNum->setDisabled(false);
			//使能滑动条
			ui.horizontalSliderThrMax->setDisabled(false);
			ui.horizontalSliderThrMin->setDisabled(false);
			//初始化显示标签值
			ui.labelThreshMaxNum->setNum(0);
			ui.labelThreshMinNum->setNum(0);
			//初始化滑动条范围
			ui.horizontalSliderThrMax->setValue(0);
			ui.horizontalSliderThrMin->setValue(0);
			ui.horizontalSliderThrMax->setMinimum(1);
			ui.horizontalSliderThrMin->setMinimum(0);
			ui.horizontalSliderThrMax->setMaximum(255);
			ui.horizontalSliderThrMin->setMaximum(254);

		}
		else if (ui.checkBoxThreshold->checkState() == 0)//未被选中
		{
			ui.labelThreshMax->setDisabled(true);
			ui.labelThreshMin->setDisabled(true);
			ui.labelThreshMaxNum->setDisabled(true);
			ui.labelThreshMinNum->setDisabled(true);
			ui.horizontalSliderThrMax->setDisabled(true);
			ui.horizontalSliderThrMin->setDisabled(true);
		}
	}
	else
	{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE!", QMessageBox::Ok);
		ui.checkBoxThreshold->setCheckState(Qt::Unchecked);
	}
	
}

//二值化滑动条
//在二值化checkbox使能状态下,调整对应slider值,同步到ui的数值标签和labelShow
void Qt5DigitalImageProcessing::thresholdNumChange()
{
	int min = ui.horizontalSliderThrMin->value();
	int max = ui.horizontalSliderThrMax->value();
	//opencv二值化
	ImageProcess imgProcess(this);
	QImage img;
	img = imgProcess.imageCenter(&(imgProcess.thresholdImg(
									&QImage(_originPath), min, max)),ui.labelShow);
	//更新ui图片显示和min,max标签值
	ui.labelShow->setPixmap(QPixmap::fromImage(img));
	ui.labelShow->setAlignment(Qt::AlignCenter);
	ui.labelThreshMinNum->setNum(min);
	ui.labelThreshMaxNum->setNum(max);

}

//Label图像修改RGB值,非原图
//rgb值改变时,修改_rgbRecord[i]值,表示需要从ui中获取rgb数值
//_rgbRecord[] 对应值为r,g,b
//rgb数值修改函数响应逻辑:
//		QSlider组件发生改变时,先修改成员数组数值,使之参与后续运算
//		根据成员数组元素值是否参与运算,修改并重置像素rgb数值
//QSliderRGB滑块组件同样是在初始化时关闭,加载图片后使能
void Qt5DigitalImageProcessing::rgbChange()
{
	//设置rgb参与运算标志,信号发射且未设置标记, 将其参与标志使能
	if (QObject::sender() == ui.horizontalSliderR &&_rgbRecord[0]==0)
	{
		_rgbRecord[0] = 1;
	}
	else if (QObject::sender() == ui.horizontalSliderG &&_rgbRecord[1]==0)
	{
		_rgbRecord[1] = 1;
	}
	else if (QObject::sender() == ui.horizontalSliderB&&_rgbRecord[2]==0)
	{
		_rgbRecord[2] = 1;
	}
	//按条件设置rgb增量,并修改标签值
	int redAdd, greenAdd, blueAdd;
	(_rgbRecord[0] = 1) ? (redAdd = ui.horizontalSliderR->value(),
		ui.labelColorRNum->setNum(ui.horizontalSliderR->value())) : redAdd = 0;
	(_rgbRecord[1] = 1) ? (greenAdd = ui.horizontalSliderG->value(),
		ui.labelColorGNum->setNum(ui.horizontalSliderG->value())) : greenAdd = 0;
	(_rgbRecord[2] = 1) ? (blueAdd = ui.horizontalSliderB->value(),
		ui.labelColorBNum->setNum(ui.horizontalSliderB->value())) : blueAdd = 0;
	//计算newRGB数值
	QColor color;
	int red, green, blue;
	QImage image(ui.labelShow->pixmap()->toImage());
	int width = image.width();
	int height = image.height();
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			color = QColor(image.pixel(x, y));
			red = color.red() + redAdd;
			red > 255 ? 255 : red;
			green = color.green() + greenAdd;
			green > 255 ? 255 : green;
			blue = color.blue() + blueAdd;
			blue > 255 ? 255 : blue;
			image.setPixel(x, y, qRgb(red, green, blue));
		}
	}
	ui.labelShow->setPixmap(QPixmap::fromImage(image));
	ui.labelShow->setAlignment(Qt::AlignCenter);
}





//调整原图亮度滑动条
//滑动条使能是在加载图片之后
void Qt5DigitalImageProcessing::adjustLuminanceAndContrast()
{
	ImageProcess imgProcess(this);
	int luminance= ui.horizontalSliderLuminance->value();
	int contrast = ui.horizontalSliderContrast->value();
	QImage img(_originPath);
	//实际为-255,255滑动范围却是0-255*2,因此-255
	img = imgProcess.adjustLuminanceAndContrast(&img, luminance-255, contrast-255);
	img = imgProcess.imageCenter(&img, ui.labelShow);
	ui.labelShow->setPixmap(QPixmap::fromImage(img));
	ui.labelShow->setAlignment(Qt::AlignCenter);
	ui.labelContrastNum->setNum(contrast);
	ui.labelLuminanceNum->setNum(luminance);
}

void Qt5DigitalImageProcessing::adjustSaturation()
{
	ImageProcess imgProcess(this);
	int value = ui.horizontalSliderSaturation->value();
	QImage img(_originPath);
	img = imgProcess.adjustSaturation(&img, value);
	img = imgProcess.imageCenter(&img, ui.labelShow);
	ui.labelShow->setPixmap(QPixmap::fromImage(img));
	ui.labelShow->setAlignment(Qt::AlignCenter);
	ui.labelSaturationNum->setNum(value);
}


