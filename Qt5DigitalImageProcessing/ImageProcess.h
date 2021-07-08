#pragma once

#include <QObject>
#include <opencv.hpp>
#include <opencv_modules.hpp>
#include "qimage.h"
#include <QImage>
#include <QLabel>

#include "ConvertMatQImage.h"
using namespace cv;

class ImageProcess : public QObject
{
	Q_OBJECT

public:
	ImageProcess(QObject *parent = nullptr); 
	//����ָ��nullptr�������ʹ��Ĭ�Ϲ��캯������ʱ����

	~ImageProcess();
public:
	QImage gray(const QImage*);  //�ҶȻ�
	QImage setRGB(const QImage* image, int valueR, int valueG, int valueB);//����RGBֵ
	QImage adjustContrast(const QImage* src_image,  int value);//�����Աȶ�
	QImage imageCenter(const QImage* qimage, const QLabel* qLabel); //����ͼƬ����
	QImage adjustSaturation(const QImage* image, int value);//�������Ͷ�
	QImage adjustLuminanceAndContrast(const QImage* image, int valueL, int valueC); //�������ȶԱȶ�
	QImage edge(const QImage* image);
	QImage thresholdImg(const QImage* image, int min,int max);
	QImage meanFilter(QImage* image);

	//���̷߳����ź�ʱʹ��
	void gray(const QImage*, QImage* result);  //�ҶȻ�
	void setRGB(const QImage* image, int valueR, int valueG, int valueB, QImage* result);//����RGBֵ
	void adjustContrast(const QImage* src_image, int value, QImage* result);//�����Աȶ�
	void imageCenter(const QImage* qimage, const QLabel* qLabel, QImage* result); //����ͼƬ����
	void adjustSaturation(const QImage* image, int value, QImage* result);//�������Ͷ�
	void adjustLuminanceAndContrast(const QImage* image, int valueL, int valueC, QImage* result); //�������ȶԱȶ�
	void edge(const QImage* image, QImage* result);
	void thresholdImg(const QImage* image, int min, int max, QImage* result);
	void meanFilter(QImage* image, QImage* result);

signals:
	void sendResult(QImage);
};
