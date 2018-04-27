#include <iostream>
#include <string>
#include <iomanip>
#include  "image.h"

using namespace std;

image img;
string inputFile, inputFile2, outputFile;
char choice;
string checkString(string _s)
{
    if( _s.find(".bmp") == string::npos )
        _s.append(".bmp");
    return _s;
}
void displayStatus()
{
    cout << setw(20) << "File name: " << checkString(inputFile) << setw(20) << ", File size: " << img.get_size() <<" Byte" << endl;
    cout << setw(20) << "Image height: " << img.get_height() << setw(20) << ", Image width: "<< img.get_width() << ", Pixel count: " << img.pixel_count() << endl;
    cout << endl;
}
void displayHello()
{
    cout << "NOTE : This program only supports 24-bits per pixel bitmap format files." << endl;
    cout << "Ahlan ya user ya habibi :)" << endl << endl;
}
void displayError()
{
    system("CLS");
    cout << "Erorr...Try Again." << endl << endl;
}
void displayDone()
{
    system("CLS");
    cout << "Done!" << endl << endl;
}
void displayBye()
{
    system("CLS");
    cout << "Bye..." << endl;
}
void displayMenu()
{
    cout << "Please select a filter to apply it on the image or 0 to exit:" << endl << endl;
    cout << "1 - Black & White Filter\n";
    cout << "2 - Invert Filter\n";
    cout << "3 - Merge Filter\n";
    cout << "4 - Flip Image\n";
    cout << "5 - Rotate Image\n";
    cout << "6 - Darken and Lighten Image\n";
    cout << "7 - Detect Image Edges\n";
    cout << "8 - Enlarge Image\n";
    cout << "9 - Shrink Image\n";
    cout << "a - Mirror Image\n";
    cout << "b - Shuffle Image\n";
    cout << "c - Blur Image\n";
    cout << "d - Grayscale\n";
    cout << "l - Load another image\n";
    cout << "s - Save the image to a file\n";
    cout << "0 - Exit\n" << endl;
    if(!img)
        cout << "image::set_image() Warning - select (l) to load image." << endl << endl;
    else
        displayStatus();
    cout << ">> ";
}
int main()
{
    displayHello();
    while(true)
    {
        displayMenu();
        cin>>choice;
        switch (choice)
        {
        case '0':
            displayBye();
            return 0;
        case '1':
        {
            img.BW_image();
            displayDone();
            break;
        }
        case '2':
        {
            img.invert_image();
            displayDone();
            break;
        }
        case '3':
        {
            image img2;
            cout << "Enter file name(2): "<<endl<<endl<<">>> ";
            cin>>inputFile2;
            img2.set_image(checkString(inputFile2));
            img.merge_image(img2);
            break;
        }
        case '4':
        {
            char temp;
            cout<<"Flip (h)orizontally or (v)ertically?"<<endl<<endl<<">>> ";
            cin>>temp;
            img.flip_image(temp);
            break;
        }
        case '5':
        {
            int temp;
            cout<<"Do you want to rotate (90), (180) or (270) degrees?"<<endl<<endl<<">>> ";
            cin>>temp;
            switch (temp)
            {
            case 90 :
                img.rotate_image(1);
                displayDone();
                break;
            case 180:
                img.rotate_image(2);
                displayDone();
                break;
            case 270:
                img.rotate_image(3);
                displayDone();
                break;
            default :
                displayError();
                cin.clear();
                break;
            }
            break;
        }
        case '6':
        {
            char temp;
            cout<<"Do you want to (d)arken or (l)ighten?"<<endl<<endl<<">>> ";
            cin>>temp;
            switch (temp)
            {
            case 'd' :
                img.darken_image();
                displayDone();
                break;
            case 'l' :
                img.lighten_image();
                displayDone();
                break;
            default  :
                displayError();
                cin.clear();
                break;
            }
            break;
        }
        case '7':
        {
            img.detect_image();
            displayDone();
            break;
        }
        case '8':
        {
            int temp;
            cout<<"Which quarter 1, 2, 3 or 4?"<<endl<<endl<<">>> ";
            cin>>temp;
            switch (temp)
            {
            case 1   :
                img = img.enlarge_image(0,0);
                displayDone();
                break;
            case 2   :
                img = img.enlarge_image(0,img.get_width()/2);
                displayDone();
                break;
            case 3   :
                img = img.enlarge_image(img.get_height()/2,0);
                displayDone();
                break;
            case 4   :
                img = img.enlarge_image(img.get_height()/2,img.get_width()/2);
                displayDone();
                break;
            default  :
                displayError();
                cin.clear();
                break;
            }
            break;
        }
        case '9':
        {
            int temp;
            cout<<"Shrink to 1/(2), 1/(3) or 1/(4)?"<<endl<<endl<<">>> ";
            cin>>temp;
            switch (temp)
            {
            case 2   :
                img = img.shrink_image(2);
                displayDone();
                break;
            case 3   :
                img = img.shrink_image(3);
                displayDone();
                break;
            case 4   :
                img = img.shrink_image(4);
                displayDone();
                break;
            default  :
                displayError();
                cin.clear();
                break;
            }
            break;
        }
        case 'a':
        {
            char temp;
            cout<<"Do you want to Mirror (u)pper, (d)own, (r)ight or (l)eft?"<<endl<<endl<<">>> ";
            cin>>temp;
            switch (temp)
            {
            case 'u' :
                img.mirror_upper_image();
                displayDone();
                break;
            case 'd' :
                img.mirror_lower_image();
                displayDone();
                break;
            case 'r' :
                img.mirror_right_image();
                displayDone();
                break;
            case 'l' :
                img.mirror_left_image();
                displayDone();
                break;
            default  :
                displayError();
                cin.clear();
                break;
            }
            break;
        }
        case 'b':
        {
            int arr[4];
            cout << "Input order of shuffle ex(4,1,2,3): "<<endl<<endl<<">>> ";
            for(int k = 0; k < 4; k++)
                cin>>arr[k];
            img = img.shuffle_image(arr);
            displayDone();
            break;
        }
        case 'c':
        {
            img.blur_image();
            displayDone();
            break;
        }
        case 'd':
        {
            img.greyscale_image();
            displayDone();
            break;
        }
        case 's':
        {
            cout << "Enter file name: "<<endl<<endl<<">>> ";
            cin>>outputFile;
            img.save_image(checkString(outputFile));
            displayDone();
            break;
        }
        case 'l':
        {
            cout << "Enter file name: "<<endl<<endl<<">>> ";
            cin>>inputFile;
            img.set_image(checkString(inputFile));
            displayDone();
            break;
        }
        default:
        {
            displayError();
            cin.clear();
            break;
        }
        }
    }
    return 0;
}

