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
bool getbanknumber(Mat imgbank, vector<Card>& num)
{
	Mat imgGray, imgGuass, imgThre, imgdst;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(15, 3));
	
	cvtColor(imgbank, imgGray, COLOR_BGR2GRAY);
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
				Mat ROI = imgbank(boundRect);
				num.push_back({ ROI ,boundRect });
			}

			rectangle(imgbank, boundRect.tl(), boundRect.br(), Scalar(0, 0, 255), 2);
		}
	}

	/*for (int i = 0; i < num.size(); i++)
	{
		imshow(to_string(i), num[i].img);
		waitKey(0);
	}*/

	imshow("test", imgbank);
	
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
bool Cut(vector<Card>& num, vector<Card>& single)
{
	//循环上面切割出来的四个小块，将上面的字符一一切割出来。
	for (int i = 0; i < num.size(); i++)
	{
		Mat imgGray;
		cvtColor(num[i].img, imgGray, COLOR_BGR2GRAY);

		Mat imgThre;
		threshold(imgGray, imgThre, 0, 255, THRESH_BINARY | THRESH_OTSU);

		//imshow("imgGray", imgGray);
		//imshow("imgThre", imgThre);

		vector <vector<Point>> contours;
		vector<Vec4i> hierarchy;

		findContours(imgThre, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		//drawContours(num[i].img, contours, -1, Scalar(255, 0, 255), 2);
		

		for (int j = 0; j < contours.size(); j++)
		{
			Rect boundRect = boundingRect(contours[j]);
			//字符相对于银行卡所在的位置
			Rect roi_rect(boundRect.x + num[i].boundRect.x, boundRect.y + num[i].boundRect.y, boundRect.width, boundRect.height);
			Mat r_roi = num[i].img(boundRect);
			single.push_back({ r_roi ,roi_rect });

			rectangle(num[i].img, boundRect.tl(), boundRect.br(), Scalar(0, 0, 255), 2);
		}
		for (int j = 0; j < num.size(); j++)
		{
			imshow(to_string(j), num[j].img);
			waitKey(0);
		}
	}

	if (single.size() != 16)
	{
		cout << "false" << endl;
		return false;
	}
	//for (int i = 0; i < single.size() - 1; i++)
	//{
	//	for (int j = 0; j < single.size() - 1 - i; j++)
	//	{
	//		if (single[j].boundRect.x > single[j + 1].boundRect.x)
	//		{
	//			Card temp = single[j];
	//			single[j] = single[j + 1];
	//			single[j + 1] = temp;
	//		}
	//	}
	//}


	for (int i = 0; i < single.size(); i++)
	{
		imshow("",single[i].img);
		waitKey(0);
	}

	return true;
}