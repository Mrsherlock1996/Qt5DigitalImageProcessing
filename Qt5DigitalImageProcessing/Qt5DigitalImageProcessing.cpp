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
	_customMsgBox.setText("��ӭʹ�á��๦��ͼ����Ƶ�����������������м򵥵�ͼ�����Ƶ�����ܡ�\n"
		"ͼ���ܰ�����ѡ�򿪡���ת�����񡢻ҶȻ�����ֵ�˲���"
		"��Ե��⡢ԭͼ���ϡ�٤���⡢��ֵ����ɫ�ʵ��������ȵ������Աȶȵ��������Ͷȵ����ȹ��ܡ�\n"
		"��Ƶ���ܰ�����ͣ�����š����������ҶȻ�����Ե��⡢ƽ������ֵ�����ֲ������ˡ����ŵȹ��ܡ�\n");

	ui.statusBar->showMessage("Welcome", 2000);

	QLabel* permanentLabel = new QLabel(this);
	permanentLabel->setObjectName("status");			//�������õı�ǩ����status,�����·�������ʾ·����
	permanentLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
	permanentLabel->setText("Welcome to using");

	ui.statusBar->addPermanentWidget(permanentLabel);
	ui.tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px;background:  transparent; }");

}

Qt5DigitalImageProcessing::~Qt5DigitalImageProcessing()
{
}

/*
ѡ��ͼƬ��ť
	����: ����ͼƬ,��label��ʾѡ��ͼƬ,Сlabel��һ����ʾ��ǰͼƬ����ͼ,����Сlabel��ʾ����ͼƬ����ͼ
	�òۺ���ͬʱͨ��ui����ķ���������QAction����actionOpen
*/
void Qt5DigitalImageProcessing::on_pushButtonSelectImage_clicked()
{
	ImageProcess imgProcess(this);
	QFileDialog fileDlg;
	QString title("selected images");
	QString filter("jpg(*.jpg);;png(*.png);;jpeg(*.jpeg);;all(*.*)");
	fileDlg.resize(QSize(300, 400));	//�����޸ı�׼�Ի����ʼ��Сʧ��,Ӧ�̳�QDialog���װ
	_imgsPathList= QFileDialog::getOpenFileNames(this, title, QDir::currentPath(),filter );
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
void Qt5DigitalImageProcessing::on_pushButtonPreImg_clicked()
{
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
