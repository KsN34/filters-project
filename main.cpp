#include <iostream>
#include <string>
#include <vector>
#include "Image_Class.h"
#include <stdexcept>
#include <cmath>
#include <fstream>
using namespace std;

// Grayscale filter(1)
void grey_scale(Image& image){

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
}

// Black an dWhite filter (2)
void black_and_white(Image& image){

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

}

// invert Image filter(3)
void invert_image(Image& image){

    for (int h = 0; h < image.height; ++h) {
        for (int w = 0; w < image.width; ++w) {
            for (int c = 0; c < image.channels; ++c) {
                image.setPixel(w, h, c, 255 - image.getPixel(w, h, c));
            }
        }
    }


}

// Merge Images filter(4)
void merge_photos(Image image)
{
    string image_name2;
    cout << "please enter your 2nd image name " << "\n";
    cin >> image_name2;
    Image image2(image_name2);
    Image merged_image(image.width,image.height);

    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            double color;
            for (int k = 0; k < 3; ++k)
            {
                color = (1 - 0.5) * image(i,j,k) + 0.5 * image2(i,j,k);
                merged_image(i,j,k) = color;
            }
        }
    }
    merged_image.saveImage("mergedimage.png");

}

// Flip Image filter(5)
void flip(Image& image)
{
    cout << "please enter 1 to flip horizontal or 2 to flip vertical\n";
    string num;
    cin >> num;
    //flipped horizontal
    Image flipped_horizontal(image.width, image.height);
    if (num == "1"){

        for (int i = 0; i < image.width; i++) {
            for (int j = 0; j < image.height; j++) {
                for (int k = 0; k < image.channels; k++) {
                    flipped_horizontal(image.width-1-i,j ,k) = image(i,j,k);
                }
            }
        }
        for (int i = 0; i < image.width; i++)
        {
            for (int j = 0; j < image.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    image(i,j,k) = flipped_horizontal(i,j,k);
                }

            }
        }


    }else if (num == "2"){

        //flipped vertical
        Image flipped_vertical(image.width, image.height);
        for (int i = 0; i < image.width; i++) {
            for (int j = 0; j < image.height; j++) {
                for (int k = 0; k < image.channels; k++) {
                    flipped_vertical(i,image.height-1-j,k) = image(i,j,k);
                }
            }
        }
        for (int i = 0; i < image.width; i++)
        {
            for (int j = 0; j < image.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    image(i,j,k) = flipped_vertical(i,j,k);
                }

            }
        }

    }else{
        cout << "enter a valid number" << "\n";
    }




}

// Image Rotate filter (6)
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

// Darken and Lighten filter(7)
void darken_lighten(Image& image) {
    cout << "if you want to bright the image enter number > 1\n"
            "if you want to dark the image enter number < 1\n";

    float bright;
    cin >> bright;
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < image.channels; k++) {
                int x = image(i, j, k) * (bright);

                if (x > 255)
                {
                    x = 255;
                }
                image(i,j,k) = x;
            }
        }
    }
}

// Image Crop filter(8)
void image_crop(Image& image){

    int x, y, W, H;
    cout << "Please enter x left corner: ";
    cin >> x;
    cout << "Please enter y right corner: ";
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
    for (int i = 0; i < modernW; ++i){
        for (int j = 0; j < modernH; ++j){
            for (int k = 0; k < image.channels; ++k){
                cropped(i,j,k) = image(x + i,y + j,k);


            }
        }
    }

    cropped.saveImage("croppedimage.png");

}

// Image Frame filter (9)
void frame(Image& image, int frameThickness, unsigned char red, unsigned char green, unsigned char blue) {
    // Create vertical frame lines
    for (int x = 0; x < image.width; ++x) {
        for (int y = 0; y < frameThickness; ++y) {
            image.setPixel(x, y, 0, red);
            image.setPixel(x, y, 1, green);
            image.setPixel(x, y, 2, blue);
        }
        for (int y = image.height - frameThickness; y < image.height; ++y) {
            image.setPixel(x, y, 0, red);
            image.setPixel(x, y, 1, green);
            image.setPixel(x, y, 2, blue);
        }
    }

    // Create horizontal frame lines
    for (int y = frameThickness; y < image.height - frameThickness; ++y) {
        for (int x = 0; x < frameThickness; ++x) {
            image.setPixel(x, y, 0, red);
            image.setPixel(x, y, 1, green);
            image.setPixel(x, y, 2, blue);
        }
        for (int x = image.width - frameThickness; x < image.width; ++x) {
            image.setPixel(x, y, 0, red);
            image.setPixel(x, y, 1, green);
            image.setPixel(x, y, 2, blue);
        }
    }
}

