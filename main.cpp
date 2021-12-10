#include "matrix.h"
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

int main(int argc, char* argv[]) {
	std::string operation = argv[1];
	std::string outfile = argv[2];
	std::string file1 = argv[3];
	std::string file2;
	if (argc > 4) {
		file2 = argv[4];
	}
		
	Matrix* matrix1 = new Matrix();
	Matrix* matrix2 = new Matrix();
	Matrix* new_matrix;

	read_file(matrix1, file1);
	if (operation != "determinant" && operation != "inverse") {
		read_file(matrix2, file2);
	}

	if (operation == "add") {
		new_matrix = matrix1->add(matrix2);
		new_matrix->write_to_file(outfile);
		delete new_matrix;
	}
	else if (operation == "subtract") {
		new_matrix = matrix1->subtract(matrix2);
		new_matrix->write_to_file(outfile);
		delete new_matrix;
	}
	else if (operation == "multiply") {
		new_matrix = matrix1->multiply(matrix2);
		new_matrix->write_to_file(outfile);
		delete new_matrix;
	}
	else if (operation == "determinant") {
		std::cout << Matrix::CalcDet(matrix1->Transform_to_vector());
	}
	else if (operation == "inverse") {
		std::vector<std::vector<double>> new_vec = matrix1->Inverse_matrix();
		std::ofstream output(outfile);
		for (int i = 0; i < new_vec.size(); i++) {
			for (int j = 0; j < new_vec[0].size(); j++) {
				output << new_vec[i][j] << " ";
			}
			output << "\n";
		}
		output.close();
	}

	delete matrix1;
	delete matrix2;
}