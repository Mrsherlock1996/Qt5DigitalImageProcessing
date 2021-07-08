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
	//必须指出nullptr否则后续使用默认构造函数创建时报错

	~ImageProcess();
public:
	QImage gray(const QImage*);  //灰度化
	QImage setRGB(const QImage* image, int valueR, int valueG, int valueB);//调整RGB值
	QImage adjustContrast(const QImage* src_image,  int value);//调整对比度
	QImage imageCenter(const QImage* qimage, const QLabel* qLabel); //调整图片比例
	QImage adjustSaturation(const QImage* image, int value);//调整饱和度
	QImage adjustLuminanceAndContrast(const QImage* image, int valueL, int valueC); //调整亮度对比度
	QImage edge(const QImage* image);
	QImage thresholdImg(const QImage* image, int min,int max);
	QImage meanFilter(QImage* image);

	//多线程发射信号时使用
	void gray(const QImage*, QImage* result);  //灰度化
	void setRGB(const QImage* image, int valueR, int valueG, int valueB, QImage* result);//调整RGB值
	void adjustContrast(const QImage* src_image, int value, QImage* result);//调整对比度
	void imageCenter(const QImage* qimage, const QLabel* qLabel, QImage* result); //调整图片比例
	void adjustSaturation(const QImage* image, int value, QImage* result);//调整饱和度
	void adjustLuminanceAndContrast(const QImage* image, int valueL, int valueC, QImage* result); //调整亮度对比度
	void edge(const QImage* image, QImage* result);
	void thresholdImg(const QImage* image, int min, int max, QImage* result);
	void meanFilter(QImage* image, QImage* result);

signals:
	void sendResult(QImage);
};
