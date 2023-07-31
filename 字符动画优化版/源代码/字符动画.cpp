/*
	* 编译环境：Windows 11 + Visual Studio 2022 + OpenCV 4.8.0。
*/

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <io.h>

//开启O2优化
#pragma GCC optimize(2)
#pragma G++ optimize(2)

using namespace cv;
using namespace std;

//显示或隐藏光标
void ShowCursor(bool visible) {
	CONSOLE_CURSOR_INFO cursor_info = { 20, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main() {
	//输入输出加速
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	//获取句柄和光标坐标
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };

	//关闭OpenCV日志
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	//获取视频文件
	string vidoename;
	cout << "请输入视频文件路径:";
	cin >> vidoename;

	VideoCapture video;
	Mat frame, gray;
	video.open(vidoename);

	//获取视频分辨率
	int cols = video.get(CAP_PROP_FRAME_WIDTH);
	int rows = video.get(CAP_PROP_FRAME_HEIGHT);

	//计算移动量
	int size;
	cout << "\n请输入字符尺寸:";
	cin >> size;

	int deltx = size;
	int delty = size * 2;

	//读入延迟
	int lag;
	cout << "\n请输入延迟:";
	cin >> lag;

	char c;
	cout << "\n是否保存字符画？[Y/n]:";
	cin >> c;
	bool flag = (c == 'y' || c == 'Y');

	//创建或更新文件夹
	int res = 0;

	if (flag)
		if (_access("ASCII_Art", 0) != 0) {
			system("md ASCII_Art");
		}
		else {
			system("rd /s /q ASCII_Art");
			system("md ASCII_Art");
		}

	vector<string> str;
	char ch[] = " .,-'`:!1&@#$";
	int n = 0;

	//隐藏光标
	ShowCursor(false);

	system("cls");

	//开启计时器
	clock_t start = clock();

	while (n++ < video.get(CAP_PROP_FRAME_COUNT)) {
		//获取每一帧图像并转换成GRAY颜色
		video.read(frame);
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		string s = "";

		//生成字符画
		for (int row = 0; row < rows; row += delty) {
			for (int col = 0; col < cols; col += deltx)
				s = s + ch[gray.at<uchar>(row, col) / 20];

			s += '\n';
		}

		if (flag) {
			//创建文本文档并保存字符画
			char name[32];
			sprintf_s(name, "ASCII_Art/%d.txt", n);
			FILE* fp = fopen(name, "w");

			fprintf(fp, "%s", s.c_str());
			fclose(fp);
		}

		//将字符画存储到容器中
		str.push_back(s);

		//加载信息
		if (n % 20 == 0) {
			SetConsoleCursorPosition(hOut, pos);
			cout << "加载中...   " << n << "/" << video.get(CAP_PROP_FRAME_COUNT) << "\t";
			cout << n / video.get(CAP_PROP_FRAME_COUNT) * 100 << "%    ";
		}
	}

	//结束计时器
	clock_t end = clock();

	//显示光标
	ShowCursor(true);

	//打印信息
	cout << "共生成了 " << video.get(CAP_PROP_FRAME_COUNT) << "张字符画\n";
	cout << "耗时 " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";
	cout << "平均速度为 " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - start) / CLOCKS_PER_SEC) << "帧/s\n";

	cout << "\n按下任意播放字符动画";
	_getch();
	system("cls");

	//隐藏光标
	ShowCursor(false);
	
	//开启计时器
	start = clock();

	for (int i = 0; i < video.get(CAP_PROP_FRAME_COUNT); ++i) {
		//打印字符画
		SetConsoleCursorPosition(hOut, pos);
		cout << str[i];

		//等待指定时间
		waitKey(lag);
	}

	//结束计时器
	end = clock();

	//显示光标
	ShowCursor(true);

	//打印信息
	cout << "共播放了 " << video.get(CAP_PROP_FRAME_COUNT) << "张字符画\n";
	cout << "耗时 " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";
	cout << "平均速度为 " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - start) / CLOCKS_PER_SEC) << "帧/s\n";

	cout << "\n按下任意键关闭窗口";
	_getch();

	return 0;
}