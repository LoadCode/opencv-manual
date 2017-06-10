#include <iostream>
#include <CLI/CLI.hpp>
#include <cstring>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define BYTE_SIZE 8
#define PXL_LEN_STORAGE 10 //10 pixels to save the data length from the start
#define LSB_POSITION 0

void image_assembly(cv::Mat &output_image, uchar **pixels_array)
{
	for (int i = 0; i < output_image.rows; i++)
		for(int j = 0; j < output_image.cols;j++)
			output_image.at<uchar>(i,j) = pixels_array[i][j];
}

void get_image_pixels(cv::Mat &image, uchar **pixels_array)
{
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			pixels_array[i][j] = image.at<uchar>(i,j);
}


template <typename T>
unsigned int get_n_bits_array(unsigned char *byte_array, T stream)
{
	unsigned int nbits = (sizeof stream) * BYTE_SIZE;
	for(unsigned int i = 0, u = nbits-1; i <nbits; i++,u--)
		byte_array[i] = (stream >> u) & 1;
	return nbits;
}

template <typename T>
void get_value_from_n_bits_array(unsigned char *byte_array, T &output_value)
{
    size_t nbits = sizeof(T) * BYTE_SIZE;
	output_value = 0;
	for (size_t i = 0, u = nbits-1; i < nbits; i++, u--)
		output_value += byte_array[i]<<u;
}

template <typename T>
void assembly_bytes(unsigned char *byte_array,T &data)
{
	data = 0;
	size_t size = sizeof data;
	int byte_shift = size - 1;
	for(unsigned int i = 0; i < size; i++, byte_shift--)
	{
		data |= byte_array[i]<<(byte_shift*BYTE_SIZE);
	}
}

unsigned int steg_n_bytes_array(unsigned char **matrix, int rows, int cols, unsigned char *byte_array,unsigned int nbytes, unsigned int position)
{
	unsigned int array_length = nbytes * BYTE_SIZE;
	unsigned int last_position = 0;
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
		{
			matrix[i][j] = (matrix[i][j]&(~1))|byte_array[last_position];
			last_position ++;
			if(last_position == array_length)
				return last_position;
		}
	return last_position;
}

unsigned int inv_steg_n_bytes(unsigned char **matrix, unsigned int rows, unsigned int cols, unsigned int position, unsigned char *byte_array, unsigned int nbytes)
{
	
	unsigned int initial_row = floor(position/cols);
	unsigned int initial_col = position%cols;
	unsigned int byte_index  = 0;
	unsigned int bit_index   = BYTE_SIZE-1;
	unsigned int counter     = 0;
	std::cout << "Initial row and col = ("<<initial_row<<" "<<initial_col<<")"<<std::endl;
	for(unsigned int i = initial_row; i < rows; i++)
	{
		for (unsigned int j = initial_col; j < cols; j++)
		{
			byte_array[byte_index] |= (matrix[i][j]&1) << bit_index;
			counter++;
			bit_index--;
			if(counter == BYTE_SIZE)
			{
				counter = 0;
				bit_index = BYTE_SIZE-1;//shift from bit 7 (MSB)
				byte_index++;
				if(byte_index == nbytes)
					return 0;
			}
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{
	std::string image_name("Lenna.png");
	std::string data("Julio");
	unsigned int data_length = 0;
	int needed_pxl = 0;
	char *c_data = NULL;
	uchar **pixels_array = NULL;
	unsigned char byte_array[40];
	unsigned int byte_array_len = 0;
	cv::Mat image;
	cv::Mat output_image;
	
	CLI::App app{"Simple Steganography Application"};
	app.add_option("-f,--file", image_name, "The name+extension of the image");
	app.add_option("-d,--data", data, "The text data you want to hide");
	std::cout<<"Starting process"<<std::endl;
	
	try
	{
		app.parse(argc, argv);
	}catch(const CLI::ParseError &e)
	{
		return app.exit(e);
	}
	data_length = data.length();
	c_data = new char[data_length];
	strcpy(c_data, data.c_str());
	
	/*Read image*/
	image = cv::imread("Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::namedWindow("Display Window", cv::WINDOW_NORMAL);
	//cv::imshow("Display Window", image);
	
	pixels_array = new uchar*[image.rows];
	for (int i = 0; i < image.rows; i++)
		pixels_array[i] = new uchar[image.cols];
	
	/*Save pixels into an array*/
	get_image_pixels(image, pixels_array);
	
	output_image = cv::Mat(image.rows,image.cols,CV_8UC1);
	//image_assembly(output_image, pixels_array);
	
	//cv::namedWindow("Display 2", cv::WINDOW_NORMAL);
	//cv::imshow("Display 2", output_image);
	
	/*Compute capacity*/
	needed_pxl = data_length * BYTE_SIZE + PXL_LEN_STORAGE;
	if(needed_pxl > image.rows*image.cols)
	{
		std::cout <<"The data is bigger than the image file."<<std::endl;
		return 0;
	}
	
	/**********Apply steganography function*********/
	/*Save data length in the first 10 pixels*/
	byte_array_len = get_n_bits_array(byte_array, data_length);
	std::cout<<"Original first pixels values" <<std::endl;
	for (unsigned int i = 0; i < byte_array_len; i++)
		std::cout <<" "<<(int)pixels_array[0][i];
	std::cout<<std::endl;
    steg_n_bytes_array(pixels_array, image.rows, image.cols, byte_array, byte_array_len);

	//mostrar el stream de bits
	std::cout<<"byte_array = ";
	for (unsigned int i = 0; i < byte_array_len; i++)
		std::cout <<(int)byte_array[i];
	std::cout <<std::endl;
	
	//mostrar los primeros byte_array_len de pixels_array
	std::cout<<"First pixels values" <<std::endl;
	for (unsigned int i = 0; i < byte_array_len; i++)
		std::cout <<" "<<(int)pixels_array[0][i];
	std::cout<<std::endl;
	
	// recuperar el valor codificado en los 32 primeros pixels
	//(unsigned char **matrix, int rows, int cols, unsigned int position, unsigned char *byte_array, unsigned int nbytes)
	inv_steg_n_bytes(pixels_array, image.rows, image.cols, 0, byte_array, 4);
	//mostrar primeros 4 bytes	
	for (int i = 0; i < 4; i++)
		std::cout <<(int)byte_array[i];
	std::cout <<std::endl;
    unsigned int recuperado;
	assembly_bytes(byte_array,recuperado);
	std::cout <<"recuperado = "<<recuperado<<std::endl;
	/*Save data from 11th pixel*/
	
	
	
	std::cout <<"File name: "<<image_name<<std::endl;
	std::cout <<"Data to hide: "<<c_data<<std::endl;
	std::cout <<"strlen(data) = "<<strlen(c_data)<<std::endl;
	
	/*Free resources*/
	cv::waitKey(0);
	delete [] c_data;
	c_data = NULL;
	for (int i = 0; i < image.rows; i++)
		delete [] pixels_array[i];
	delete [] pixels_array;
	pixels_array = NULL;
	
	
	return 0;
}

