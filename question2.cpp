/**
 * Author: Jason Klamert
 * Date: 10/09/2016
 * Description: Program to find image statistics and calculate the histogram. The histogram is displayed at the end
 * of the program and some conclusions are reached based on some basic information found thorughout the program.
 **/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#define DEBUG false
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  //Usage check!
  if ( argc < 3 || argc > 3)
  {
        std::cerr << "usage: " << "Program " << argv[0] << " takes an two images as an argument! The first image be grayscale and the second image should be color.\n" << std::endl;
        return ( 1 );
  }

  cv::Mat imageOne = cv::imread(argv[1]);
  
  //Empty check!
  if ( imageOne.empty() )
  {
      std::cerr << "Image One is Empty: " << argv[1] << std::endl;
      return ( 1 );
  }

  Mat imageTwo = imread(argv[2]);

  //Empty check!
  if ( imageTwo.empty() )
  {
  	std::cerr << "Image Two is Empty: " << argv[2] << std::endl;
   	return ( 1 );
  }

  //Initialize and declare vars.
  double min,max = 0;
  int meanAccumulator = 0, channelOneAccumulator = 0, channelTwoAccumulator = 0, channelThreeAccumulator = 0;
  short int mean = 0, channelOneMean = 0, channelTwoMean = 0, channelThreeMean = 0;
  
  //Image one is our grayscale image.
  cvtColor(imageOne, imageOne, CV_BGR2GRAY);

	  //Calculate the mean of the grayscale image and each channel of a color image.
	  for(int rowNum = 0; rowNum < imageOne.rows; rowNum++ ){
	          for(int colNum = 0;colNum < imageOne.cols; colNum++){
			
			int temp2 = (int) imageTwo.at<Vec3b>(rowNum,colNum)[0];
			int temp3 = (int) imageTwo.at<Vec3b>(rowNum,colNum)[1];
			int temp4 = (int) imageTwo.at<Vec3b>(rowNum,colNum)[2];
			int temp = (int)  imageOne.at<uchar>(rowNum,colNum);
			
			channelOneAccumulator += temp2;
			channelTwoAccumulator += temp3;
			channelThreeAccumulator += temp4;
 			meanAccumulator += temp;
	          }
	  }

  //Find the mean intensity of the images.
  mean = (short int) (meanAccumulator/(imageOne.rows*imageOne.cols));
  channelOneMean = (short int) (channelOneAccumulator/(imageTwo.rows*imageTwo.cols));
  channelTwoMean = (short int) (channelTwoAccumulator/(imageTwo.rows*imageTwo.cols));
  channelThreeMean = (short int) (channelThreeAccumulator/(imageTwo.rows*imageTwo.cols));

  //Threshhold the imageOne and imageTwo.	
  for(int rowNum = 0; rowNum < imageOne.rows; rowNum++){
  	for(int colNum = 0; colNum < imageOne.cols; colNum++){

		if((int) imageOne.at<uchar>(rowNum,colNum) < mean)
			imageOne.at<uchar>(rowNum,colNum) = (uchar) 0;
		else
			imageOne.at<uchar>(rowNum,colNum) = (uchar) 255;

		if((int) imageTwo.at<Vec3b>(rowNum,colNum)[0] < channelOneMean)
			imageTwo.at<Vec3b>(rowNum,colNum)[0] = (uchar) 0;
		else
			imageTwo.at<Vec3b>(rowNum,colNum)[0] = (uchar) 255;

		if((int) imageTwo.at<Vec3b>(rowNum,colNum)[1] < channelOneMean)
                        imageTwo.at<Vec3b>(rowNum,colNum)[1] = (uchar) 0;
		else
                        imageTwo.at<Vec3b>(rowNum,colNum)[1] = (uchar) 255;

		if((int) imageTwo.at<Vec3b>(rowNum,colNum)[2] < channelOneMean)
                        imageTwo.at<Vec3b>(rowNum,colNum)[2] = (uchar) 0;
		else
                        imageTwo.at<Vec3b>(rowNum,colNum)[2] = (uchar) 255;

	}
  }

  //Draw conclusions from our statistical measures.
  if(DEBUG == true){
        std::cout << "imageOne: Rows: " << imageOne.rows << std::endl;
        std::cout << "imageOne: Cols: " << imageOne.cols << std::endl;
	std::cout << "imageOne: total pixels: " << imageOne.rows * imageOne.cols << std::endl;
	std::cout << "imageTwo: Rows: " << imageOne.rows << std::endl;
        std::cout << "imageTwo: Cols: " << imageOne.cols << std::endl;
        std::cout << "imageTwo: total pixels: " << imageOne.rows * imageOne.cols << std::endl;
        
	std::cout << "imageOne: mean intensity: " << mean << std::endl;
        cout << "imageTwo: mean intensity for channel one: " << channelOneMean << endl;
        cout << "imageTwo: mean intensity for channel two: " << channelTwoMean << endl;
        cout << "imageTwo: mean intensity for channel three: " << channelThreeMean << endl;

  }else{
  	std::cout << "imageOne: mean intensity for image: " << mean << std::endl;
	cout << "imageTwo: mean intensity for channel one: " << channelOneMean << endl;
	cout << "imageTwo: mean intensity for channel two: " << channelTwoMean << endl;
	cout << "imageTwo: mean intensity for channel three: " << channelThreeMean << endl;
  }
  
  //Finally, display the image.
  cv::namedWindow( "Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display Image", imageOne );
  cv::waitKey(0);

  cv::namedWindow( "Display Image2", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display Image2", imageTwo );
  cv::waitKey(0);


  return 0;
}
