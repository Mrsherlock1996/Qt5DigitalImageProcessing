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
	ImageProcess(QObject *parent);
	~ImageProcess();
public:
	void showASCII();
	QImage gray(const QImage*);  //�ҶȻ�
	QImage setRGB(const QImage* image, int valueR, int valueG, int valueB);//����RGBֵ
	QImage adjustContrast(const QImage* src_image,  int value);//�����Աȶ�
	QImage imageCenter(const QImage* qimage, const QLabel* qLabel); //����ͼƬ����
	QImage adjustSaturation(const QImage* image, int value);//�������Ͷ�
	QImage adjustLuminanceAndContrast(const QImage* image, int valueL, int valueC); //��������
	QImage edge(const QImage* image);
	Mat masaike(const Mat* image);
	QImage thresholdImg(const QImage* image, int min,int max);
	
	QImage meanFilter(QImage* image);
	QImage gamma(QImage* image);
};
