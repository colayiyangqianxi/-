#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

struct Card
{
	Mat img;
	Rect boundRect;
};

//��ȡģ��
bool gettemplate(Mat img, vector<Card>& Card);

//���п���
bool getbanknumber(Mat img, vector<Card>& num);
