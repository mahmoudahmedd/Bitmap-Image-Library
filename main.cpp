#include <iostream>
#include  "image.h"

using namespace std;

int main()
{

	image a("test.bmp");
	a.darken_image(.5);
	a.lighten_image(.3);
	a.save_image("new.bmp");
	
	return 0;
}
	