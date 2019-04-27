/****************************************************
 *                  Image Processor                 *
 *              Aeolus Robotics test 1/2            *
 *                                                  *
 *                                                  *  
 *                                                  *
 * Author: José Jaramillo                           *
 ****************************************************/
#include "main.cpp" 
#include <stdint.h>

using namespace cv;
int main(int argc, char **argv){

    if (argc != 6) {
        printf("usage: find_region <Image_Path> <Output_Path> <Row> <Column> <Greedy>\n");
        return -1;
    }

    Mat image;
    Mat imagebw;

    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); 
    region obj2(imagebw,(intptr_t)argv[4],(intptr_t)argv[3],(intptr_t)argv[5]);
    //obj2.save_pixels(argv[2]);






    return 0;
}