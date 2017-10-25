/**
 * Author: Jason Klamert
 * Date: 10/09/2016
 * Description: Program to shrink an image by removing every other row and column. Then display this newly shrunken image.
 **/ 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <fstream>
using namespace cv;
using namespace std;
#define DEBUG false

void matchByChiSquare();
void usage();
void calculateGrayscaleHist();
void calculateGrayscaleHist2();
void calculateColorHist(Mat image_roi);
void getHistFromFile(char*);
void createHistFile();
void calculateGrayscaleHist2FromFile();

double hist[256];
double hist2[256];
double incomingHist[256];
double probability1[256];
double probability2[256];
Mat imageOne, imageTwo;
double accumulator;

int main( int argc, char** argv )
{
	srand(NULL);
    	char c;
	bool fileFlag = false;

	//createHistFile();	

	//Create a light and dark of an image or increase the contrast to run through the program.
	while ((c = getopt (argc, argv, ":hf:i:s:")) != -1){
		switch (c)
      		{
      			case 'i':
				if(DEBUG == true)
				cout << "Inside case i" << endl;
				imageTwo = imread(optarg);	
        			break;
			case 's':
				if(DEBUG == true)
				cout << "Inside case s" << endl;
				imageOne = imread(optarg);
				break;
			case 'h':
				if(DEBUG == true)
				cout << "Inside case h" << endl;
				usage();
				break;
      			case 'f':
        			if(DEBUG == true)
				cout << "Inside case f\n";
				getHistFromFile(optarg);
				fileFlag = true;
		        	break;
      			
			default:
        			if(DEBUG == true)
				cout << "Inside case default\n";
				usage();
				return -1;
      		}
	}
  
  //Empty check!
  if ( imageOne.empty() )
  {
      std::cerr << "Image One is Empty: " << argv[1] << std::endl;
      return ( 1 );
  }

  if(!fileFlag){
  	//Empty check!
  	if ( imageTwo.empty() )
  	{	
		cerr << "Image Two is Empty: " << argv[1] << endl;
		return ( 1 );
  	}

	cvtColor(imageTwo, imageTwo, CV_BGR2GRAY);
	calculateGrayscaleHist2();
  }
  else{
	calculateGrayscaleHist2FromFile();
  }

  
  cvtColor(imageOne, imageOne, CV_BGR2GRAY);
  calculateGrayscaleHist();
  matchByChiSquare();

  cout << "ChiScore: " << accumulator << endl;

  return 0;
}

/**
 * Author: Jason Klamert
 * Date: 10/26/2016
 * Description: Function that takes two histograms and performs histogram matching using a ChiSquare score. The higher the better.
 **/
void matchByChiSquare()
{
	int count;
	double difference;
	double square;
	double quotient;

	for( count = 0; count < 256; count++){
		//cout << "hist1: " << probability1[count] << " hist2: " << probability2[count] << endl;
		if(probability1[count] != 0.0 && probability2[count] != 0.0){
			difference = abs(probability1[count] - probability2[count]);
			square = difference * difference;
			quotient = (square/probability1[count]);
			accumulator += quotient;
		}else{
		//Add nothing
		}
		//cout << "ChiSquare of " << count << " is " << accumulator << endl;	
	} 	

	

}

/**
 * Author: Jason Klamert
 * Date: 10/26/2016
 * Description: Usage for the program question4.
 **/
void usage()
{
	cout << "Usage: question4 -s SourceImage [ -f file ] or [ -i ReferenceImage ] " << endl;
	cout << "Note: A source image must always be present but the reference histogram may come from another"
	     << " image or it may come from another file." << endl;
}

/**
 * Author: Jason Klamert
 * Date: 10/26/2016
 * Description: Calculate histogram for a grayscale and return it.
 **/
void calculateGrayscaleHist(){
        
        for( int rowNum = 0; rowNum < imageOne.rows; rowNum++){
                for( int colNum = 0; colNum < imageOne.cols; colNum++){
                        int temp = (int) imageOne.at<uchar>(rowNum,colNum);
                        hist[temp] = hist[temp] + 1;
                }
        }

        
  //Probability of each intensity calculation!
  for(int count = 0; count < 256; count++){
        probability1[count] = (double) (hist[count] / (imageOne.rows * imageOne.cols));
  }

}



/**
 * Author: Jason Klamert
 * Date: 10/26/2016
 * Description: Calculate histogram for a grayscale and return it.
 **/
void calculateGrayscaleHist2(){


        for( int rowNum = 0; rowNum < imageTwo.rows; rowNum++){
                for( int colNum = 0; colNum < imageTwo.cols; colNum++){
                        int temp = (int) imageTwo.at<uchar>(rowNum,colNum);
                        hist2[temp] = hist2[temp] + 1;
                }
        }
	


  	//Probability of each intensity calculation!
  	for(int count = 0; count < 256; count++){
        	probability2[count] = (double) (hist2[count] / (imageTwo.rows * imageTwo.cols));
      	}
  
}


/**
 * Author:Jason Klamert
 * Date: 10/27/2016
 * Description: calculate normalized hist from the file.
 **/
void calculateGrayscaleHist2FromFile(){

        
        for( int rowNum = 0; rowNum < 256; rowNum++){
        
        	int temp = incomingHist[rowNum];
                hist2[temp] = hist2[temp] + 1;
               
        }

        //Probability of each intensity calculation!
        for(int count = 0; count < 256; count++){
        	probability2[count] = (double) (hist2[count] / 256);
        }
        
}
        
 
/**
 * Author: Jason Klamert
 * Date: 10/27/2016
 * Description: Get a histogram from file.
 **/ 
void getHistFromFile(char* fileName){
	ifstream input;
	input.open(fileName,fstream::in);
	int count = 0;
	char trashLine;

	if(input){
		cout << "Reading your histogram from file " << fileName << " line by line." << endl;
		while(input >>  incomingHist[count] >> trashLine){
			cout << "incomingHist[" << count << "] from file " << incomingHist[count] << endl;
			count++;
		}
	}
	else{
		perror("file I/O");
		exit(-1);
	}	
}

/**
 * Author: Jason Klamert
 * Date: 10/27/2016
 * Description: function to write a value between 0-256 in a file. One per line.
 **/
void createHistFile(){
        
	char* name = "histogram.txt";
	ofstream output(name);
        int count = 0;

        if(output){
                cout << "Creating a histogram file named " << name << " line by line." << endl;
                while(count < 256){
                        output << (rand() % 256) << "\n";
                        cout << "hist2[" << count << "] from file " << hist2[count] << endl;
                	count++;
		}
        }
        else{
                perror("file I/O");
                usage();
                exit(-1);
        }
}

