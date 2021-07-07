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
	QImage gray(const QImage*);  //灰度化
	QImage setRGB(const QImage* image, int valueR, int valueG, int valueB);//调整RGB值
	QImage adjustContrast(const QImage* src_image,  int value);//调整对比度
	QImage imageCenter(const QImage* qimage, const QLabel* qLabel); //调整图片比例
	QImage adjustSaturation(const QImage* image, int value);//调整饱和度
	QImage adjustLuminanceAndContrast(const QImage* image, int valueL, int valueC); //调整亮度
	QImage edge(const QImage* image);
	Mat masaike(const Mat* image);
	QImage thresholdImg(const QImage* image, int min,int max);
	
	QImage meanFilter(QImage* image);
	QImage gamma(QImage* image);
};
