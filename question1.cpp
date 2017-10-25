/*
 * question1.cpp
 * Description: Program to calculate difference in grayscale and get the average.
 *  Created on: 10/05/2016
 *      Author: Jason Klamert
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#define DEBUG false
#define THRESHOLD 1
using namespace std;
using namespace cv;

void mouseHandlerGrayscale(int event, int x, int y, int flags, void* param);
void mouseHandlerColor(int event, int x, int y, int flags, void* param);
Mat imageOne;

int main( int argc, char** argv )
{ 
  
  //Usage check.
  if ( argc < 2 || argc > 2)
    {
        std::cerr << "usage: " << "Program takes one image argument!\n" << std::endl;
        return ( 1 );
    }
  
  imageOne = cv::imread(argv[1]);
  
  //Check if imageOne empty.
  if ( imageOne.empty() )
  {
      std::cerr << "Image One is Empty: " << argv[1] << std::endl;
      return ( 1 );
  }

  int indicator = 0;

//Use threshold number to determine if color or grayscale. Using R=G=B isn't as reliable.
for(int rowNum = 0; rowNum < imageOne.rows; rowNum++){
	for(int colNum = 0; colNum < imageOne.cols; colNum++){	
  		int B = (int) imageOne.at<Vec3b>(rowNum,colNum)[0];
  		int G = (int) imageOne.at<Vec3b>(rowNum,colNum)[1];
  		int R = (int) imageOne.at<Vec3b>(rowNum,colNum)[2];

  		// IF B=G=R Then image is grayscale
  		if(!(abs(R-G) < THRESHOLD)){
			indicator++;
  		}
	}
}  


  if(DEBUG == true){
	
	if(indicator > 0)
	 cout << "Color image!\n";
	else
	 cout << "Grayscale image!\n";
	cout << "Color condition has been violated " << indicator << " times!\n";
	std::cout << "image size in pixels: " << imageOne.rows * imageOne.cols << std::endl; 
  }

  cv::namedWindow( "imageOne", cv::WINDOW_AUTOSIZE );

  if(indicator > 0){
  	cvSetMouseCallback("imageOne",mouseHandlerColor, &imageOne);
  }else{
  	cvSetMouseCallback("imageOne",mouseHandlerGrayscale, &imageOne);
  }

  cv::imshow( "imageOne", imageOne );
  cv::waitKey(0);
  

  return 0;
}

/**
 * Author: Jason Klamert
 * Date: 10/22/2016
 * Description: Event for Left Click on the Mouse in OpenCV Grayscale Window.
 **/ 
void mouseHandlerGrayscale(int event, int x, int y, int flags, void* param)
{
	switch(event){
      	
	case CV_EVENT_LBUTTONDOWN: 
        	if(DEBUG == true)
		cout << "Left button down for grayscale!\n";
		cout << "Grayscale Intensity at X: " << x << " Y: " << y << " is "
		<< (int) imageOne.at<uchar>(x,y) << "!" << endl;
        break;
    	}
}


/**
 * Author: Jason Klamert
 * Date: 10/22/2016
 * Description: Event for left click on the mouse in openCV Color Window.
 **/
void mouseHandlerColor(int event, int x, int y, int flags, void* param)
{
       switch(event){

        case CV_EVENT_LBUTTONDOWN:
                if(DEBUG == true)
		cout << "Left button down for Color!\n";
                cout << "Color of pixel at Position: X " << x << " Y: " << y 
		<< " is B:" << (int)  imageOne.at<Vec3b>(x,y)[0] << " G:" << (int) imageOne.at<Vec3b>(x,y)[1] << " R:"
		<< (int) imageOne.at<Vec3b>(x,y)[2] <<  endl;
        break;
        }
}

