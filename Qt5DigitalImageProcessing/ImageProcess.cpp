#include "ImageProcess.h"

ImageProcess::ImageProcess(QObject *parent)
	: QObject(parent)
{
}

ImageProcess::~ImageProcess()
{
}

//QImage�ҶȻ�,�㷨:rgb��ɫƽ����
QImage ImageProcess::gray(const QImage * image)
{
	QImage newImage = image->convertToFormat(QImage::Format_ARGB32); //QImageͼƬ��ʽת��
	QColor oldColor;
	for (int y = 0; y < newImage.height(); y++)
	{
		for (int x = 0; x < newImage.width(); x++)
		{
			oldColor = QColor(image->pixel(x, y));
			int average = (oldColor.red() + oldColor.blue() + oldColor.green()) / 3;
			newImage.setPixel(x, y, qRgb(average, average, average));
		}
	}
	return newImage;
}

//����QImage��rgb��ֵ,
QImage ImageProcess::setRGB(const QImage * image, int valueR, int valueG, int valueB)
{
	QImage newImage = *image;
	int r, g, b;
	QColor color;
	int height = newImage.height();
	int width = newImage.width();
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			color = QColor(image->pixel(x, y));
			r = saturate_cast<int>(color.red() + valueR);
			g = saturate_cast<int>(color.green() + valueG);
			b = saturate_cast<int>(color.blue() + valueB);
			newImage.setPixel(x, y, qRgb(r, g, b));
		}
	}
	return newImage;
}


//�����Աȶ�
QImage ImageProcess::adjustContrast(const QImage * src_image, int value)
{
	QImage image = *src_image;
	//value�Աȶ�����
	/*�㷨����:
		    1) newRGB = RGB + (RGB - Threshold) * (1 / (1 - Contrast / 255) - 1), �Աȶȴ���0
			2) newRGB = RGB + (RGB - Threshold) * Contrast / 255, �Աȶ�С��0
		*/
	int pixels = image.width()*image.height(); //��ȡͼƬȫ�����ظ���
	unsigned int* data = (unsigned int*)image.bits();//����ͼ���һ����������ָ��
	int red, green, blue, nRed, nGreen, nBlue;
	int threshold = 127; //������ֵ
	if (value > 0 && value < 256)
	{
		float param = 1 / (1 - value / 255) - 1;
		for (int i = 0; i < pixels; ++i)		//++i��ʾ��������������: ��1��ʼ��pixels,������
		{
			nRed = qRed(data[i]);  //���ظõ����ذ�����Redֵ
			nBlue = qBlue(data[i]);
			nGreen = qGreen(data[i]);

			//���������㷨��ʽ,threshold ���ó�127

			red = nRed + (nRed - threshold)*param;
			//���ط�Χ�ض�����cv::saturate_cast(); �����ط�Χ���ó�0-255���½ض�
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			blue = nBlue + (nBlue - threshold)*param;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
			green = nGreen + (nGreen - threshold)*param;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;

			data[i] = qRgba(red, blue, green, qAlpha(data[i]));  //����ԭͼi���������ݵ�rgbֵ,alphaֵ����
		}
	}
	else
	{
		for (int i = 0; i < pixels; ++i)
		{
			nRed = qRed(data[i]);
			nGreen = qGreen(data[i]);
			nBlue = qBlue(data[i]);

			red = nRed + (nRed - 127) * value / 100.0;
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = nGreen + (nGreen - 127) * value / 100.0;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = nBlue + (nBlue - 127) * value / 100.0;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

			data[i] = qRgba(red, green, blue, qAlpha(data[i]));
		}
	}

	return image;
}

//ͼƬ������ʾ,����label��Сƥ��
QImage ImageProcess::imageCenter(const QImage * qimage, const QLabel * qLabel)
{
	QImage image;
	QSize imageSize = qimage->size();
	QSize labelSize = qLabel->size();

	//��ȡ�߶ȱ�
	double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
	double dHeightRatio = 1.0*imageSize.height() / labelSize.height();
	//����ֵ����ͼƬ
	if (dWidthRatio > dHeightRatio)
	{
		image = qimage->scaledToWidth(labelSize.width());
	}
	else
	{
		image = qimage->scaledToHeight(labelSize.height());
	}
	return image;

}

QImage ImageProcess::adjustSaturation(const QImage * image, int value)
{
	QImage img = *image;
	int red, green, blue, nRed, nGreen, nBlue;
	int pixels = img.width() * img.height();
	unsigned int *data = (unsigned int *)img.bits();    

	float Increment = value / 100.0;

	float delta = 0;
	float minVal, maxVal;
	float L, S;
	float alpha;

	for (int i = 0; i < pixels; ++i)
	{
		nRed = qRed(data[i]);
		nGreen = qGreen(data[i]);
		nBlue = qBlue(data[i]);

		minVal = std::min(std::min(nRed, nGreen), nBlue);
		maxVal = std::max(std::max(nRed, nGreen), nBlue);
		delta = (maxVal - minVal) / 255.0;
		L = 0.5*(maxVal + minVal) / 255.0;
		S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

		if (Increment > 0)
		{
			alpha = std::max(S, 1 - Increment);
			alpha = 1.0 / alpha - 1;
			red = nRed + (nRed - L * 255.0)*alpha;
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = nGreen + (nGreen - L * 255.0)*alpha;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = nBlue + (nBlue - L * 255.0)*alpha;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
		}
		else
		{
			alpha = Increment;
			red = L * 255.0 + (nRed - L * 255.0)*(1 + alpha);
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = L * 255.0 + (nGreen - L * 255.0)*(1 + alpha);
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = L * 255.0 + (nBlue - L * 255.0)*(1 + alpha);
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
		}

		data[i] = qRgba(red, green, blue, qAlpha(data[i]));
	}

	return img;
}

//ʹ��Canny�㷨
QImage ImageProcess::edge(const QImage * image)
{
	ConvertMatQImage cvt;
	cv::Mat* matImg;
	cvt.qImageToMat(image, matImg);
	cv::Mat cannyMatImg;
	cv::Mat GaussianMatImg;
	cv::Mat GrayMatImg;
	cv::GaussianBlur(*matImg, GaussianMatImg, Size(3, 3), 0, 0);
	cv::cvtColor(GaussianMatImg, GrayMatImg, COLOR_BGR2GRAY);  //������Ҫdebug,Ԫ�����Ϳ��ܲ���BGR��������BGRA��
	int T1 = 40;
	int T2 = 190;
	cv::Canny(GrayMatImg, cannyMatImg, T1, T2, 3, true);
	QImage edgeImg = cvt.matToQImage(&cannyMatImg);
	return edgeImg;
};


QImage ImageProcess::meanFilter(QImage * image)
{
	QImage* newImage = image;
	cv::Mat* matImage;
	ConvertMatQImage cvt;
	cvt.qImageToMat(newImage, matImage);
	cv::Mat* matBlur;
	cv::blur(*matImage, *matBlur, cv::Size(3, 3), cv::Point(-1, -1));
	QImage qBlur = cvt.matToQImage(matBlur);

	return qBlur;
}