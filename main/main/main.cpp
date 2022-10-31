//  Developer: Baturay Özgürün
//  Copyright © 2018 Baturay Özgürün
//  If you have any requests or questions please do not hesitate to contact me via baturay.ozgurun@gmail.com

//  Project Name: Computer Vision -- RGB to HSV Color Conversion Algorithm with OpenCV
//  Project Desciption: This project aims to convert object and scene images from RGB to HSV and present the corresponding Hue (H), Saturation (S), and Value (V) channels. The H channel is related to the wavelength of the incident light and is measured in degrees from 0 to 360. A color thresholding image is also generated in the code using the H channel and user input between 0 and 360.

//Include Headers
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

//Include Namespaces
using namespace cv;
using namespace std;

int main(){
    Mat Image, HSVImage,SmoothedImage; //Create matrix named Image HSVImage and SmoothedImage
    int choice,smoothing; //Define variables for the image choice and the preference for the image smoothing
    
    cout << "Please select the image. Press 1 for Object or 2 Scene. Otherwise, the program is ended: " << endl;
    cin >> choice; //get the image choice as object or scene
    
    if (choice==1) //object choice
        Image = imread("data/object.jpg"); //read the image
    else if (choice==2) //scene choice
        Image = imread("data/scene.jpg"); //read the image
    else
        return 0;
    
    cout << "Do you want to smooth the image? If so, press 1. " << endl;
    cin >> smoothing; //smooting preference
    
    if (smoothing==1){
        Mat kernel = Mat::ones(3,3, CV_32FC1)/9; //define (3 x 3) matrix for the average filtering
        filter2D(Image,SmoothedImage,-1,kernel); //Filter Image and assign the averaged values to the SmoothedImage
        Image=SmoothedImage; //Values in SmoothedImage is copied to the Image variable
    }
    
    cvtColor(Image,HSVImage,CV_RGB2HSV); //Find HSV Image using opencv function
    vector<Mat> channels,ImageChannels; //define variables
    split(HSVImage,channels); //Values in HSV Image are seperated into channels
    split(Image,ImageChannels); //Values in Original Image are seperated into channels
    
    //Show Images
    imshow("Image", Image);
    imshow("HSV Image", HSVImage);
    imshow("H - Channel 0", channels[0]);
    imshow("S - Channel 1", channels[1]);
    imshow("V - Channel 2", channels[2]);
    
    //Write Images
    if (choice==1){ //object choice
        imwrite("results/object/HSV.jpg", HSVImage);
        imwrite("results/object/H Channel.jpg", channels[0]);
        imwrite("results/object/S Channel.jpg", channels[1]);
        imwrite("results/object/V Channel.jpg", channels[2]);
    }
    else if (choice==2){ //scene choice
        imwrite("results/scene/HSV.jpg", HSVImage);
        imwrite("results/scene/H Channel.jpg", channels[0]);
        imwrite("results/scene/S Channel.jpg", channels[1]);
        imwrite("results/scene/V Channel.jpg", channels[2]);
    }
    else
        return 0;
    
    waitKey(100); //wait to show images
    
    int colorprew,colorprewinput; //Define variables to enter color value by the user.
    do{
        destroyAllWindows(); //Close all images
        cout << "Please, enter the prefered color in angle such as 180 for red, 90 for green, 30 for blue or any number in [0,360]: " << endl;
        cout << "Quit: Enter out of [0 360] like -1 or 400: " << endl;
        cin >> colorprewinput; //get color choice or Hue value
        
        if (colorprewinput>=0 && colorprewinput<=360){ //Check color choice is in this interval
            colorprew=colorprewinput*255/360; //Transform color choice into the pixel value
            cout << "Color in pixel value = " << colorprew << endl << endl; //Show the chosen pixel value
            
            int Row, Column; //define row and column variables of the H channel
            Row=channels[0].rows; //find the row value for the H channel
            Column=channels[0].cols; //find the column value for the H channel
            Mat Mask = Mat::zeros(Row,Column, CV_8UC1); //Define zeros matrix
            Mat HSV=channels[0]; //copy H channel to the HSV variable
            
            for (int i = 0; i < Row-1; i++){
                for (int j = 0; j < Column-1; j++){
                    if ((colorprew-10)<0){
                        colorprew=colorprew+255; //complete values for the negative values because Hue is defined as a circle
                    } else if ((colorprew+10)>255){
                        colorprew=colorprew-255; //complete values out of boundary values because Hue is defined as a circle
                    }
                    if (HSV.at<uchar>(i,j)>colorprew-10 && HSV.at<uchar>(i,j)<colorprew+10){
                        Mask.at<uchar>(i,j)=1; //when H is withing the threshold make Mask as 1 for the defined locations
                    }
                }
            }
            
            Mat Color = Mat::zeros(Row,Column, CV_8UC3); //define zeros matix
            vector<Mat> ColorChannel;
            split(Color,ColorChannel); //split Color matrix into channels
            
            ColorChannel[0]=ImageChannels[0].mul(Mask); //Using Mask values diminish or duplicate red values
            ColorChannel[1]=ImageChannels[1].mul(Mask); //Using Mask values diminish or duplicate green values
            ColorChannel[2]=ImageChannels[2].mul(Mask); //Using Mask values diminish or duplicate blue values
            
            Mat ChoiceColor; //define matrix as name ChoiceColor
            merge(ColorChannel,ChoiceColor); //Merge RGB channels
            imshow("Original Image", Image); //Show the Original Image
            imshow("Threshold Image",ChoiceColor); //Show the Threshold Image
            
            if (choice==1){ //object choice
                imwrite("results/object/Original Image.jpg", Image); //Write the Original Image
                imwrite("results/object/Threshold Image.jpg", ChoiceColor); //Write the Threshold Image
            }
            else if (choice==2){ //scene choice
                imwrite("results/scene/Original Image.jpg", Image); //Write the Original Image
                imwrite("results/scene/Threshold Image.jpg", ChoiceColor); //Write the Threshold Image
            }
            else
                return 0;
            
            waitKey(100); //wait to show images
        }
    } while (colorprewinput>=0 && colorprewinput<=360); //if entered color choice by the user within the Hue values, continue the showing threshold images
    return 0;
}
