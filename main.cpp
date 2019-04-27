/****************************************************
 *                  Image Processor                 *
 *              Aeolus Robotics test 1/2            *
 *                                                  *
 *                                                  *
 * The algorithm has two classes:                   *
 *  region: implements the find_region (to debug)   *
 *          and                                     *
 *          find_perimeter and save_pixels          *
 *          operations.                             *
 *  display:implements the display_image and        *
 *          display_pixels operations.              *
 *                                                  *
 * Author: José Jaramillo                           *
 ****************************************************/

#include <bits/stdc++.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std; 
using namespace cv;

//>>>>>>>>>> class: region <<<<<<<<<<

class region 
{ 
    public: 
    int id;
    Mat _region;
    

    // Constructors
    region(Mat image, int pixel_X, int pixel_Y,int greedy){		//Find region
	    //	Region can be find in a grayscale image 
        int reference_pixel_intensity= (int)image.at<uchar>(pixel_X,pixel_Y);
        cout << "Pixel intensity is: " << reference_pixel_intensity << endl;
        cout << "Image size: " << image.cols << " x " << image.rows << endl;

        _region = Mat::ones(image.rows,image.cols, CV_8UC1);
        cout << "Image size: " << _region.cols << " x " << image.rows << endl;

        checkPixel(image,_region,pixel_X,pixel_Y,reference_pixel_intensity,greedy);
        
        checkSurroundingPixels(image,_region,pixel_X,pixel_Y,reference_pixel_intensity,greedy);



	}

	region(const region& reg){		//Find perimeters

        _region = Mat::ones(reg._region.rows,reg._region.cols, CV_8UC1);

        // iterate over all pixels and find borders

        for (int i=0; i<_region.rows; i++){
            for (int j=0; j< _region.cols; j++){
                if(border(_region, reg._region, i, j)){
                    _region.at<uchar>(i,j)= 255;
                }
                

            }
        }


	}


    // Member Functions() 
    void save_pixels(std::string directory){
        imwrite(directory,_region);

    }

    inline Mat getRegion(){

        return _region;
    }


    private:

    bool checkPixel(Mat image, Mat& _region, int position_X, int position_Y, int reference_intensity, int greedy){
        int pixel_intensity= (int)image.at<uchar>(position_X,position_Y);
        //  check if pixel intensity is close to the reference pixel and add to region
        //cout << "Pos x " << position_X  <<  " pos y "<< position_Y<<endl;
        if (pixel_intensity > reference_intensity-greedy && pixel_intensity < reference_intensity+greedy){
            _region.at<uchar>(position_X,position_Y)= 255;
            return true;
            //cout << "Pos x " << position_X  <<  " pos y "<< position_Y<<endl;
        }
        return false;
    }

    bool checkSurroundingPixels(Mat image, Mat& _region, int position_X, int position_Y, int reference_intensity, int greedy){
        if(position_X-1>=0){
            if( (int)_region.at<uchar>(position_X-1,position_Y)==1 &&
                checkPixel(image,_region,position_X-1,position_Y,reference_intensity,greedy)){

                checkSurroundingPixels(image,_region,position_X-1,position_Y,reference_intensity,greedy);

            }

        }

        // TODO: To debug, segmentation error
        /*if(position_X+1<_region.rows){
            if( (int)_region.at<uchar>(position_X+1,position_Y)==1 &&
                checkPixel(image,_region,position_X+1,position_Y,reference_intensity,greedy)){
                    
                checkSurroundingPixels(image,_region,position_X+1,position_Y,reference_intensity,greedy);

            }

        }*/
        if(position_Y-1>=0){
            if( (int)_region.at<uchar>(position_X,position_Y-1)==1 &&
                checkPixel(image,_region,position_X,position_Y-1,reference_intensity,greedy)){

                checkSurroundingPixels(image,_region,position_X,position_Y-1,reference_intensity,greedy);

            }
        }
        if(position_Y+1<_region.cols){
            if( (int)_region.at<uchar>(position_X,position_Y+1)==1 &&
                checkPixel(image,_region,position_X,position_Y+1,reference_intensity,greedy)){
                    
                checkSurroundingPixels(image,_region,position_X+1,position_Y,reference_intensity,greedy);

            }

        }
        

        
    }

    // Check if pixel is border
    bool border (Mat& _region, Mat origin, int position_X, int position_Y){
        if((int)origin.at<uchar>(position_X,position_Y)==1){
            return false;
        }

        if(position_X-1>=0){
            if( (int)origin.at<uchar>(position_X-1,position_Y)==1){

                return true;

            }

        }

        
        if(position_X+1<origin.rows){
            if( (int)origin.at<uchar>(position_X+1,position_Y)==1){

                return true;

            }

        }

        if(position_Y-1>=0){            
            if( (int)origin.at<uchar>(position_X,position_Y-1)==1){

                return true;

            
            }
        }
        if(position_Y+1<origin.cols){
            if( (int)origin.at<uchar>(position_X,position_Y+1)==1){

                return true;

            }
        }

    }

}; 

//>>>>>>>>>> class: display <<<<<<<<<<

class display 
{ 
    public: 
    int id;
    
    // Constructors
    display(Mat image){		//display_image
        namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
        imshow( "Display window2", image );                   // Show our image inside it.

        waitKey(0);



	}

	display(region& reg){		//display_pixels 
        namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
        imshow( "Display window2", reg._region );                   // Show our image inside it.

        waitKey(0);

	}

	display(Mat image, region& reg){		//display_image with display_pixels
        
        for (int i=0; i<reg._region.rows; i++){
            for (int j=0; j<reg._region.cols; j++){
                if((int)reg._region.at<uchar>(i,j)==255){
                    image.at<uchar>(i,j*3)= 255;
                }
                

            }
        }
        namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
        imshow( "Display window2", image );                   // Show our image inside it.

        waitKey(0);

	}


}; 
  
int main() { 


    // Read image from disk (taken from opencv tutorials)
 
    Mat image;
    Mat imagebw;

    image = imread("ship.jpg", CV_LOAD_IMAGE_COLOR); 
    imagebw = imread("ship.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    if(! image.data ){                              // Check for invalid input
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }




    region obj2(imagebw,500,450,20);
    obj2._region;
    region obj3(obj2);
    display image1(image);
    display image2(obj2);
    display image3(obj3);
    display image4(image,obj2);


    return 0; 

}
