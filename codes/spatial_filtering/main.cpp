#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

typedef enum {sobel, sobel_x, sobel_y} mask_type_t;

class mask_t
{
public:
	int dimension;
	double **data;
public:
	mask_t(int dim, mask_type_t type)
	{
		dimension = dim;
		data = new double*[dimension];
			for(int i = 0; i < dimension; i++)
				data[i] = new double[dimension];
			
		if(type == sobel)
		{
			
		}	
		else if(type == sobel_x)
		{
			data[0][0] = -1;
			data[0][1] =  0;
		    data[0][2] =  1;
			data[1][0] = -2;
			data[1][1] =  0;
		    data[1][2] =  2;
			data[2][0] = -1;
			data[2][1] =  0;
		    data[2][2] =  1;
		}	
		else if(type == sobel_y)
		{
			data[0][0] = -1;
			data[0][1] = -2;
		    data[0][2] = -1;
			data[1][0] =  0;
			data[1][1] =  0;
		    data[1][2] =  0;
			data[2][0] =  1;
			data[2][1] =  2;
		    data[2][2] =  1;
		}	
	}

	~mask_t()
	{
		for (int i = 0; i < dimension; i++)
		    delete [] data[i];
		delete []data;
		cout << "Memoria liberada" <<endl;
	}

	void show()
	{
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
				cout <<"  "<<data[i][j];
			cout<<endl;
		}
	}
};


int main(int argc, char *argv[])
{
	mask_t mask(3, sobel_x);
	mask.show();
	/*Mat imagen = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("Display Window",WINDOW_NORMAL);
	imshow("Display Window",imagen);

	namedWindow("sobel window", WINDOW_NORMAL);
	imshow()
	waitKey(0);*/
    return 0;
}