// Detect Image Edges filter(10)
void edge(Image& image)
{

    Image edgeimg(image.width,image.height);

    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < 3; k++) {
                int horizontalSum = 0;
                int verticalSum = 0;

                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int ix = i + x;
                        int jy = j + y;
                        bool pixelExists = ix >= 0 && ix < image.width && jy >= 0 && jy < image.height;

                        if (pixelExists) {
                            // Horizontal Kernel
                            if (y == 0) {
                                horizontalSum += image(ix, jy, 0) * x * 2;
                            }
                            else {
                                horizontalSum += image(ix, jy, 0) * x;
                            }
                            // Vertical Kernel
                            if (x == 0) {
                                verticalSum += image(ix, jy, 0) * y * -2;
                            }
                            else {
                                verticalSum += image(ix, jy, 0) * y * -1;
                            }
                        }
                    }
                }

                int magnitude = sqrt( pow(horizontalSum, 2) + pow(verticalSum, 2) );
                if (magnitude > 30) {
                    edgeimg(i, j, k) = 0;
                }
                else {
                    edgeimg(i, j, k) = 255;
                }
            }
        }
    }
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                image(i,j,k) = edgeimg(i,j,k);
            }

        }
    }
}

// Image Resize filter(11)
void resize(Image& image)
{
    int Width, Height, x, y;
    float m,n;

    cout <<"please enter Width:";
    cin >> Width;
    cout <<"please enter Height:";
    cin >> Height;

    Image resize(Width, Height);
    m = static_cast<float>(image.width) / Width;
    n = static_cast<float>(image.height) / Height;
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++) {
            for (int k = 0; k < image.channels; k++) {
                resize(i, j, k) = image(static_cast<int>(i*m), static_cast<int>(j*n), k);
            }
        }
    }
    resize.saveImage("resizedimage.png");
}

// Blur Image filter (12)
void blurImage(Image& image, Image& blurredImage, int blurRadius) {
     // Blur horizontally
     for (int y = 0; y < image.height; y++) {
         for (int x = 0; x < image.width; x++) {
             int sumR = 0, sumG = 0, sumB = 0;
             int count = 0;

             for (int dx = -blurRadius; dx <= blurRadius; dx++) {
                 int px = x + dx;
                 if (px >= 0 && px < image.width) {
                     sumR += image(px, y, 0);
                     sumG += image(px, y, 1);
                     sumB += image(px, y, 2);
                     count++;
                     }
                 }

             blurredImage(x, y, 0) = sumR / count;
             blurredImage(x, y, 1) = sumG / count;
             blurredImage(x, y, 2) = sumB / count;
             }
         }

      // Blur vertically
     for (int y = 0; y < image.height; y++) {
         for (int x = 0; x < image.width; x++) {
             int sumR = 0, sumG = 0, sumB = 0;
             int count = 0;

             for (int dy = -blurRadius; dy <= blurRadius; dy++) {
                 int py = y + dy;
                 if (py >= 0 && py < image.height) {
                     sumR += blurredImage(x, py, 0);
                     sumG += blurredImage(x, py, 1);
                     sumB += blurredImage(x, py, 2);
                     count++;
                     }
                 }

             blurredImage(x, y, 0) = sumR / count;
             blurredImage(x, y, 1) = sumG / count;
             blurredImage(x, y, 2) = sumB / count;
             }
         }
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                image(i,j,k) = blurredImage(i,j,k);
            }

        }
    }
}


// Sunlight filter (13) "Bonus"
void sunlight(Image& image)
{
    for (int i = 0; i < image.width; ++i){
        for ( int j=0; j < image.height;++j){
            unsigned int blue =image(i,j,2)/2;
            unsigned int blue2 = blue * 0.72;
            image(i,j,2)=blue2;
        }
    }
}


// Purple Image filter(16) "Bonus"
void MakePurple(Image& image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {

            int blue = -(image(i, j, 2)/10) + image(i, j, 2);
            blue *= 1.4;
            if (blue > 255)
            {
                blue = 255;
            }
            else if (blue < 0)
            {
                blue = 0;
            }

            int red = -(image(i, j, 0)/8) + image(i, j, 0);
            red *= 1.2;
            if (red > 255)
            {
                red = 255;
            }
            else if (red < 0)
            {
                red = 0;
            }

            int green = -(image(i, j, 1)/3) + image(i, j, 1);
            green *= 1.2;
            if (green > 255)
            {
                green = 255;
            }
            else if (green < 0)
            {
                green *= 0;
            }


            image(i, j, 0) = red;
            image(i, j, 1) = green;
            image(i, j, 2) = blue;
        }
    }

}


// Image Redray filter(17) "Bonus"
void redrays(Image& image) {
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            unsigned char originalGreen = image.getPixel(x, y, 1);
            unsigned char originalBlue = image.getPixel(x, y, 2);

            image.setPixel(x, y, 1, 255 - originalBlue);
            image.setPixel(x, y, 2, 255 - originalGreen);

            image.setPixel(x, y, 0, 255);
        }
    }
}


