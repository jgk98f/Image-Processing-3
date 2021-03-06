# Image-Processing-3
More image operations using color, histogram equalization, and regions of interest.

----------------------------------------------------------------------------
README
----------------------------------------------------------------------------
Author: Jason Klamert
Programs: question[1-6]
Digital Image Processing
----------------------------------------------------------------------------
Summary:
----------------------------------------------------------------------------

The following programs correspond to the questions of project number two and
are named accordingly.

Question 1 takes one image as an arguement (color or grayscale) and it 
displays this image for us. Once displayed, the user may left click anywhere
on the image to obtain either the grayscale value or the BGR value of the
pixel that has been clicked. This will keep going until the user exits
out of the image window.

Question 2 takes one image as an arguement (color or grayscale) and it finds
the mean value of the image. Note this could be either one channel or three
dependent on if it is grayscale or BGR. The image is thresholded from this
mean value where if intensity < mean the intenisty is set to zero. Otherwise,
if intensity > mean then the intensity is set to 255. This is performed on
each channel of a color image as well to produce a thresholded image.

Question 3 takes one image as an arguement (color or grayscale) and it
performs histogram equalization on that image. For grayscale it is just a 
one channel operation but for color we calculate the histogram of each
channel and perform equalization on it seperately.

Question 4 takes either two images as arguements or one source image and a
file containing a histogram for comparison. It can be called using this:
	question4 -s SourceImage [-f fileName.txt or -i referenceImage]
If you wish to see the help message the '-h' flag is available. Once the 
input is settled the program computes or reads the histograms and normalizes
them. Once normalized we compare them by using the ChiSquare method. It should
be noted that a lower score means a better match for this method.

Question 5 is a program that takes a number of parameters from the command line
and it may be called like this:
	question5 rowNum colNum width height
It should be noted that these params correspond to a rectangle region of interest
that will be made on the users behalf. We run the histogram equilization on the
region of interest, put the region of interest back into the image and then
we display the image for the user with the equalized ROI intact.

Question 6 is a program that takes one image as an argument and it computes
the histogram equalization image from the original image. The image is then
subtracted from it histogram equalized counter part. If the user specifies
an ROI on the commandline then use the ROI and compute the equalized ROI.
Then the program will subtract the original ROI from the equalized ROI.
The program may be called like this:
	question6 image x1 y1 x2 y2 	(For ROI)
	or
	question6 image			(For Whole Image)

----------------------------------------------------------------------------
Directions:
----------------------------------------------------------------------------

Makefile:
	make -f MakeFile

clean:
	make -f MakeFile clean

run:
	question1 imageName
	question2 imageName
	question3 imageName
	question4 -s SourceImage [-f fileName.txt or -i referenceImage]
	question5 rowNum colNum width height
        question6 image x1 y1 x2 y2     (For ROI)
        or
        question6 image                 (For Whole Image)


