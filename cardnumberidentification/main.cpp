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

	for (int i = 0; i < Banknum.size(); i++) {
		imshow("bank num", Banknum[i].img);
		waitKey(0);
	}

	imshow("Image", imgbank);
	waitKey(0);
}