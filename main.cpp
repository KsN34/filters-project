#include <iostream>
#include <string>
#include <vector>
#include "Image_Class.h"
#include <stdexcept>
using namespace std;

void grey_scale()
{
    string image_name;
    cout << "please enter your colored image name " << "\n";
    cin >> image_name;
    Image image(image_name);


    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            unsigned int avg = 0;

            for (int k =0; k < image.channels; k++)
            {
                avg += image(i,j,k);
            }
            avg /= 3;
            for (int k = 0; k < 3; k++)
            {
                image(i,j,k) = avg;
            }
        }
    }

    cout << "please enter your new image name" << "\n";
    cout << "with a extension (jpg, tmp , png , tga)" << "\n";

    cin >> image_name;
    image.saveImage(image_name);
}

void black_and_white(){

    string filename;
    cout << "Please enter the colored image name to turn to black and white: " << "\n";
    cin >> filename;

    Image image(filename);

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; ++k) {
                avg += image(i, j, k);
               }
            avg = avg / 3;
            unsigned char median =127;
            if (avg < median){
                avg = 0;
            } else {
                avg = 200;
            }
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = avg;
            }
        }
    }

    cout << "Please enter the image name to store the new black and white image\n";
    cout << "and specify the extension .jpg, .bmp, .png, .tga: " << "\n";

    cin >> filename;
    image.saveImage(filename);
}

void invert_image(){

    string filename;
    cout << "Please enter the image name to invert it: " << "\n";
    cin >> filename;

    Image image(filename);


    for (int h = 0; h < image.height; ++h) {
        for (int w = 0; w < image.width; ++w) {
            for (int c = 0; c < image.channels; ++c) {
                image.setPixel(w, h, c, 255 - image.getPixel(w, h, c));
            }
        }
    }

    cout << "Please enter the image name to store the new image,\n";
    cout << "and specify the extension (.jpg, .bmp, .png, .tga): " << "\n";
    cin >> filename;

    image.saveImage(filename);
    cout << "Image saved successfully.\n";

}

void rotate_image(Image& image, int angle){
    if (angle % 90 != 0) {
        cout  << "Invalid angle, must be 90, 180, or 270" << '\n';
        throw    invalid_argument("Invalid angle, must be 90, 180, or 270");
    }

    int rotations = angle / 90;

    vector<unsigned char> tempData(image.width * image.height * image.channels);

    for (int r = 0; r < rotations; ++r) {
        for (int h = 0; h < image.height; ++h) {
            for (int w = 0; w < image.width; ++w) {
                for (int c = 0; c < image.channels; ++c) {
                    int newIndex = ((image.width - 1 - w) * image.height + h) * image.channels + c;
                    tempData[newIndex] = image.imageData[(h * image.width + w) * image.channels + c];
                }
            }
        }

        swap(image.width, image.height);
        memcpy(image.imageData, tempData.data(), image.width * image.height * image.channels);
    }
}

void image_crop(){
    string filename;
    cout << "Please enter the image you want to CROP: " << "\n";
    cin >> filename;

    Image image(filename);

    int x, y, W, H;
    cout << "Please enter x left corner: ";
    cin >> x;
    cout << "Please enter y left corner: ";
    cin >> y;
    cout <<"please enter W:";
    cin >> W;
    cout <<"please enter H:";
    cin >>H;
    x = min(image.width-1,x+350);
    y = min(image.height-1,y+350);
    W = min(image.width-1,x+600);
    H = min(image.height-1,y+600);
    int modernW = W-x+1;
    int modernH = H-y+1;
    Image cropped(modernW, modernH);
    for (int i = 0; i < modernW; ++i) {
        for (int j = 0; j < modernH; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                cropped(i,j,k) = image(x + i,y + j,k);
            }
        }
    }
    cout << "Please enter the new croped image name: \n";
    cout << "and specify the extension .jpg, .bmp, .png, .tga: " << "\n";
    cin >> filename;
    cropped.saveImage(filename);
}

int main() {

    cout << "choose a filter from 1 to 10" << "\n";
    cout << "1) Grey scaled filter\n"
            "2) white & black filter\n"
            "3) invert image filter\n"
            "4) rotate image filter\n"
            "5) image crop \n";


    string choice;
    cin >> choice;

    if (choice == "1")
    {
        grey_scale();
    }
    else if(choice == "2")
    {
        black_and_white();
    }
    else if (choice == "3")
    {
        invert_image();
    }
    else if(choice == "4")
    {
        string image_name;
        cout << "please enter image name you want to rotate: " << "\n";
        cin >> image_name;
        Image image(image_name);

        int angle;
        cout << "Enter the rotation angle (90, 180, or 270): " << "\n";
        cin >> angle;

        rotate_image(image, angle);

        cout << "Pls enter image name to store new image\n";
        cout << "and specify extension .jpg, .bmp, .png, .tga: " << "\n";

        string filename;
        cin >> filename;
        image.saveImage(filename);
    }
    else if(choice == "5")
    {
        image_crop();
    }
    else
    {
        cout << "enter a valid number from 1 to 5" << "\n";
    }

    return 0;
}
