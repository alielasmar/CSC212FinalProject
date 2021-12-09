#include "matrix.h"
#include "matrix.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

void read_file(Matrix* matrix, std::string filename) {
	std::ifstream input(filename);
	std::string line;
	int row = 0, col = 0;
	int data;

	while (getline(input, line)) {
		std::istringstream ss(line);
		while (ss >> data) {
			if (data != 0) {
				// only pushes data into the linked list if it != 0
				matrix->push_back(row, col, data);
			}
			col++;
		}
		matrix->set_num_cols(col);
		col = 0;
		row++;
	}
	matrix->set_num_rows(row);
	input.close();
}

int main() {
	Matrix* matrix1 = new Matrix();
	Matrix* matrix2 = new Matrix();
	std::string filename = "matrix1.txt";
	read_file(matrix1, filename);
	read_file(matrix2, "matrix2.txt");
	std::vector<std::vector<int>> new_vec;

	new_vec=matrix1->Transform_to_vector();

	for(int i=0; i<new_vec.size(); i++){
		for(int x=0; x < new_vec[i].size();x++){
			std::cout<< new_vec[i][x]<< " "<< " ";
		}

		std::cout<</*new_vec[i].size() <<*/" "<<std::endl;
	}


	//int x = Matrix::CalcDet(new_vec);
	//std::cout<< x <<std::endl;

	std::vector<std::vector<double>> inv_vec;
	inv_vec =matrix1->Inverse_matrix();

	for(int i=0; i<inv_vec.size(); i++){
		for(int x=0; x < inv_vec[i].size();x++){
			std::cout<< inv_vec[i][x]<< " "<< " ";
		}

		std::cout<</*new_vec[i].size() <<*/" "<<std::endl;
	}



	Matrix* new_matrix = matrix1->add(matrix2);


	new_matrix->write_to_file("matrix_new.txt");

	delete matrix1;
	delete matrix2;
	delete new_matrix;
}