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
	int _index = 0; //图片index
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
	QMessageBox _customMsgBox;
	QStringList _imgsPathList;

private slots:
	void on_pushButtonSelectImage_clicked();
};
