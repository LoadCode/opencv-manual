#include <iostream>
#include <cstring>



using namespace std;



template <typename T>
void get_n_bits_array(unsigned char *byte_array, T stream)
{
	const unsigned char BYTE_SIZE = 8;
	unsigned int nbits = (sizeof stream) * BYTE_SIZE;
	for(unsigned int i = 0, u = nbits-1; i <nbits; i++,u--)
		byte_array[i] = (stream >> u) & 1;
}

template <typename T>
void get_value_from_n_bits_array(unsigned char *byte_array, T &output_value)
{
	const unsigned char BYTE_SIZE = 8;
    size_t nbits = sizeof(T) * BYTE_SIZE;
	output_value = 0;
	for (size_t i = 0, u = nbits-1; i < nbits; i++, u--)
		output_value += byte_array[i]<<u;
}

int main(int argc, char *argv[])
{
	unsigned char byte_array[100];
	
    //string name("Julio Cesar echeverr marulanda es una graaaan persona que estudia ingenieria electronica y disfruta de la programación");
	/*string name("Julio");
	char *cname = new char[500];
	cout << name[0] <<endl;
	cout <<"sizeof(name[0]) = "<<sizeof name[0] <<endl;
	strcpy(cname, name.c_str());*/
	
	/*ASCII value of J = 74 = b0100_1010*/
	// print cname[0] bits
	/*
	for (unsigned int i = 0; i < name.length(); i++)
	{
		get_n_bits_array(byte_array, cname[i]);
		for (int j = 0; j < 8; j++)
			cout <<(int)byte_array[j];
		cout<<endl;
	}
	delete []cname;
	cname = NULL;*/
	unsigned int h = 855649;
	get_n_bits_array(byte_array, h);
	for (unsigned int j = 0; j < sizeof(int)*8; j++)
			cout <<(int)byte_array[j];
	cout <<endl;
	unsigned int w;
    get_value_from_n_bits_array(byte_array, w);
	cout<<"w = "<<w<<endl;
	
    return 0;
}






