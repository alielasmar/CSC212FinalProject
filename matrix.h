#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <fstream>
#include <iostream>

class Node {
	int row;
	int col;
	int data;
	Node* next;	

	friend class Matrix;

public:
	Node();
	Node(int row, int col, int data);
	~Node();

	int get_row();
	int get_col();
	int get_data();
	void set_data(int d);
	Node* get_next();
	void set_next(Node* n);
	bool pos_bigger(Node* node2);

};

class Matrix {
	Node* head;
	Node* tail;
	int num_rows;
	int num_cols;

public:
	Matrix();
	~Matrix();
	Node* get_head();

	void push_back(int row, int col, int data);
	void write_to_file(std::string filename);
	void set_num_rows(int num);
	int get_num_rows();
	void set_num_cols(int num);
	int get_num_cols();
	Matrix* add(Matrix* matrix2);
};

#endif