# Road-lane-recognition-algorithm
### OpenCV를 이용한 차선 인식 알고리즘 개발
##### 
이번 프로젝트는 차선을 인식하는 프로젝트이다. 기존 이미지가 아닌 실시간 주행 영상을 가져와서 비전 처리를 진행하였다.
white line과 yellow line을 인식하게 하였고, 점선 차선도 일정 시간 동안 인식하게 진행하였다. 
Opencv를 통해서 비전처리를 진행하였고 CPP를 사용하였다.


***
실시간 차선 인식 과정
  * videoCapture   
  
         VideoCapture videoCapture("sample.mp4");
  	<img width="400" alt="car" src="https://user-images.githubusercontent.com/66461571/142768976-108df56c-b40c-4b5e-a926-cc97e8c80de3.PNG">
  
  * ROI 구역 지정
  
          Mat region;
          region = region_of_interest(img);
          Mat dst, cdst, cdstP;
	  <img width="400" alt="car" src="https://user-images.githubusercontent.com/66461571/142769104-e9424c42-ef07-430e-b6d6-d7580a5096f6.png">
          
   * Canny 알고리즘 
   
          Canny(region, dst, 50, 200, 3);
          cvtColor(dst, cdst, COLOR_GRAY2BGR);
          cdstP = cdst.clone();
          
   * Hough Line 검출
   
          cv::HoughLines(region, lines, 1, CV_PI / 180, 155, 0, 0);
   
	  <img width="400" alt="car" src="https://user-images.githubusercontent.com/66461571/142769180-425b25a1-9465-4915-81e8-868625e9f545.png">
   * 검출한 엣지 각도 지정
   
          float left = CV_PI / 4;
	        float right = CV_PI - left;
          
        유사한 각도의 직선들을 검출하고 크기값들 중 최대값과 최솟값을 구하여 출력
        
   * 점선 차선 인식 
   
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
