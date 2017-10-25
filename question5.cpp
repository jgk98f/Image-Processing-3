/**
 * Author: Jason Klamert
 * Date: 10/09/2016
 * Description: Program to find ROI of image and perform histogram equalization on the ROI. Then display
 * the image with these new changes.
 **/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#define DEBUG false
#define THRESHOLD 1
using namespace cv;
using namespace std;

void calculateGrayscaleHist(Mat image_roi);
void calculateColorHist(Mat image_roi);

int main( int argc, char** argv )
{
  
  //Usage check!
  if (argc > 6 || argc < 6)
  {
        std::cerr << "usage: " << "Program " << argv[0] << " takes an image as an argument!\n" 
	<< " You may choose to specify a region of interest by calling the program like this: "
        << "question5 image row# col# width height" << std::endl;
        return ( 1 );
  }

  cv::Mat imageOne = cv::imread(argv[1]);
  
  int x1,y1,width,height;
  
  //Empty check!
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

        x1 = atoi((const char*)argv[2]);
        y1 = atoi((const char*)argv[3]);
        width = atoi((const char*)argv[4]);
        height = atoi((const char*)argv[5]);
	int x2 = x1 + width;
	int y2 = y1 + height;
        Rect roi(Point(x1, y1), Point(x2,y2));
        double hist[256];
	double histChannel1[256];
	double histChannel2[256];
	double histChannel3[256];
        double prob[256];
	double probChannel1[256];
	double probChannel2[256];
	double probChannel3[256];     
	double accumChannel1[256];
	double accumChannel2[256];
	double accumChannel3[256];
	double previous,previous2,previous3,previous4;

if(indicator == 0){
	cout << "Image was determined to be grayscale so the THRESHOLD was less than one." << endl;
	imageOne.convertTo(imageOne,CV_8UC1);
	cvtColor(imageOne, imageOne, CV_BGR2GRAY);
	Mat image_roi = imageOne(roi);
        calculateGrayscaleHist(image_roi);

}else{
	Mat image_roi = imageOne(roi);
	cout << "Image was determined to be BGR so the THRESHOLD was greater than one." << endl;
        calculateColorHist(image_roi);

}

        cv::namedWindow( "Display Image3", cv::WINDOW_AUTOSIZE );
        cv::imshow( "Display Image3", imageOne );
        cv::waitKey(0);            

        return 0;
}

void calculateGrayscaleHist(Mat image_roi){
	
        double hist[256];
        double prob[256];
        double accum[256];
	double previous;

        for( int rowNum = 0; rowNum < image_roi.rows; rowNum++){
                for( int colNum = 0; colNum < image_roi.cols; colNum++){
                        int temp = (int) image_roi.at<uchar>(rowNum,colNum);
                        hist[temp] = hist[temp] + 1;
                }
        }

        for( int rowNum = 0; rowNum < 256; rowNum++){
                prob[rowNum] = (hist[rowNum]/(image_roi.rows * image_roi.cols));
        }

        for( int rowNum = 0; rowNum < 256; rowNum++){
                double temp = 256 * prob[rowNum];
                previous += temp;
                accum[rowNum] = floor(previous);
        }

        for(int rowNum = 0; rowNum < image_roi.rows; rowNum++){
                for(int colNum = 0; colNum < image_roi.cols; colNum++){
                        image_roi.at<uchar>(rowNum,colNum) = (uchar) accum[(int) image_roi.at<uchar>(rowNum,colNum)];
                }
        }

} 

void calculateColorHist(Mat image_roi){

	double histChannel1[256];
	double histChannel2[256];
	double histChannel3[256];
	double probChannel1[256];
	double probChannel2[256];
	double probChannel3[256];
	double accumChannel1[256];
	double accumChannel2[256];
	double accumChannel3[256];
	double previous,previous2,previous3;
       
        for( int rowNum = 0; rowNum < image_roi.rows; rowNum++){
                for( int colNum = 0; colNum < image_roi.cols; colNum++){
                        int temp = (int) image_roi.at<Vec3b>(rowNum,colNum)[0];
			int temp2 = (int) image_roi.at<Vec3b>(rowNum,colNum)[1];
			int temp3 = (int) image_roi.at<Vec3b>(rowNum,colNum)[2];
                        histChannel1[temp] = histChannel1[temp] + 1;
			histChannel2[temp2] = histChannel2[temp2] + 1;
			histChannel3[temp3] = histChannel3[temp3] + 1;
                }
        }

        for( int rowNum = 0; rowNum < 256; rowNum++){
                probChannel1[rowNum] = (histChannel1[rowNum]/(image_roi.rows * image_roi.cols));
		probChannel2[rowNum] = (histChannel2[rowNum]/(image_roi.rows * image_roi.cols));
		probChannel3[rowNum] = (histChannel3[rowNum]/(image_roi.rows * image_roi.cols));
        }

        for( int rowNum = 0; rowNum < 256; rowNum++){
                double temp = 256 * probChannel1[rowNum];
		double temp2 = 256 * probChannel2[rowNum];
		double temp3 = 256 * probChannel3[rowNum];
                previous += temp;
		previous2 += temp2;
		previous3 += temp3;
                accumChannel1[rowNum] = floor(previous);
		accumChannel2[rowNum] = floor(previous2);
		accumChannel3[rowNum] = floor(previous3);
        }

        for(int rowNum = 0; rowNum < image_roi.rows; rowNum++){
                for(int colNum = 0; colNum < image_roi.cols; colNum++){
                        image_roi.at<Vec3b>(rowNum,colNum)[0] = (uchar) accumChannel1[(int) image_roi.at<Vec3b>(rowNum,colNum)[0]];
			image_roi.at<Vec3b>(rowNum,colNum)[1] = (uchar) accumChannel1[(int) image_roi.at<Vec3b>(rowNum,colNum)[1]];
			image_roi.at<Vec3b>(rowNum,colNum)[2] = (uchar) accumChannel1[(int) image_roi.at<Vec3b>(rowNum,colNum)[2]];
                }
        }
}
