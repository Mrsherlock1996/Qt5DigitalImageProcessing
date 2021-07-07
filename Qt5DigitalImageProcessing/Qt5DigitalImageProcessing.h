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
#include "ConvertMatQImage.h"
#include <qcheckbox.h>
using namespace cv;

class Qt5DigitalImageProcessing : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qt5DigitalImageProcessing(QWidget *parent = Q_NULLPTR);
	~Qt5DigitalImageProcessing();
	int _index = 0; //ͼƬ����
	int _type = 0; //��Ƶ��������


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

};
