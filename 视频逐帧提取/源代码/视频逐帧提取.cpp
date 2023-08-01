/*
	* 编译环境：Windows 11 + Visual Studio 2022 + OpenCV 4.8.0。
*/

#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
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
	video.open(vidoename);

	//获取或创建文件夹
	string foldername;
	cout << "您将会把文件保存到哪里？";
	cin >> foldername;

	//隐藏光标并清屏
	ShowCursor(false);
	system("cls");

	//开启计时器
	clock_t start = clock();

	//逐帧提取图片
	for (int i = 1; i <= video.get(CAP_PROP_FRAME_COUNT); ++i) {
		//获取图片
		Mat tmp;
		video.read(tmp);

		//保存图片
		string imgname = foldername + '/' + to_string(i) + ".jpg";
		imwrite(imgname, tmp);

		//加载信息
		if (i % 20 == 0) {
			SetConsoleCursorPosition(hOut, pos);
			cout << "加载中...   " << i << " / " << video.get(CAP_PROP_FRAME_COUNT) << "        ";
			cout << i / video.get(CAP_PROP_FRAME_COUNT) * 100 << "%    ";
		}
	}

	//结束计时器
	clock_t end = clock();

	//显示光标并清屏
	ShowCursor(true);
	system("cls");

	//打印信息
	cout << "共生保存 " << video.get(CAP_PROP_FRAME_COUNT) << "张图片\n";
	cout << "耗时 " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";
	cout << "平均速度为 " << video.get(CAP_PROP_FRAME_COUNT) / (int)((double)(end - start) / CLOCKS_PER_SEC) << "帧/s\n";

	//结束程序
	system("pause");
	return 0;
}