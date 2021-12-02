#include "matrix.h"

Node::Node() {
	this->row = 0;
	this->col = 0;
	this->data = 0;
	this->next = nullptr;
}

Node::Node(int row, int col, int data) {
	this->row = row;
	this->col = col;
	this->data = data;
	this->next = nullptr;
}

Node::~Node() {
	
}

int Node::get_row() {
	return row;
}

int Node::get_col() {
	return col;
}

int Node::get_data() {
	return data;
}

void Node::set_data(int d) {
	this->data = d;
}

Node* Node::get_next() {
	return next;
}

void Node::set_next(Node* n) {
	this->next = n;
}

bool Node::pos_bigger(Node* node2) {
	if (this->row > node2->get_row()) {
		return true;
	}
	if (this->row == node2->get_row() && this->col >= node2->get_col()) {
		return true;
	}
	return false;
}


Matrix::Matrix() {
	head = nullptr;
	tail = nullptr;
}

Matrix::~Matrix() {
	Node* current = head;
	Node* next;
	while (current != nullptr) {
		next = current->next;
		delete current;
		current = next;
	}
}

Node* Matrix::get_head() {
	return head;
}

void Matrix::push_back(int row, int col, int data) {
	if (head == nullptr) {
		head = new Node(row, col, data);
		tail = head;
	}
	else {
		tail->next = new Node(row, col, data);
		tail = tail->next;
	}
}

void Matrix::set_num_rows(int num) {
	num_rows = num;
}

int Matrix::get_num_rows() {
	return num_rows;
}

void Matrix::set_num_cols(int num) {
	num_cols = num;
}

int Matrix::get_num_cols() {
	return num_cols;
}

int Matrix::get_exacl_data(int inrow, int incol， matrix1){
	int maxrow = 0;
	int maxcol = 0;
	while(curr1 != nullptr){
		if(curr1->get_col()== incol()&& curr1->get_row()== inrow){
			return curr->get_data();
		}
		if(curr1->get_row()>maxrow){
			maxrow = curr1->get_row();
		}
		if(curr1->get_col()>maxcol){
			maxrow = curr1->get_col();
		}
		curr1 = curr1->next
	}
	if(maxrow>inrow && maxcol>incol){
		return 0;
	}else{
		std::cout<<"error, in " << inrow << "row and " << incol << "col, we can not find a number, please check! for now 0 will replace"
		return 0;
	}
}

// writes a matrix to a file
void Matrix::write_to_file(std::string filename) {
	std::ofstream output(filename);
	Node* curr = this->head;
	for (int i = 0; i < this->num_rows; i++) {
		for (int j = 0; j < this->num_cols; j++) {
			if (curr != nullptr && i == curr->row && j == curr->col) {
				output << curr->data << " ";
				curr = curr->next;
			}
			else {
				output << 0 << " ";
			}
		}
		output << "\n";
	}
	output.close();
}

Matrix* Matrix::add(Matrix* matrix2) {
	Matrix* newMatrix = new Matrix();
	newMatrix->set_num_rows(this->num_rows);
	newMatrix->set_num_cols(this->num_cols);
	Node* curr1 = this->head;
	Node* curr2 = matrix2->get_head();

	if (this->num_rows == matrix2->get_num_rows() && this->num_cols == matrix2->get_num_cols()) {
		while (curr1 != nullptr && curr2 != nullptr) {			
			if (curr1 != nullptr && curr1->pos_bigger(curr2)) {
				newMatrix->push_back(curr2->get_row(), curr2->get_col(), curr2->get_data());
				curr2 = curr2->get_next();
			}
			if (curr2 != nullptr && curr2->pos_bigger(curr1)) {
				newMatrix->push_back(curr1->get_row(), curr1->get_col(), curr1->get_data());
				curr1 = curr1->get_next();
			}
		}

		while (curr1 != nullptr) {
			newMatrix->push_back(curr1->get_row(), curr1->get_col(), curr1->get_data());
			curr1 = curr1->get_next();
		}

		while (curr2 != nullptr) {
			newMatrix->push_back(curr2->get_row(), curr2->get_col(), curr2->get_data());
			curr2 = curr2->get_next();
		}

		Node* currNew = newMatrix->get_head();
		while (currNew != nullptr && currNew->get_next() != nullptr) {
			if (currNew->get_row() == currNew->get_next()->get_row() && currNew->get_col() == currNew->get_next()->get_col()) {
				currNew->set_data( currNew->get_data() + currNew->get_next()->get_data() );
				Node* temp = currNew->get_next();
				currNew->set_next(currNew->get_next()->get_next());
				delete temp;
			}
			currNew = currNew->get_next();
		}
	}
	else {
		std::cout << "error: matrices are not the same size";
	}
	return newMatrix;
}