// Image Skew filter(18) "Bonus"
void skew(Image& image)
{

    double angle,preangle;
    cout << "please enter the angle of the skew deviation" << "\n";
    cin >> preangle;
    angle = preangle * M_PI / 180;

    int nwidth = image.width + tan(angle) * image.height;

    Image skewed(nwidth,image.height);
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < image.channels; ++k)
            {

                int i2 = i + j * tan(angle);
                skewed(i2,j,k) = image(i,j,k);

            }

        }
    }

    skewed.saveImage("skewedimage.png");
}


// blur filter used in filter 10
void blur(Image& image, int blurRadius)
{
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {

            for (int c = 0; c < image.channels; c++) {
                int sum = 0;
                int count = 0;

                for (int dy = -blurRadius; dy <= blurRadius; dy++) {
                    for (int dx = -blurRadius; dx <= blurRadius; dx++) {

                        int px = x + dx;
                        int py = y + dy;

                        if (px >= 0 && px < image.width && py >= 0 && py < image.height) {

                            sum += image(px, py, c);
                            count++;
                        }
                    }
                }

                image(x, y, c) = sum / count;
            }
        }
    }
}

int main(){

//  Image nams input
    cout << "welcome to our filters program" << "\n";
    cout << "Please enter the image name you want to edit: " << "\n";
    string filename;
    cin >> filename;

    Image image(filename);

    //Main Menu
    while (true) {

        cout << " now please choose a filter from 1 to 5 or 0 to exit" << "\n";
        cout << "1) Grey scaled filter \n"
                "2) white & black filter \n"
                "3) invert image filter \n"
                "4) merge 2 photos filter \n" // error in saving
                "5) flip image filter \n"
                "6) rotate image filter \n"
                "7) darken & lighten filter \n"
                "8) crop image filter \n" // error in saving
                "9) adding frame filter \n" //no
                "10) detect image edge filter \n"
                "11) resize image filter \n" // error in saving
                "12) blur image filter \n"
                "13) sunlight filter \n"
                "14) pruple image filter \n"
                "15) redray filter \n"
                "16) image skewing filter \n"
                "0) save and exit \n";

        string new_filename;
        string choice;
        cin >> choice;


//       Filter 1
        if (choice == "1") {
            grey_scale(image);
        }

//       Filter 2
        else if (choice == "2") {
            black_and_white(image);
        }

//       Filter 3
        else if (choice == "3") {
            invert_image(image);
        }

//      filter 4
        else if (choice == "4" ){
            merge_photos(image);
        }

//      filter 5
        else if (choice == "5"){
            flip(image);
        }

//      filter 6
        else if (choice == "6") {
            int angle;
            cout << "Enter the rotation angle (90, 180, or 270): " << "\n";
            cin >> angle;

            rotate_image(image, angle);

        }

//      filter 7
        else if (choice == "7"){
            darken_lighten(image);
        }

//      filter 8
        else if (choice == "8") {
            image_crop(image);
        }

//      filter 9
        else if (choice == "9") {
            int frameColorChoice;
            cout << "Choose frame color:\n";
            cout << "1) Red\n";
            cout << "2) Blue\n";
            cout << "3) Green\n";
            cin >> frameColorChoice;

            unsigned char red, green, blue;
            switch (frameColorChoice) {
                case 1:
                    red = 255;
                    green = 0;
                    blue = 0;
                    break;
                case 2:
                    red = 0;
                    green = 0;
                    blue = 255;
                    break;
                case 3:
                    red = 0;
                    green = 255;
                    blue = 0;
                    break;
                default:
                    cerr << "Invalid color choice.  color (red)." << "\n";
                    red = 255;
                    green = 0;
                    blue = 0;
                    break;
            }

            int thickness;
            cout << "Enter the thickness of the frame: \n";
            cin >> thickness;

            frame(image, thickness, red, green, blue);

        }

//      filter 10
        else if (choice == "10"){
            grey_scale(image);
            blur(image,3);
            edge(image);
        }

//      filter 11
        else if (choice == "11"){
            resize(image);
        }

//      filter 12
        else if (choice == "12" ){
            Image blurredImage(image.width, image.height);

            int blurRadius;
            cout << "Enter the blur radius: \n";
            cin >> blurRadius;

            blurImage(image, blurredImage, blurRadius);


        }

//      filter 13
        else if (choice == "13" ){
            sunlight(image);
        }



//      filter 16
        else if (choice == "14"){
            MakePurple(image);
        }

//      filter 17
        else if (choice == "15"){
            redrays(image);
        }

//      filter 18
        else if (choice == "16"){
            skew(image);

        }


//      exit the program & saving the new image
        else if (choice == "0")
        {

            cout << "choose a new image name:" << "\n";
            cout << "and specify the extension .jpg, .bmp, .png, .tga: " << "\n";
            cin >> new_filename;
            image.saveImage(new_filename);
            exit(0);
        }

        else
        {
            cout << "enter a valid number from 1 to 5 or 0 to exit" << "\n";
        }
    }

}





