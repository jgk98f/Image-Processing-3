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
#define THRESHOLD 1
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  
  //Usage check!
  if (argc > 6 || argc < 2 || argc < 6 && argc > 2)
  {
        std::cerr << "usage: " << "Program " << argv[0] << " takes an image as an argument!\n" 
	<< " You may choose to specify a region of interest by calling the program like this: "
        << "question6 image x1 y1 x2 y2" << std::endl;
        return ( 1 );
  }

  cv::Mat imageOne = cv::imread(argv[1]);
  
  int x1,x2,y1,y2;

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

    Mat original = imageOne.clone();

    if(argc == 6){
        x1 = atoi((const char*)argv[2]);
        y1 = atoi((const char*)argv[3]);
        x2 = atoi((const char*)argv[4]);
        y2 = atoi((const char*)argv[5]);
       
        Rect roi(Point(x1, y1), Point(x2, y2));
 
        Mat image_roi = imageOne(roi);
	Mat original_roi = original(roi);
        double hist[256];
	double histChannel1[256];
	double histChannel2[256];
	double histChannel3[256];
        double prob[256];
	double probChannel1[256];
	double probChannel2[256];
	double probChannel3[256];
        double accum[256];
	double accumChannel1[256];
	double accumChannel2[256];
	double accumChannel3[256];
	double previous,previous2,previous3,previous4;

if(indicator == 0){

	imageOne.convertTo(imageOne,CV_8UC1);
	cvtColor(imageOne, imageOne, CV_BGR2GRAY);


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

        for(int rowNum = 0; rowNum < original_roi.rows; rowNum++){
                for(int colNum = 0; colNum < original_roi.cols; colNum++){
                   original_roi.at<uchar>(rowNum,colNum) = (uchar) image_roi.at<uchar>(rowNum,colNum) - original_roi.at<uchar>(rowNum,colNum); 
                }
        }

	cv::namedWindow( "Display Image3", cv::WINDOW_AUTOSIZE );
        cv::imshow( "Display Image3", original );
        cv::waitKey(0);

}else{
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

        for(int rowNum = 0; rowNum < original_roi.rows; rowNum++){
                for(int colNum = 0; colNum < original_roi.cols; colNum++){
                   original_roi.at<Vec3b>(rowNum,colNum)[0] = (uchar) image_roi.at<Vec3b>(rowNum,colNum)[0] - original_roi.at<Vec3b>(rowNum,colNum)[0];
     		   original_roi.at<Vec3b>(rowNum,colNum)[1] = (uchar) image_roi.at<Vec3b>(rowNum,colNum)[1] - original_roi.at<Vec3b>(rowNum,colNum)[1];
		   original_roi.at<Vec3b>(rowNum,colNum)[2] = (uchar) image_roi.at<Vec3b>(rowNum,colNum)[2] - original_roi.at<Vec3b>(rowNum,colNum)[2];
                }
        }

	cv::namedWindow( "Display Image3", cv::WINDOW_AUTOSIZE );
        cv::imshow( "Display Image3", original );
        cv::waitKey(0);
            
}
        return 0;
  }

  Mat result = imageOne.clone();
  Mat imageTwo = imageOne.clone();
  //Initialize and declare vars.
  double min,max = 0;
  int normalizedHistogram[256];
  float coefficientOfVariation = 0;
  int const totalBins = 256;
  int accumulator = 0;
  int sumOfSquares = 0;
  float standardDeviation = 0;
  float variance = 0;
  int meanAccumulator = 0;
  short int mean = 0;
  imageOne.convertTo(imageOne,CV_8UC1);

  double histogram[totalBins];
  double histogramChannel1[totalBins];
  double histogramChannel2[totalBins];
  double histogramChannel3[totalBins];

  double probability[totalBins];
  double probabilityChannel1[totalBins];
  double probabilityChannel2[totalBins];
  double probabilityChannel3[totalBins];

  double accumulatorHist[totalBins];
  double accumulatorHistChannel1[totalBins];
  double accumulatorHistChannel2[totalBins];
  double accumulatorHistChannel3[totalBins];

  double previousTerm3;
  double previousTerm4;
  double previousTerm2;
  double previousTerm;

  cvtColor(imageOne, imageOne, CV_BGR2GRAY);
  //Set all histogram to 0.
  for(int count = 0; count < totalBins; count++){
        histogram[count] = 0;
  }

          //Count all of the intensity values found at each pixel and place it in histogram. In addition add up all of
          //the values found so we can take the mean later.
          for(int rowNum = 0; rowNum < imageOne.rows; rowNum++ ){
                  for(int colNum = 0;colNum < imageOne.cols; colNum++){

                        if(DEBUG == true)
                        {
                                //std::cout << "imageOne " << "row: " << rowNum << " col: " << colNum << " Itensity: "
                                //<<  (int) imageOne.at<uchar>(colNum,rowNum) << std::endl;
                        }

                        int temp = (int)  imageOne.at<uchar>(rowNum,colNum);
                        int temp2 = (int) imageTwo.at<Vec3b>(rowNum,colNum)[0];
                        int temp3 = (int) imageTwo.at<Vec3b>(rowNum,colNum)[1];
                        int temp4 = (int) imageTwo.at<Vec3b>(rowNum,colNum)[2];
                        meanAccumulator += temp;
                        histogram[temp] = histogram[temp] + 1;
                        histogramChannel1[temp2] = histogramChannel1[temp2] + 1;
                        histogramChannel2[temp3] = histogramChannel2[temp3] + 1;
                        histogramChannel3[temp4] = histogramChannel3[temp4] + 1;
                        accumulator = accumulator + 1;
                  }
          }

  //Find the mean intensity of the image.
  mean = (short int) (meanAccumulator/(imageOne.rows*imageOne.cols));

         //Calculate variance summation of the image from the mean.
         for(int rowNum = 0; rowNum < imageOne.rows; rowNum++){
                for(int colNum = 0; colNum < imageOne.cols; colNum++){

                        int temp = (int) imageOne.at<uchar>(rowNum,colNum);
                        int result = temp - mean;
                        result = result * result;
                        sumOfSquares += result;
                }
         }

  //Probability of each intensity calculation!
  for(int count = 0; count < 256; count++){
        probability[count] = (double) (histogram[count] / (imageOne.rows * imageOne.cols));
        probabilityChannel1[count] = (double) (histogramChannel1[count] / (imageTwo.rows * imageTwo.cols));
        probabilityChannel2[count] = (double) (histogramChannel2[count] / (imageTwo.rows * imageTwo.cols));
        probabilityChannel3[count] = (double) (histogramChannel3[count] / (imageTwo.rows * imageTwo.cols));
  }

  //Calculate new pixel distribution.
  for(int count = 0; count < 256; count++){
        double temp = 256 * probability[count];
        double temp2 = 256 * probabilityChannel1[count];
        double temp3 = 256 * probabilityChannel2[count];
        double temp4 = 256 * probabilityChannel3[count];
        previousTerm2 += temp2;
        previousTerm3 += temp3;
        previousTerm4 += temp4;
        previousTerm += temp;
        accumulatorHist[count] = floor(previousTerm);
        accumulatorHistChannel1[count] = floor(previousTerm2);
        accumulatorHistChannel2[count] = floor(previousTerm3);
        accumulatorHistChannel3[count] = floor(previousTerm4);
  }

  //Apply the equalization to the two images.
  for(int rowNum = 0; rowNum < imageOne.rows; rowNum++){
        for(int colNum = 0; colNum < imageOne.cols; colNum++){
                imageOne.at<uchar>(rowNum,colNum) = (uchar) accumulatorHist[(int) imageOne.at<uchar>(rowNum,colNum)];
                imageTwo.at<Vec3b>(rowNum,colNum)[0] = (uchar) accumulatorHistChannel1[(int) imageTwo.at<Vec3b>(rowNum,colNum)[0]];
                imageTwo.at<Vec3b>(rowNum,colNum)[1] = (uchar) accumulatorHistChannel2[(int) imageTwo.at<Vec3b>(rowNum,colNum)[1]];
                imageTwo.at<Vec3b>(rowNum,colNum)[2] = (uchar) accumulatorHistChannel3[(int) imageTwo.at<Vec3b>(rowNum,colNum)[2]];
        }
  }

  //compute the standard deviation and coefficient Of Variation for the data set.
  variance = (sumOfSquares/((imageOne.rows*imageOne.cols) - 1));
  standardDeviation = sqrt(variance);
  coefficientOfVariation = standardDeviation/mean;

  //Draw conclusions from our statistical measures.
  if(DEBUG == true){
        for(int count = 0; count < 256; count++){
                std::cout << "Intensity Level Gray: " << count << " has probability " << probability[count] << "\n";
                cout << "Intensity level Channel 3: " << count << " has probability " << probabilityChannel3[count] << "\n";
                cout << "Intensity level Channel 2: " << count << " has probability " << probabilityChannel2[count] << "\n";
                cout << "Intensity level Channel 1: " << count << " has probability " << probabilityChannel1[count] << "\n";
        }

        for(int count = 0; count < 256; count++){
                 cout << "New pixel distribution numbers gray: " << accumulatorHist[count] << endl;
                 cout << "New pixel distribution numbers Channel1: " << accumulatorHistChannel1[count] << endl;
                 cout << "New pixel distribution numbers Channel2: " << accumulatorHistChannel2[count] << endl;
                 cout << "New pixel distribution numbers Channel3: " << accumulatorHistChannel3[count] << endl;
        }

        std::cout << "imageOne: Rows: " << imageOne.rows << std::endl;
        std::cout << "imageOne: Cols: " << imageOne.cols << std::endl;
        std::cout << "imageOne: total pixels: " << imageOne.rows * imageOne.cols << std::endl;
        std::cout << "imageOne: pixels processed for histogram: " << accumulator << std::endl;
        std::cout << "imageOne: mean intensity for image: " << mean << std::endl;
        std::cout << "imageOne: standard deviation: " << standardDeviation << std::endl;
        std::cout << "imageOne: variance: " << variance << std::endl;
	cout << "indicator violated: " << indicator << " times!" << endl;
  }else{
        std::cout << "imageOne: mean intensity for image: " << mean << std::endl;
        std::cout << "imageOne: standard deviation: " << standardDeviation << std::endl;
        std::cout << "imageOne: variance: " << variance << std::endl;
	cout << "indicator violated: " << indicator << " times!" << endl;
	if(argc == 6)
	cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << endl;
  }

  //Finally, display the corresponding color or grayscale image.
  if(indicator == 0){
	original.convertTo(original,CV_8UC1);
	cvtColor(original, original, CV_BGR2GRAY);
	for(int rowNum = 0; rowNum < imageOne.rows; rowNum++){
		for( int colNum = 0; colNum < imageOne.cols; colNum++){
			imageOne.at<uchar>(rowNum,colNum) = (uchar) imageOne.at<uchar>(rowNum,colNum) - original.at<uchar>(rowNum,colNum);		
		}
	}

        cv::namedWindow( "Display Image", cv::WINDOW_AUTOSIZE );
        cv::imshow( "Display Image", imageOne );
        cv::waitKey(0);
  }
  else if(indicator > 0){

	for(int rowNum = 0; rowNum < imageOne.rows; rowNum++){
                for( int colNum = 0; colNum < imageOne.cols; colNum++){
                        result.at<Vec3b>(rowNum,colNum)[0] = (uchar) imageTwo.at<Vec3b>(rowNum,colNum)[0] - original.at<Vec3b>(rowNum,colNum)[0];
			result.at<Vec3b>(rowNum,colNum)[1] = (uchar) imageTwo.at<Vec3b>(rowNum,colNum)[1] - original.at<Vec3b>(rowNum,colNum)[1];
			result.at<Vec3b>(rowNum,colNum)[2] = (uchar) imageTwo.at<Vec3b>(rowNum,colNum)[2] - original.at<Vec3b>(rowNum,colNum)[2];
                }
        }

        cv::namedWindow( "Display Image2", cv::WINDOW_AUTOSIZE );
        cv::imshow( "Display Image2", result );
        cv::waitKey(0);
  }

  return 0;
}

