#include <iostream>

class matrix
{
public:
	int rows;
	int cols;
	double **data;

	matrix(const double *input_data[], int _rows, int _cols);
	~matrix();
	void show();
};


matrix::matrix(const double *input_data[], int _rows, int _cols)
{
	rows = _rows;
	cols = _cols;
	data = new double*[rows];
	for (int i = 0; i < rows; i++)
		data[i] = new double[cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			data[i][j] = input_data[i][j];
}

void matrix::show()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			std::cout<<"  "<<data[i][j];
		std::cout<<std::endl;
	}
}


int main(int argc, char *argv[])
{
	double mat [][3] = {{1,2,3},{4,5,6}};
	matrix m1(&mat[0],2,3);
    return 0;
}




