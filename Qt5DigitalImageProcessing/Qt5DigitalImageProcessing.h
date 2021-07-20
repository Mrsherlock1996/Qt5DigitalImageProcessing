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
	int _index = 0; //图片索引
	int _type = 0; //视频操作类型
	QThread* _thread;
	ImageProcess* _imgProcess;

private:
    Ui::Qt5DigitalImageProcessingClass ui;

	bool _language = true;
	bool _isstart = false;
	QString _originPath;	//ui.labelShow中的图片的原始路径
	QString _videoPath;
	cv::VideoCapture _capture;
	int _playRate;
	int _delay;
	QMessageBox _customMsgBox;	//消息对话框
	QStringList _imgsPathList;	//图像路径列表
	void initial();	//组件初始化函数
	int _rgbRecord[3] = { 0,0,0 }; 
	//分别对应rgb三个位置,ui中的rgb值修改时,将其[i]值置为1,为0时该值不参与rgb重置表达式的计算
	//该成员只是启动rgb三个slider参与计算,一旦启动,后面都默认该slider数值均参与运算
	//用QObject::Sender()这个函数返回信号发射主体

private slots:
	//on_命名槽函数式自动关联信号槽
	void on_pushButtonSelectImage_clicked(); //选择图片
	void on_pushButtonPreImg_clicked();			//上一张
	void on_pushButtonNextImg_clicked();		//下一张
	void on_pushButtonTurnLeft_clicked();		//左旋转
	void on_pushButtonTurnRight_clicked();		//右旋转
	void on_pushButtonMirrorHorizontal_clicked();		//水平镜像
	void on_pushButtonMirrorVertical_clicked();		//垂直镜像
	void on_pushButtonSaveImg_clicked();			//保存图片
	void on_checkBoxWaterMark_clicked();		//水印
	void on_pushButtonCvtGray_clicked();		//灰度化
	void on_pushButtonShowOriginImg_clicked();		//显示原图
	void on_pushButtonEdgeDetection_clicked();			//边缘检测
	void on_pushButtonMeanFilter_clicked();			//均值滤波
	void on_checkBoxThreshold_clicked();			//二值化开关
	void adjustLuminanceAndContrast();		//调节亮度对比度
	void adjustSaturation();			//调整饱和度

	//ui界面关联信号槽
	void	thresholdNumChange();		//二值化
	void rgbChange();

signals:
	//每次都将结果图片地址发射出去,使任务类直接修改给出的QImage对象, 这样可以减少一次发射和一次connect
	void sendToGray(const QImage* s_image, QImage* result);  //灰度化
	void sendToSetRGB(const QImage* s_image, int s_valueR, int s_valueG, int s_valueB, QImage* result);//调整RGB值
	void sendToAdjustContrast(const  QImage* s_src_image, int s_value, QImage* result);//调整对比度
	void sendToImageCenter( const QImage* s_qimage, const QLabel* s_qLabel, QImage* result); //调整图片比例
	void sendToAdjustSaturation(const QImage* s_image, int s_value, QImage* result);//调整饱和度
	void sendToAdjustLuminanceAndContrast(const QImage* s_image, int s_valueL, int s_valueC, QImage* result); //调整亮度对比度
	void sendToEdge(const QImage* s_image, QImage* result);
	void sendToThresholdImg(const QImage* s_image, int s_min, int s_max, QImage* result);
	void sendToMeanFilter(QImage*s_image, QImage* result);


};
