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
	head = nullptr; // initialize both to nullptr
	tail = nullptr;
}

Matrix::~Matrix() {
	Node* current = head; // destructor for the linked list
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

void Matrix::write_to_file(std::string filename) {
	std::ofstream output(filename);
	Node* curr = this->head;
	// The nested for loops loop though every coordinate in the matrix
	for (int i = 0; i < this->num_rows; i++) {
		for (int j = 0; j < this->num_cols; j++) {
			// if statement makes sure the current linked list index is not null
			// also checks row and col of current index to see if it matches i and j
			if (curr != nullptr && i == curr->row && j == curr->col) {
				// if it matches, output the data value at location (i,j)
				output << curr->data << " ";
				curr = curr->next;
			}
			else {
				// otherwise, output 0 for (i,j) since it is sparse
				output << 0 << " ";
			}
		}
		output << "\n";
	}
	output.close();
}

Matrix* Matrix::add(Matrix* matrix2) {
	Matrix* newMatrix = new Matrix();
	// New matrix must be the same size as the ones being added
	newMatrix->set_num_rows(this->num_rows);
	newMatrix->set_num_cols(this->num_cols);
	Node* curr1 = this->head;
	Node* curr2 = matrix2->get_head();

	// I will try to explain this algorithm on paper since it is difficult to explain here
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

		// The algorithm below iterates through the new linked list to add duplicate values together.
		// For example, if two nodes have the same row/col values, their data is added together and the duplicate is deleted.
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
		std::cout << "error: matrices are not the same size\n";
	}
	return newMatrix;
}

Matrix* Matrix::subtract(Matrix* matrix2) {
	// Multiply all elements of matrix2 by -1 and add the two together
	Node* curr = matrix2->get_head();
	while (curr != nullptr) {
		curr->set_data(-1 * curr->get_data());
		curr = curr->get_next();
	}
	return this->add(matrix2);
}

Matrix* Matrix::multiply(Matrix* matrix2) {
	Matrix* newMatrix = new Matrix();
	// New matrix must have the number of rows of the first matrix and number of columns of the second matrix
	newMatrix->set_num_rows(this->num_rows);
	newMatrix->set_num_cols(matrix2->get_num_cols());
	Node* curr1 = this->head;
	Node* curr2 = matrix2->get_head();

	// If sizes match
	if (this->num_cols == matrix2->get_num_rows()) {
		// Iterates through the first matrix linked list
		while (curr1 != nullptr) {
			// Iterates through the second matrix linked list
			while (curr2 != nullptr) {
				// If the two nodes "match," ie the column of the first is the same as the row of the second, we multiply and add
				if (curr1->get_col() == curr2->get_row()) {
					// The product is added to the cell which corresponds to the row of the first and the column of the second
					int product = curr1->get_data() * curr2->get_data();
					place(curr1->get_row(), curr2->get_col(), product, newMatrix);
				}
				curr2 = curr2->get_next();
			}
			curr1 = curr1->get_next();
			curr2 = matrix2->get_head();
		}
	}
	else {
		std::cout << "error: matrices are incompatible for multiplication (cols of first must equal rows of second)\n";
	}
	return newMatrix;
}

void Matrix::place(int row, int col, int product, Matrix* newMatrix) {
	Node* curr = newMatrix->get_head();
	// Iterates through the new matrix
	while (curr != nullptr) {
		// If there exists a node corresponding to the given row and col, add the product to the data in that node and return
		if (curr->get_row() == row && curr->get_col() == col) {
			curr->set_data(curr->get_data() + product);
			return;
		}
		curr = curr->get_next();
	}
	// If there is not a node found, push back a new one with the row, col, and data specified
	newMatrix->push_back(row, col, product);
}

int Matrix::get_max_col() {
	int maxcol = 0;
	Node* curr = this->head;
	while (curr != nullptr) {
		if (curr->get_col() > maxcol) {
			maxcol = curr->get_col();
		}
		curr = curr->next;
	}
	return maxcol;
}

int Matrix::get_max_row() {
	int maxrow = 0;
	Node* curr = this->head;
	while (curr != nullptr) {
		if (curr->get_row() > maxrow) {
			maxrow = curr->get_row();
		}
		curr = curr->next;
	}
	return maxrow;
}

int Matrix::get_exacl_data(int inrow, int incol) {
	int maxrow = 0;
	int maxcol = 0;
	Node* curr = this->head;
	while (curr != nullptr) {
		if (curr->get_col() == incol && curr->get_row() == inrow) {
			return curr->get_data();
		}
		if (curr->get_row() > maxrow) {
			maxrow = curr->get_row();
		}
		if (curr->get_col() > maxcol) {
			maxrow = curr->get_col();
		}
		curr = curr->next;
	}
	if (maxrow > inrow && maxcol > incol) {
		return 0;
	}
	else {
		std::cout << "error, in " << inrow << "row and " << incol << "col, we can not find a number, please check! for now 0 will replace";
		return 0;
	}
}

void Matrix::Transform_to_vector(std::vector<std::vector<int>>* twod_vec) {
	int row = this->get_max_row(); // get maxrow for the matrix
	int col = this->get_max_col(); // get maxcol for the matrix
	std::vector<int> new_row;
	for (int i = 0; i < row; i++) {
		for (int k = 0; k < col; k++) {
			new_row.push_back(this->get_exacl_data(i, k));
		}
		twod_vec->push_back(new_row);
	}
}


//since the determinant of a matrix with integer values is a linear combination of integers, it must also be an integer
int Matrix::CalcDet(std::vector<std::vector<int>> matrix) {
	//this function is written in c++ to calculate the determinant of matrix
	// it's a recursive function that can handle matrix of any dimension
	int det = 0; // the determinant value will be stored here
	if (matrix.size() == 1) return matrix[0][0]; // no calculation needed
	else if (matrix.size() == 2) {
		//in this case we calculate the determinant of a 2-dimensional matrix in a
		//default procedure
		det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
		return det;
	}
	else {
		//in this case we calculate the determinant of a squared matrix that have
		// for example 3x3 order greater than 2
		for (int p = 0; p < matrix[0].size(); p++) {
			//this loop iterate on each elements of the first row in the matrix.
			//at each element we cancel the row and column it exist in
			//and form the Cofactor from the rest of the elements in the matrix
			std::vector<std::vector<int>> Tempmatrix; // to hold the Cofactor;
			for (int i = 1; i < matrix.size(); i++) {
				// iteration will start from row one cancelling the first row values
				std::vector<int> TempRow;

				// iteration will pass all cells of the i row excluding the j
				//value that match p column
				for (int j = 0; j < matrix[i].size(); j++) {
					if (j != p) TempRow.push_back(matrix[i][j]);//add current cell to TempRow
				}

				if (TempRow.size() > 0) Tempmatrix.push_back(TempRow);
				//after adding each row of the new matrix to the vector tempx
				//we add it to the vector temp which is the vector where the new
				//matrix will be formed
			}
			det = det + matrix[0][p] * pow(-1, p) * CalcDet(Tempmatrix);
			//then we calculate the value of determinant by using a recursive way
			//where we re-call the function by passing to it the Cofactor
			//we keep doing this until we get our determinant
		}
		return det;
	}
}