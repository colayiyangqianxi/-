#include<iostream>
#include"template.hpp"

using namespace std;
using namespace cv;

//��ȡģ��
bool gettemplate(Mat img, vector<Card>& Cards)
{
	Mat imgGray;
	//ת�Ҷ�
	cvtColor(img, imgGray, COLOR_BGR2GRAY);

	//��ֵ��
	Mat imgInrange;
	inRange(imgGray, Scalar(0, 0, 0), Scalar(0, 0, 0), imgInrange);

	//������
	vector<vector<Point>>  contours;
	vector<Vec4i> hierarchy;

	findContours(imgInrange, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//������(����ͼ����������νṹ��������������)
	
	//�ұ߽��
	for (int i = 0; i < contours.size(); i++)
	{
		//�ұ߽��
		Rect boundRect = boundingRect(contours[i]);
		//���߽��
		//rectangle(img, boundRect.tl(), boundRect.br(), Scalar(0, 255, 0), 2);
		double ratio = double(boundRect.width) / double(boundRect.height);

		//ɸѡ���ַ�����
		if (ratio > 0.5 && ratio < 1)
		{
			Mat roi = img(boundRect);  //���ַ��۳�������Cards��������
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

//���п�
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
		//ͨ������������ɸѡ�����п�������
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

//�и���С��
