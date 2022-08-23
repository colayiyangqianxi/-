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

//获取模板
bool gettemplate(Mat img, vector<Card>& Card);

//银行卡号
bool getbanknumber(Mat img, vector<Card>& num);
