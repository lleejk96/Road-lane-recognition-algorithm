#include <iostream>
#include <opencv2/opencv.hpp> 

using namespace cv;
using namespace std;

Point delay_pt1, delay_pt2;  //매 페이지의 값을 누적한다
bool check;


void DrawLine(Mat region, Mat dst, Mat img, int count, int time)
{
	float left = CV_PI / 4;		//왼쪽 차선 각도
	float right = CV_PI - left;		//오른쪽 차선 각도
	
	vector<Vec2f> lines;
	Point pt1, pt2;
	cv::HoughLines(region, lines, 1, CV_PI / 180, 155, 0, 0);		//허프 변환

	for (size_t i = 0; i < lines.size(); i++)
	{
		float theta = lines[i][1];			//검출된 직선의 각도
		float max = 0, min = 1000000;		//직선 크기 최대값 최솟값
		
		if ((int)theta == (int)left) {		//왼쪽 차선
			for (size_t i = 0; i < lines.size(); i++) {
				if (min > lines[i][0] && (int)left == (int)lines[i][1])
					min = lines[i][0];
			}
			double a = cos(theta), b = sin(theta);
			double x0 = a * min, y0 = b * min;
			pt1.x = cvRound(x0 + 1000 * (-b)) +300;
			pt1.y = cvRound(y0 + 1000 * (a)) +300;
			pt2.x = cvRound(x0 - 1000 * (-b)) +300;
			pt2.y = cvRound(y0 - 1000 * (a)) +300;
			line(img, pt1, pt2, Scalar(100, 100, 255), 2, LINE_AA);
		}
		if ((int)theta == (int)right) {		//오른쪽 차선
			for (size_t i = 0; i < lines.size(); i++) {
				if (max < lines[i][0] && (int)right == (int)lines[i][1])
					max = lines[i][0];
			}
			double a = cos(theta), b = sin(theta);
			double x0 = a * max, y0 = b * max;
			pt1.x = cvRound(x0 + 1000 * (-b)) +450;
			pt1.y = cvRound(y0 + 1000 * (a)) +300;
			pt2.x = cvRound(x0 - 1000 * (-b)) +450;
			pt2.y = cvRound(y0 - 1000 * (a)) +300;
			delay_pt1 = pt1;
			delay_pt2 = pt2;
			check = true;
			cout << "p1 : " << pt1 << "p3 : " << delay_pt1 << endl;
			cout << "p2 : " << pt2 << "p4 : " << delay_pt2 << endl;
			line(img, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
		}
		else { //오른쪽 차선이 검출되지 않을 때 
			if (time < 200 && check == true) {	//점선 차선이면 몇 초이내에 검출될떄까지 라인 생성
				line(img, delay_pt1, delay_pt2, Scalar(100, 50, 155), 2, LINE_AA);
				cout << "time : " << time << endl;
				time++;
			}
			else {	// 몇초 이후에도 라인이 검출되지 않으면 라인 제거
				time = 0;
				cout << "false" << endl;
				check = false;
			}
		}
	}
}


Mat region_of_interest(Mat img) {		//roi 구역 지정
	Mat region;

	region = img(Rect(300,300,700,400));

	return region;
}

int main(int argc, char** argv)
{
	VideoCapture videoCapture("sample.mp4");
	int time = 0;

	if (!videoCapture.isOpened())
	{
		printf("Can't open the camera");
		return -1;
	}
	check = true;
	Mat img;
	int count = 0;
	while (1)
	{
		videoCapture >> img;

		if (img.empty())
		{
			printf("empty image");
			return 0;
		}
		Mat region;
		region = region_of_interest(img);
		Mat dst, cdst, cdstP;
		
		Canny(region, dst, 50, 200, 3);
		cvtColor(dst, cdst, COLOR_GRAY2BGR);
		cdstP = cdst.clone();
		// 허프 직선 검출 알고리즘
		
		DrawLine(dst, cdst, img, count, time);
		
		int fps = (int)videoCapture.get(CAP_PROP_FPS);
		rectangle(img, Rect(Point(400, 400), Point(1000, 700)), Scalar(0, 0, 255), 1, 8, 0);   //인식 범위 출력

		imshow("camera img", img);
		count++;
		if (waitKey(1000/fps) >= 0)
			break;
	}

}
