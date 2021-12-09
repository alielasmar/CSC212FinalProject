#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Node {
	/*
	The Node class consists of row, col and data. Row and col represent the row and column
	of a specific element in the matrix. The data field is a nonzero number in the matrix.
	Next is the pointer which points to the next element in the matrix linked list.
	*/
	int row;
	int col;
	int data;
	Node* next;	

	friend class Matrix;

public:
	Node();
	Node(int row, int col, int data); // constructor with arguments
	~Node();

	int get_row();			// returns the row value
	int get_col();			// returns the col value
	int get_data();			// returns the data
	void set_data(int d);	// sets the data
	Node* get_next();		// returns the next pointer
	void set_next(Node* n);	// sets the next pointer
	bool pos_bigger(Node* node2);	// compares the positions of two nodes, for example, (5,1) is bigger than (2,3) [(row, col)]

};

class Matrix {
	/*
	The Matrix class is the linked list itself. It holds information for the number of rows 
	and columns in the matrix itself as well as the head and tail of the linked list.
	*/
	Node* head;
	Node* tail;
	int num_rows;
	int num_cols;
	void place(int row, int col, int data, Matrix* newMatrix); // helper method which adds data into a certain index of the new matrix
public:
	Matrix();
	Matrix(int num_rows, int num_cols);
	~Matrix();

	Node* get_head();							// returns the head pointer
	void push_back(int row, int col, int data);	// adds a node to the linked list
	void write_to_file(std::string filename);	// writes a matrix linked list to a file (converts it)
	void set_num_rows(int num);					// sets the number of rows (used when reading into matrix)
	int get_num_rows();							// returns the number of rows
	void set_num_cols(int num);					// sets the number of columns (used when reading into matrix)
	int get_num_cols();							// returns number of columns
	Matrix* add(Matrix* matrix2);				// adds two matrices
	Matrix* subtract(Matrix* matrix2);			// subtacts one matrix from another
	Matrix* multiply(Matrix* matrix2);			// multiplies two matrices

	int get_exacl_data(int inrow, int incol);
	std::vector<std::vector<int>> Transform_to_vector();
	private static int CalcDet(std::vector<std::vector<int>> matrix);
	void Inverse_matrix (std::vector<std::vector<int>>* inv_vec)

};

#endif
