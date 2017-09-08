/*
# Copyright (C) <2017>  <Chetan Nanda: chetannanda@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <iostream>
#include <vector>

// OpenCV headers
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

/// Global Variables
const int sky_slider1_max = 255;
const int sky_slider2_max = 255;

int skycolor_min;
int skycolor_max;

const int sky_threshold_max = 255;
int threshold_val;

/// Matrices to store images
Mat src1;

/// Create Trackbars
char TrackbarName1[50];
char TrackbarName2[50];
char TrackbarName3[50];

void print_size(Mat img)
{
	cout<<"Image size: "<<img.cols<<" x "<<img.rows<<endl;
	return;
}

/**
 * @function on_trackbar_sky_color
 * @brief This function implements the color based sky segmentation
 * By default blue sky color is in range (for BGR images) (130, 0, 0) <--> (255,255, 255)
 * This blue color range can be adjusted using slider
 */
void on_trackbar_sky_color( int, void* )
{

	// Make a clone of original image
	Mat copySrc = src1.clone();
	Mat skyImg;

	// Do the color base sky segmentation	
	inRange(copySrc, Scalar(skycolor_min, 0, 0), Scalar(skycolor_max, 255, 255), skyImg);

	Mat temp;
       	copySrc.copyTo(temp, 255 - skyImg);
	
	imshow( "Color based sky detection", temp);
	
	return;
}


/**
 * @function Calculate_border
 * 
 * This function implement the Calculate_border as describe in the paper
 * http://journals.sagepub.com/doi/pdf/10.5772/56884
 *
 * Return the sky mask that can be applied on original image
 */

Mat  Calculate_border(Mat grad, int threash_hold, std::vector<uint8_t> &border)
{
	// Gradient image is input to this function	
	Mat skyMask(grad.rows, grad.cols, CV_8UC1);

	skyMask = Scalar(1);  //used only Scalar.val[0]
	
	for (int j = 0; j < grad.cols; j++)
	{
		border[j] = grad.rows;
		
		for (int i =0; i < grad.rows; i++)
		{
			skyMask.at<uint8_t>(i,j) = 0;
			
			if (((int)grad.at<uint8_t>(i, j)) > threash_hold)
			{
				border[j] = i;
				break;
			}
		}
	}
	
	return skyMask;
}

/**
 * @function on_trackbar_threshold
 * @brief 
 * This function implements the sky segmentation based on paper
 * " Sky Region Detection in a Single Image for Autonomous Ground Robot Navigation"
 * http://journals.sagepub.com/doi/pdf/10.5772/56884
 *
 * Current implementation do not implement the complete paper, and provide a 
 * a slider for adjusting the treshold.	 
 */
void on_trackbar_threshold( int, void* )
{
	// Make a clone of original image
	Mat copySrc = src1.clone();
	Mat skyImg;


	// Do the sky segmentation	
	Mat graySrc;

	// As per paper " Sky Region Detection in a Single Image for Autonomous Ground Robot Navigation"
	// First convert the image to GrayScale
	cvtColor(copySrc, graySrc, cv::COLOR_BGR2GRAY);
	
	// calculate the "*derivatives*" in x and y directions. 
	// For this, we use the function cv::Sobel
	Mat grad_x, grad_y, grad;
	Mat abs_grad_x, abs_grad_y;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Sobel( graySrc, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	Sobel( graySrc, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	convertScaleAbs( grad_y, abs_grad_y );
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	std::vector<uint8_t> border(grad.cols);
	Mat skyMask = Calculate_border(grad, threshold_val, border);
	
	Mat temp;
	copySrc.copyTo(temp, skyMask);
	imshow( "Threshold based sky detection", temp);
	
	return;
}

int main( int argc, char** argv )
{
	std::cout<<"Sky detect"<<endl;	
	
	/// Read image ( same size, same type )
	std::cout<<"Loading image ... "<<argv[1]<<endl;

	src1 = imread(argv[1]);
		
	if( !src1.data ) { printf("Error loading src1 \n"); return -1; }

	/// Initialize values
	skycolor_min = 130;
	skycolor_max = 255;
	threshold_val = 30;

	/// Create Windows
	namedWindow("Color based sky detection", 1);
	namedWindow("Threshold based sky detection", 1);


	sprintf( TrackbarName1, "sky color min %d", skycolor_min );
	sprintf( TrackbarName2, "sky color max %d", skycolor_max );
	
	sprintf( TrackbarName3, "Threshold x %d", 255 );

	createTrackbar( TrackbarName1, "Color based sky detection", &skycolor_min, sky_slider1_max, on_trackbar_sky_color );
	createTrackbar( TrackbarName2, "Color based sky detection", &skycolor_max, sky_slider2_max, on_trackbar_sky_color );
	createTrackbar( TrackbarName3, "Threshold based sky detection", &threshold_val, sky_threshold_max, on_trackbar_threshold );

	/// Show some stuff
	on_trackbar_sky_color( sky_slider1_max, 0 );
	on_trackbar_threshold( threshold_val, 0);

	// Show the original image also
	imshow("Original", src1);

	/// Wait until user press some key
	waitKey(0);
	return 0;
}

