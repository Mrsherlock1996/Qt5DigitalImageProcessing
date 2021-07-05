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

using namespace cv;

class Qt5DigitalImageProcessing : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qt5DigitalImageProcessing(QWidget *parent = Q_NULLPTR);
	~Qt5DigitalImageProcessing();
	int _index = 0; //图片索引
	int _type = 0; //视频操作类型


private:
    Ui::Qt5DigitalImageProcessingClass ui;
	bool _language = true;
	bool _isstart = false;
	QString _originPath;
	QString _videoPath;
	cv::VideoCapture _capture;
	int _playRate;
	int _delay;
	QMessageBox _customMsgBox;	//消息对话框
	QStringList _imgsPathList;	//图像路径列表

private slots:
	void on_pushButtonSelectImage_clicked();
	void on_pushButtonPreImg_clicked();
	void on_pushButtonNextImg_clicked();
	void on_pushButtonTurnLeft_clicked();
	void on_pushButtonTurnRight_clicked();
	void on_pushButtonMirrorHorizontal_clicked();
	void on_pushButtonMirrorVertical_clicked();
	void on_pushButtonSaveImg_clicked();
	void on_checkBoxWaterMark_clicked();

};
