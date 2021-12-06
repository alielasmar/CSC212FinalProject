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

int Matrix::get_max_col(){
	int maxcol = 0;
	Node* curr = this->head;
	while(curr != nullptr){
		if(curr->get_col()>maxcol){
			maxcol = curr->get_col();
		}
		curr = curr->next;
	}
	return maxcol;
}

int Matrix::get_max_row(){
	int maxrow = 0;
	Node* curr = this->head;
	while(curr != nullptr){
		if(curr->get_row()>maxrow){
			maxrow = curr->get_row();
		}
		curr = curr->next;
	}
	return maxrow;
}

int Matrix::get_exacl_data(int inrow, int incol){
	int maxrow = 0;
	int maxcol = 0;
	Node* curr = this->head;
	while(curr != nullptr){
		if(curr-> get_col() == incol && curr-> get_row()== inrow){
			//std::cout<<curr->get_data()<<std::endl;
			return curr->get_data();
		}
		if(curr->get_row() >= maxrow){
			maxrow = curr->get_row();
		}
		if(curr->get_col() >= maxcol){
			maxcol = curr->get_col();
		}
		curr = curr->next;
	}
	if(maxrow >= inrow && maxcol >= incol){
		return 0;
	}else{
		std::cout<<"error, in " << inrow << "row and " << incol<<std::endl;
		return 0;
	}
}

void Matrix::Transform_to_vector(std::vector<std::vector<double>> * twod_vec){
	int row = this->get_max_row(); // get maxrow for the matrix
	int col = this->get_max_col(); // get maxcol for the matrix
	std::vector<double> new_row;
	for(int i = 0; i <= row; i++){
		for(int k =0; k<= col; k++){
			new_row.push_back(this->get_exacl_data(i,k));
		}
		twod_vec->push_back(new_row);
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
//Matrix* Matrix::Multiply(Matrix* matrix2){
	//find the max row and col for both matrix, compare both of them, make sure first matrix col = second matrix row, if not error.
	//First matrix row will be the new matrix row, second matrix col will be the new col;
	//set a two for loop, i go through form 0 to maxrow for the first matrix and inside loop, k form 0 to maxcol of the second matrix.
	//one more loop inside use x form 0 to first matrix col or second matrix row, use excal number form first matrix(i,x) times second matrix (i,k), at the end add all of them together
	//and then save it on new matrix position i and k.
//}
