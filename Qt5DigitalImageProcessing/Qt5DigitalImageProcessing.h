#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Qt5DigitalImageProcessing.h"
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QImage>
#include <qmessagebox.h>
#include <opencv.hpp>
#include <qfiledialog.h>
#include "ImageProcess.h"
#include <qcheckbox.h>
using namespace cv;
#include <qthread.h>

class Qt5DigitalImageProcessing : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qt5DigitalImageProcessing(QWidget *parent = Q_NULLPTR);
	~Qt5DigitalImageProcessing();
	int _index = 0; //ͼƬ����
	int _type = 0; //��Ƶ��������
	QThread* _thread;
	ImageProcess* _imgProcess;

private:
    Ui::Qt5DigitalImageProcessingClass ui;

	bool _language = true;
	bool _isstart = false;
	QString _originPath;	//ui.labelShow�е�ͼƬ��ԭʼ·��
	QString _videoPath;
	cv::VideoCapture _capture;
	int _playRate;
	int _delay;
	QMessageBox _customMsgBox;	//��Ϣ�Ի���
	QStringList _imgsPathList;	//ͼ��·���б�
	void initial();	//�����ʼ������
	int _rgbRecord[3] = { 0,0,0 }; 
	//�ֱ��Ӧrgb����λ��,ui�е�rgbֵ�޸�ʱ,����[i]ֵ��Ϊ1,Ϊ0ʱ��ֵ������rgb���ñ��ʽ�ļ���
	//�ó�Աֻ������rgb����slider�������,һ������,���涼Ĭ�ϸ�slider��ֵ����������
	//��QObject::Sender()������������źŷ�������

private slots:
	//on_�����ۺ���ʽ�Զ������źŲ�
	void on_pushButtonSelectImage_clicked(); //ѡ��ͼƬ
	void on_pushButtonPreImg_clicked();			//��һ��
	void on_pushButtonNextImg_clicked();		//��һ��
	void on_pushButtonTurnLeft_clicked();		//����ת
	void on_pushButtonTurnRight_clicked();		//����ת
	void on_pushButtonMirrorHorizontal_clicked();		//ˮƽ����
	void on_pushButtonMirrorVertical_clicked();		//��ֱ����
	void on_pushButtonSaveImg_clicked();			//����ͼƬ
	void on_checkBoxWaterMark_clicked();		//ˮӡ
	void on_pushButtonCvtGray_clicked();		//�ҶȻ�
	void on_pushButtonShowOriginImg_clicked();		//��ʾԭͼ
	void on_pushButtonEdgeDetection_clicked();			//��Ե���
	void on_pushButtonMeanFilter_clicked();			//��ֵ�˲�
	void on_checkBoxThreshold_clicked();			//��ֵ������
	void adjustLuminanceAndContrast();		//�������ȶԱȶ�
	void adjustSaturation();			//�������Ͷ�

	//ui��������źŲ�
	void	thresholdNumChange();		//��ֵ��
	void rgbChange();

signals:
	//ÿ�ζ������ͼƬ��ַ�����ȥ,ʹ������ֱ���޸ĸ�����QImage����, �������Լ���һ�η����һ��connect
	void sendToGray(const QImage* s_image, QImage* result);  //�ҶȻ�
	void sendToSetRGB(const QImage* s_image, int s_valueR, int s_valueG, int s_valueB, QImage* result);//����RGBֵ
	void sendToAdjustContrast(const  QImage* s_src_image, int s_value, QImage* result);//�����Աȶ�
	void sendToImageCenter( const QImage* s_qimage, const QLabel* s_qLabel, QImage* result); //����ͼƬ����
	void sendToAdjustSaturation(const QImage* s_image, int s_value, QImage* result);//�������Ͷ�
	void sendToAdjustLuminanceAndContrast(const QImage* s_image, int s_valueL, int s_valueC, QImage* result); //�������ȶԱȶ�
	void sendToEdge(const QImage* s_image, QImage* result);
	void sendToThresholdImg(const QImage* s_image, int s_min, int s_max, QImage* result);
	void sendToMeanFilter(QImage*s_image, QImage* result);


};
