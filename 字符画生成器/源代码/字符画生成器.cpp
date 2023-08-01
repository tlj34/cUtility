/*
	* 编译环境：Windows 11 + Visual Studio 2022 + OpenCV 4.8.0。
*/

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//开启O2优化
#pragma GCC optimize(2)
#pragma G++ optimize(2)

char st1[] = " .,-'`:!1&@#$";
char st2[] = "$@#&1!:`'-,. ";
string str;

int main() {
	//输入输出加速
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	//获取句柄和光标位置
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };

	//关闭OpenCV日志
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	//获取图像文件
	string name;
	cout << "请输入图片路径:";
	cin >> name;
	Mat tmpimg = imread(name, IMREAD_GRAYSCALE);

	//对图片继续缩放
	double times;
	cout << "\n请输入缩小倍数:";
	cin >> times;
	Mat img;
	resize(tmpimg, img, Size((int)(tmpimg.cols / times * 2), (int)(tmpimg.rows / times)), 0, 0, INTER_NEAREST);

	//询问模式
	int mode;
	cout << "\n请输入模式:";
	cin >> mode;

	//生成字符画
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j)
			if(mode)
				str = str + st1[img.at<uchar>(i, j) / 20];
			else
				str = str + st2[img.at<uchar>(i, j) / 20];
		str += '\n';
	}

	//打印字符画
	SetConsoleCursorPosition(hOut, pos);
	cout << str;

	//询问并保存字符画
	char ch;
	cout << "\n\n是否创建一个文本文档来保存字此符画？[Y/n]";
	cin >> ch;

	if (ch == 'Y' || ch == 'y') {
		string name;
		cout << "\n请输入文本文档名称:";
		cin >> name;

		FILE* fp = fopen(name.c_str(), "w");
		fprintf(fp, "%s", str.c_str());
		fclose(fp);
	}

	//结束程序
	system("pause");
	return 0;
}