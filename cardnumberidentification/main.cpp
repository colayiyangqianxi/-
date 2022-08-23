#include"template.hpp"
#include<iostream>

using namespace std;
using namespace cv;

void main()
{
	string tempath = "D:/hanjiang/hanjiang-assess/1/1/ocr_a_reference.png";
	Mat img = imread(tempath);
	vector<Card> Cards;

	gettemplate(img, Cards);

	string bankpath = "D:/hanjiang/hanjiang-assess/1/1/credit_card_05.png";
	Mat imgbank = imread(bankpath);
	vector<Card> Banknum;

	getbanknumber(imgbank, Banknum);

	vector<Card> single;
	Cut(Banknum, single);

	
	//imshow("Image", imgbank);
	waitKey(0);
}