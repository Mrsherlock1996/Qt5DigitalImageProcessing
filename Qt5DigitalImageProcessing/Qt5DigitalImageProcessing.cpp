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

	//���������źź��������Ա����
	/* �źźͲۺ�������ʱ,��Ҫstatic_cast<>()ָ�����庯��
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



	initial();		//��ʼ���������

	QLabel* permanentLabel = new QLabel(this);
	permanentLabel->setObjectName("status");			//�������õı�ǩ����status,�����·�������ʾ·����
	permanentLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
	permanentLabel->setText("Welcome to using");

	ui.statusBar->addPermanentWidget(permanentLabel);
	//ui.tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px;background:  transparent; }");
}

Qt5DigitalImageProcessing::~Qt5DigitalImageProcessing()
{
}

/*
ѡ��ͼƬ��ť
	����: ����ͼƬ,��label��ʾѡ��ͼƬ,Сlabel��һ����ʾ��ǰͼƬ����ͼ,����Сlabel��ʾ����ͼƬ����ͼ
	�òۺ���ͬʱͨ��ui����ķ���������QAction����actionOpen
	��ѡ��ͼƬ��ʹ�����ȺͶԱȶȻ������ͱ��ͶȻ�����ʹ��
*/
void Qt5DigitalImageProcessing::on_pushButtonSelectImage_clicked()
{
	ImageProcess imgProcess(this);
	QFileDialog fileDlg;
	QString title("selected images");
	QString filter("jpg(*.jpg);;png(*.png);;jpeg(*.jpeg);;all(*.*)");
	fileDlg.resize(QSize(300, 400));	//�����޸ı�׼�Ի����ʼ��Сʧ��,Ӧ�̳�QDialog���װ
	_imgsPathList= QFileDialog::getOpenFileNames(this, title, QDir::currentPath(),filter );
	//ѡ��ͼƬ���Զ�ʹ������,�Աȶ�,���Ͷ�,R,G,B������
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

//����ͼƬ��������3ʱ:����ͼƬ,��ʹСlabel�ɼ�
	if (_imgsPathList.size() >= 3 )		
	{
		//�����һ��ͼƬ,չʾ�ڴ�Label��
		_index = 0;
		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;	//labelShowͼƬ·����������,imageprocessʱ��
		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����3��ͼ
		QString image3Path = _imgsPathList.at((_index + 2) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image3(image3Path);
		QImage image3Center = imgProcess.imageCenter(&image3, ui.labelOther3);
		ui.labelOther3->setPixmap(QPixmap::fromImage(image3Center));
		ui.labelOther3->setAlignment(Qt::AlignCenter);

		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(true);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
	else if (_imgsPathList.size() == 1)
	{
		//�����һ��ͼƬ,չʾ�ڴ�Label��
		_index = 0;
		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;// labelShowͼƬ·����������, imageprocessʱ��
		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����һ�ź���һ�Ű�ť���ܹر�,������С��ǩ
		ui.pushButtonNextImg->setDisabled(true);
		ui.pushButtonPreImg->setDisabled(true);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(false);
		ui.labelOther3->setVisible(false);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
	else if (_imgsPathList.size() ==2 )
	{
		// �����һ��ͼƬ, չʾ�ڴ�Label��
			_index = 0;
		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);


		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(false);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
}

//��ʼ������,��ʼ�����״̬
void Qt5DigitalImageProcessing::initial()
{
	//Ĭ�Ϲر����°�ť,��ʼ��
//�ر�ǰ���Ű�ť
	ui.pushButtonPreImg->setDisabled(true);
	ui.pushButtonNextImg->setDisabled(true);
	//�ر�ˮӡ
	ui.checkBoxWaterMark->setCheckState(Qt::Unchecked);
	//�رն�ֵ���������
	ui.checkBoxThreshold->setCheckState(Qt::Unchecked);
	ui.horizontalSliderThrMax->setDisabled(true);
	ui.horizontalSliderThrMin->setDisabled(true);
	ui.labelThreshMaxNum->setDisabled(true);
	ui.labelThreshMinNum->setDisabled(true);

	//��ʼ�����ȶԱȶȻ�����������ر�,����ͼƬ���
	ui.horizontalSliderLuminance->setRange(0, 255 * 2);  //ʵ�ʻ�����ΧΪ-255,255,����ʱvalueL-255
	ui.horizontalSliderContrast->setRange(0, 255 * 2);
	ui.horizontalSliderLuminance->setValue(0);
	ui.horizontalSliderContrast->setValue(0);
	ui.horizontalSliderContrast->setDisabled(true);
	ui.horizontalSliderLuminance->setDisabled(true);
	//��ʼ�����ȶԱȶȱ�ǩֵ
	ui.labelLuminanceNum->setNum(0);
	ui.labelContrastNum->setNum(0);

	//��ʼ�����ͶȻ�����,������ر�,����ͼƬ���
	ui.horizontalSliderSaturation->setValue(0);
	ui.horizontalSliderSaturation->setDisabled(true);
	ui.horizontalSliderSaturation->setRange(0, 99);
	//��ʼ�����Ͷȱ�ǩֵ
	ui.labelSaturationNum->setNum(0);

	//��ʼ��RGB������, ������ر�,����ͼƬ���
	ui.horizontalSliderB->setValue(0);
	ui.horizontalSliderB->setDisabled(true);
	ui.horizontalSliderG->setValue(0);
	ui.horizontalSliderG->setDisabled(true);
	ui.horizontalSliderR->setValue(0);
	ui.horizontalSliderR->setDisabled(true);
	//��ʼ��RGB��ǩֵ
	ui.labelColorBNum->setNum(0);
	ui.labelColorGNum->setNum(0);
	ui.labelColorRNum->setNum(0);
}

/*
��һ��ͼƬ��ť
	����:���ó�Ա_index����ͼƬλ����Ϣ
	ʵ���㷨: 
				%������, ʹ��%ʵ�� i ��0-n֮���ѭ�� 	i = (i++)%n, ���� i from 0 to n-1
			1) Сlabel��ͼƬ��ʾ����:
					Сlabel�ǽ�����i��ͼƬ�ļ�ѭ��������� i+1 ,��λ����ǰ��ͼƬi, Ȼ���i��Ϊ��˳��ĵ�һ��,Сlabel��˳�����(i+1)%size��(i+2)%size
			2)�ļ�ת���ɵ�ǰͼƬ��˳��λ����, ����θ��ݵ�ǰ��������ǰһ�������λ���Ҳ�����, ����һ��ѭ��
					���λ�ܼ�,ֻ��Ҫ (i+1)%size���ܿ���ʵ�� ��һ�ŵĶ�λ, λ�ú��� 1
					��ǰ��λ��Ҫ: ֻ��Ҫ( i+n-1)%n �Ϳ��Եõ�ǰһ�ŵ�λ��,�Ҳ����� , ��λ��ǰ�� 1 
*/
/*
void Qt5DigitalImageProcessing::on_pushButtonPreImg_clicked()
{
	//ʹ�����̴߳���
	ImageProcess imgProcess(this);
	int imgNum = _imgsPathList.size();
	if (imgNum>= 3)
	{
		//�ȶ�λ����ǰͼƬ��λ��, ��ʹ�ù�ʽ (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;
		qDebug() << "_index = " << _index << endl; //����ͨ��

		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);

		//����ͼƬ��ʽ����ʾ��labelShow��
		//QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		QImage centerImage;

		emit	 sendToImageCenter(&image, ui.labelShow,&centerImage);
	
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��

		//����ǰͼƬչʾ��СLabel��,
		//QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		QImage  other1Image;

		emit	 sendToImageCenter(&image, ui.labelShow, &other1Image);

		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		//QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		QImage image2Center;
		emit	 sendToImageCenter(&image, ui.labelShow,& image2Center);

		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����3��ͼ
		QString image3Path = _imgsPathList.at((_index + 2) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image3(image3Path);
		//QImage image3Center = imgProcess.imageCenter(&image3, ui.labelOther3);
		QImage image3Center;
		emit	 sendToImageCenter(&image, ui.labelShow, &image3Center);

		ui.labelOther3->setPixmap(QPixmap::fromImage(image3Center));
		ui.labelOther3->setAlignment(Qt::AlignCenter);

		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(true);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
	else if (imgNum == 2)
	{
		//�ȶ�λ����ǰͼƬ��λ��, ��ʹ�ù�ʽ (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;

		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��

		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);


		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(false);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
}
*/
void Qt5DigitalImageProcessing::on_pushButtonPreImg_clicked()
{
	ImageProcess imgProcess(this);
	int imgNum = _imgsPathList.size();
	if (imgNum >= 3)
	{
		//�ȶ�λ����ǰͼƬ��λ��, ��ʹ�ù�ʽ (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;
		qDebug() << "_index = " << _index << endl; //����ͨ��

		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);

		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);

		//emit	 sendToImageCenter(&image, ui.labelShow, &centerImage);

		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��

		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);


		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);

		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����3��ͼ
		QString image3Path = _imgsPathList.at((_index + 2) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image3(image3Path);
		QImage image3Center = imgProcess.imageCenter(&image3, ui.labelOther3);

		ui.labelOther3->setPixmap(QPixmap::fromImage(image3Center));
		ui.labelOther3->setAlignment(Qt::AlignCenter);

		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(true);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
	else if (imgNum == 2)
	{
		//�ȶ�λ����ǰͼƬ��λ��, ��ʹ�ù�ʽ (index+n-1)%n
		_index = (_index + imgNum - 1) % imgNum;

		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��

		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);


		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(false);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
}

void Qt5DigitalImageProcessing::on_pushButtonNextImg_clicked()
{
	ImageProcess imgProcess(this);
	int imgNum = _imgsPathList.size();
	if (imgNum >= 3)
	{
		//�ȶ�λ����ǰͼƬ��λ��, �����xλ,ֻ��Ҫ (i+x)%n
		_index = (_index + 1) % imgNum;
		qDebug() << "_index = " << _index << endl; //����ͨ��

		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��

		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����3��ͼ
		QString image3Path = _imgsPathList.at((_index + 2) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image3(image3Path);
		QImage image3Center = imgProcess.imageCenter(&image3, ui.labelOther3);
		ui.labelOther3->setPixmap(QPixmap::fromImage(image3Center));
		ui.labelOther3->setAlignment(Qt::AlignCenter);

		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(true);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
	else if (imgNum == 2)
	{
		//�ȶ�λ����ǰͼƬ��λ��, ͬ��
		_index = (_index + 1) % imgNum;

		QString srcDirPath = _imgsPathList.at(_index);  //��ȡ·���б��е�1��
		QImage image(srcDirPath);
		//����ͼƬ��ʽ����ʾ��labelShow��
		QImage centerImage = imgProcess.imageCenter(&image, ui.labelShow);
		ui.labelShow->setPixmap(QPixmap::fromImage(centerImage));
		ui.labelShow->setAlignment(Qt::AlignCenter); //����ͼƬ��label��϶
		_originPath = srcDirPath;//labelShowͼƬ·����������,imageprocessʱ��

		//����ǰͼƬչʾ��СLabel��,
		QImage  other1Image = imgProcess.imageCenter(&image, ui.labelOther1);
		ui.labelOther1->setPixmap(QPixmap::fromImage(other1Image));
		ui.labelOther1->setAlignment(Qt::AlignCenter);

		//����ͼ����ͼ��ʾ��Сlabel��,����2��ͼ
		QString image2Path = _imgsPathList.at((_index + 1) % _imgsPathList.size());   //���������ǵ�ǰ����ֵ
		QImage image2(image2Path);
		QImage image2Center = imgProcess.imageCenter(&image2, ui.labelOther2);
		ui.labelOther2->setPixmap(QPixmap::fromImage(image2Center));
		ui.labelOther2->setAlignment(Qt::AlignCenter);


		//����һ�ź���һ�Ű�ťʹ��,����ʾС��ǩ
		ui.pushButtonNextImg->setDisabled(false);
		ui.pushButtonPreImg->setDisabled(false);
		ui.labelOther1->setVisible(true);
		ui.labelOther2->setVisible(true);
		ui.labelOther3->setVisible(false);


		//״̬����ʾ·��
		QLabel* showImgPath = ui.statusBar->findChild<QLabel*>("status");	//��ǩ����status�ڹ��캯��������
		showImgPath->setText(srcDirPath);	//��ʾ��һ�ż���ͼƬ��ַ
	}
}


/*
��ת����Ķ�����labelShow��pixmap()�е�image,��ԭͼ
*/
//��ǰͼƬ������ת(��Label)
void Qt5DigitalImageProcessing::on_pushButtonTurnLeft_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QMatrix matrix;
		matrix.rotate(-90);
		QImage images(ui.labelShow->pixmap()->toImage()); 
		//pixmap()����QPixmap����,QPixmap::toImage()����QImage����
		images = images.transformed(matrix, Qt::FastTransformation);
		//ͼƬ��ѡ�����ʾʱ��ͨ��ImageProcess::imageCenter()�������ߴ�,���ٵ���
		ui.labelShow->setPixmap(QPixmap::fromImage(images));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else{
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE", QMessageBox::Ok);
	}
}

//��ǰͼƬ������ת
void Qt5DigitalImageProcessing::on_pushButtonTurnRight_clicked()
{
	if (ui.labelShow->pixmap() != nullptr)
	{
		QMatrix matrix;
		matrix.rotate(90);
		QImage images(ui.labelShow->pixmap()->toImage());
		//pixmap()����QPixmap����,QPixmap::toImage()����QImage����
		images = images.transformed(matrix, Qt::FastTransformation);
		//ͼƬ��ѡ�����ʾʱ��ͨ��ImageProcess::imageCenter()�������ߴ�,���ٵ���
		ui.labelShow->setPixmap(QPixmap::fromImage(images));
		ui.labelShow->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "WARNING", "NO IMAGE", QMessageBox::Ok);
	}
}

//��ǰLabelͼƬˮƽ����
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

//��ǰLabelͼƬ��ֱ����
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


//ˮӡ�����ݲ�ָ���ֶ����ˮӡͼƬ,���ԸĽ�
//ˮӡ����Ԥ������,ѡ������ʾ��ˮӡ��ͼƬ,����ˮӡֻ��Ҫ�л�������ͼƬ
//����ʱˮӡ��ѡ���򱣴��ˮӡͼƬ
//ˮӡ����Ŀ��ΪlabelShow��image��ԭͼ
void Qt5DigitalImageProcessing::on_checkBoxWaterMark_clicked()
{

	if (ui.checkBoxWaterMark->isChecked())	//ˮӡ��ťѡ��, ���ˮӡ
	{	
		if (ui.labelShow->pixmap() != nullptr)	//�Ѽ���ͼƬ
		{
			QImage image = ui.labelShow->pixmap()->toImage();  
			QImage wmImage("images/waterMark.png");//ָ��ˮӡͼƬ

			int wmWidth = wmImage.width();
			int wmHeight = wmImage.height();
			int r, g, b;
			//��ͼƬ���Ͻ�0,0�㿪ʼ��ˮӡͼƬ���ǵ�ͼƬ��
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
		else//δ����ͼƬ
		{
			QMessageBox::warning(nullptr, "WARNING", "NO IMAGES!", QMessageBox::Ok);
		}
	
	}
	else
	{
		if (ui.labelShow->pixmap() != nullptr)
		{
			//��̫����ʵ��,��ʱ��ʵ������ˮӡ����,��ͨ�������ŵȰ�ťʵ��
			//QMessageBox::warning(nullptr, "WARNING", "Dismark image please push ��һ�� or ��һ�� buttons", QMessageBox::Ok);
		}
		else//δ����ͼƬ
		{
			QMessageBox::warning(nullptr, "WARNING", "NO IMAGES!", QMessageBox::Ok);
		}
	}
	
}



//���浱ǰlabelͼƬ(�������Ի���ָ��·��)
//���ﴦ���ͼƬҲֻ��labelͼƬ,��ԭʼͼƬͼƬ
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
��ʾԭͼ, �ҶȻ�,��Ե���,��ֵ�˲�
gamma�任, ��ֵ��,�Աȶ�,���Ͷ�,rgb��ֵ����,ͼƬ����,���ȵ���
*/

//��ʾԭͼ
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

//�ҶȻ�
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

//��Ե���
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
//��ֵ�˲�
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

//��ֵ������
//ֻ�е�checboxʹ��ʱ, ���ܵ���min��maxֵ,���򻬿��ֹ�ƶ�
//��checboxʹ��ʱ,��ʼ��splider��ֵΪĬ��ֵ0,��Χ255
//��������ʱĬ�Ϲر�
void Qt5DigitalImageProcessing::on_checkBoxThreshold_clicked()
{
	if (!_originPath.isNull()) 
	{
		if (ui.checkBoxThreshold->checkState() == 2)//��ѡ��
		{
			//ʹ����ʾ��ǩ
			ui.labelThreshMax->setDisabled(false);
			ui.labelThreshMin->setDisabled(false);
			ui.labelThreshMaxNum->setDisabled(false);
			ui.labelThreshMinNum->setDisabled(false);
			//ʹ�ܻ�����
			ui.horizontalSliderThrMax->setDisabled(false);
			ui.horizontalSliderThrMin->setDisabled(false);
			//��ʼ����ʾ��ǩֵ
			ui.labelThreshMaxNum->setNum(0);
			ui.labelThreshMinNum->setNum(0);
			//��ʼ����������Χ
			ui.horizontalSliderThrMax->setValue(0);
			ui.horizontalSliderThrMin->setValue(0);
			ui.horizontalSliderThrMax->setMinimum(1);
			ui.horizontalSliderThrMin->setMinimum(0);
			ui.horizontalSliderThrMax->setMaximum(255);
			ui.horizontalSliderThrMin->setMaximum(254);

		}
		else if (ui.checkBoxThreshold->checkState() == 0)//δ��ѡ��
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

//��ֵ��������
//�ڶ�ֵ��checkboxʹ��״̬��,������Ӧsliderֵ,ͬ����ui����ֵ��ǩ��labelShow
void Qt5DigitalImageProcessing::thresholdNumChange()
{
	int min = ui.horizontalSliderThrMin->value();
	int max = ui.horizontalSliderThrMax->value();
	//opencv��ֵ��
	ImageProcess imgProcess(this);
	QImage img;
	img = imgProcess.imageCenter(&(imgProcess.thresholdImg(
									&QImage(_originPath), min, max)),ui.labelShow);
	//����uiͼƬ��ʾ��min,max��ǩֵ
	ui.labelShow->setPixmap(QPixmap::fromImage(img));
	ui.labelShow->setAlignment(Qt::AlignCenter);
	ui.labelThreshMinNum->setNum(min);
	ui.labelThreshMaxNum->setNum(max);

}

//Labelͼ���޸�RGBֵ,��ԭͼ
//rgbֵ�ı�ʱ,�޸�_rgbRecord[i]ֵ,��ʾ��Ҫ��ui�л�ȡrgb��ֵ
//_rgbRecord[] ��ӦֵΪr,g,b
//rgb��ֵ�޸ĺ�����Ӧ�߼�:
//		QSlider��������ı�ʱ,���޸ĳ�Ա������ֵ,ʹ֮�����������
//		���ݳ�Ա����Ԫ��ֵ�Ƿ��������,�޸Ĳ���������rgb��ֵ
//QSliderRGB�������ͬ�����ڳ�ʼ��ʱ�ر�,����ͼƬ��ʹ��
void Qt5DigitalImageProcessing::rgbChange()
{
	//����rgb���������־,�źŷ�����δ���ñ��, ��������־ʹ��
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
	//����������rgb����,���޸ı�ǩֵ
	int redAdd, greenAdd, blueAdd;
	(_rgbRecord[0] = 1) ? (redAdd = ui.horizontalSliderR->value(),
		ui.labelColorRNum->setNum(ui.horizontalSliderR->value())) : redAdd = 0;
	(_rgbRecord[1] = 1) ? (greenAdd = ui.horizontalSliderG->value(),
		ui.labelColorGNum->setNum(ui.horizontalSliderG->value())) : greenAdd = 0;
	(_rgbRecord[2] = 1) ? (blueAdd = ui.horizontalSliderB->value(),
		ui.labelColorBNum->setNum(ui.horizontalSliderB->value())) : blueAdd = 0;
	//����newRGB��ֵ
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





//����ԭͼ���Ȼ�����
//������ʹ�����ڼ���ͼƬ֮��
void Qt5DigitalImageProcessing::adjustLuminanceAndContrast()
{
	ImageProcess imgProcess(this);
	int luminance= ui.horizontalSliderLuminance->value();
	int contrast = ui.horizontalSliderContrast->value();
	QImage img(_originPath);
	//ʵ��Ϊ-255,255������Χȴ��0-255*2,���-255
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


