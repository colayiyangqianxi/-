#include<iostream>
#include"template.hpp"

using namespace std;
using namespace cv;

//获取模板
bool gettemplate(Mat img, vector<Card>& Cards)
{
	Mat imgGray;
	//转灰度
	cvtColor(img, imgGray, COLOR_BGR2GRAY);

	//二值化
	Mat imgInrange;
	inRange(imgGray, Scalar(0, 0, 0), Scalar(0, 0, 0), imgInrange);

	//找轮廓
	vector<vector<Point>>  contours;
	vector<Vec4i> hierarchy;

	findContours(imgInrange, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//找轮廓(输入图像，轮廓，层次结构，方法，链近似)
	
	//找边界框
	for (int i = 0; i < contours.size(); i++)
	{
		//找边界框
		Rect boundRect = boundingRect(contours[i]);
		//画边界框
		//rectangle(img, boundRect.tl(), boundRect.br(), Scalar(0, 255, 0), 2);
		double ratio = double(boundRect.width) / double(boundRect.height);

		//筛选出字符轮廓
		if (ratio > 0.5 && ratio < 1)
		{
			Mat roi = img(boundRect);  //将字符扣出，放入Cards容器备用
			Cards.push_back({ roi ,boundRect });
		}
	}

	if (Cards.empty())
	{
		return false;
	}
	else
	{
		return true;
	}

}

//银行卡
bool getbanknumber(Mat img, vector<Card>& num)
{
	Mat imgGray, imgGuass, imgThre, imgdst;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(15, 3));
	
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgGuass, Size(3, 3), 0);
	threshold(imgGuass, imgThre, 0, 255, THRESH_BINARY | THRESH_OTSU);
	morphologyEx(imgThre, imgdst, MORPH_CLOSE, kernel);

	//imshow("img1", imgGray);
	//imshow("img2", imgGuass);
	//imshow("img3", imgThre);
	imshow("img4", imgdst);

	vector<vector<Point>>  contours;
	vector<Vec4i> hierarchy;

	findContours(imgdst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		//通过面积、长宽比筛选出银行卡号区域
		double area = contourArea(contours[i]);

		if (area > 850 && area < 1300)
		{
			Rect boundRect = boundingRect(contours[i]);
			double ratio = double(boundRect.width) / double(boundRect.height);

			if (ratio > 2.5 && ratio < 3.5)
			{
				Mat ROI = img(boundRect);
				num.push_back({ ROI ,boundRect });
			}

			rectangle(img, boundRect.tl(), boundRect.br(), Scalar(0, 0, 255), 2);
		}
	}

	imshow("test", img);
	
	if (num.size() != 4)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

//切割四小块
