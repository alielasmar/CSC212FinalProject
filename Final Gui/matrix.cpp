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

Matrix::Matrix(int num_rows, int num_cols) {
    head = nullptr;
    tail = nullptr;
    this->num_rows = num_rows;
    this->num_cols = num_cols;
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
    filename =newFileM;
    std::ofstream output(filename);
    Node* curr = this->head;
    // The nested for loops loop though every coordinate in the matrix
    for (int i = 0; i < this->num_rows; i++) {
        for (int j = 0; j < this->num_cols; j++) {
            // if statement makes sure the current linked list index is not null
            // also checks row and col of current index to see if it matches i and j
            if (curr != nullptr && i == curr->row && j == curr->col) {
                // if it matches, output the data value at location (i,j)
                output << curr->data << "   ";
                curr = curr->next;
            }
            else {
                // otherwise, output 0 for (i,j) since it is sparse
                output << 0 << "    ";
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
                currNew->set_data(currNew->get_data() + currNew->get_next()->get_data());
                Node* temp = currNew->get_next();
                currNew->set_next(currNew->get_next()->get_next());
                delete temp;
            }
            currNew = currNew->get_next();
        }
    }
    else {

        std::ofstream output(newFileM);
        output << "error: matrices are not the same size\n";
        output.close();
    }
    return newMatrix;
}

Matrix* Matrix::subtract(Matrix* matrix2) {
    // Multiply all elements of matrix2 by -1 and add the two together
    Matrix* matrixSub = new Matrix(matrix2->get_num_rows(), matrix2->get_num_cols());
    Node* curr = matrix2->get_head();
    while (curr != nullptr) {
        matrixSub->push_back(curr->get_row(), curr->get_col(), curr->get_data() * -1);
        curr = curr->get_next();
    }
    Matrix* newMatrix = this->add(matrixSub);
    delete matrixSub;
    return newMatrix;
}

Matrix* Matrix::multiply(Matrix* matrix2) {
    Matrix* newMatrix = new Matrix();
    // New matrix must have the number of rows of the first matrix and number of columns of the second matrix
    newMatrix->set_num_rows(this->num_rows);
    newMatrix->set_num_cols(matrix2->get_num_cols());
    Node* curr1 = this->head;
    Node* curr2 = matrix2->get_head();

    newMatrix->initialize();

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
        std::ofstream output(newFileM);
        output << "error: matrices are incompatible for multiplication (cols of first must equal rows of second)\n";
        output.close();
    }
    return newMatrix;
}

void Matrix::place(int row, int col, int product, Matrix* newMatrix) {
    Node* curr = newMatrix->get_head();
    // Iterates through the new matrix
    while (curr != nullptr) {
        // If there exists a node corresponding to the given row and col,
        // add the product to the data in that node and return
        if (curr->get_row() == row && curr->get_col() == col) {
            curr->set_data(curr->get_data() + product);
            return;
        }
        curr = curr->get_next();
    }
}

void Matrix::initialize() {
    for (int i = 0; i < this->num_rows; i++) {
        for (int j = 0; j < this->num_cols; j++) {
            // fills an empty linked list with 0
            this->push_back(i, j, 0);
        }
    }
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
        if(maxrow > inrow && maxcol > incol){
            return 0;
        }
        curr = curr->next;
    }
    if (this->num_rows >= inrow && this->num_cols >= incol) {
        return 0;
    }
    else {
        std::ofstream output(newFileM);
        output<< "error, in " << inrow << "row and " << incol << "col, we can not find a number, please check! for now 0 will replace\n";
        return 0;
    }
}

std::vector<std::vector<int> > Matrix::Transform_to_vector() {
    std::vector<std::vector<int> > twod_vec;
    std::vector<int> new_row;
    for (int i = 0; i < this->num_rows; i++) {
        for (int k = 0; k < this->num_cols; k++) {
            new_row.push_back(this->get_exacl_data(i, k));
        }
        twod_vec.push_back(new_row);
        new_row.clear();
    }
    return twod_vec;
}

/**Calculates determnant of a matrix of any size
 * @Param - 2d matrix of integer values
 * @return - integer value of determinate (the determinant of a matrix with integer values is a linear combination of integers, it must also be an integer)
*/
int Matrix::CalcDet(std::vector<std::vector<int> > matrix) {

    int det = 0; //the determinant value
    if (matrix.size() == 1) return matrix[0][0]; //1X1 determinate doesn't need calculated
    else if (matrix.size() == 2) {
        det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]); //base case - determinant of a 2d matrix
        return det;
    }
    else { //calculate the determinant of squared matrix with order greater than 2
     //iterate accross first row of cofactors (left to right)
        for (int p = 0; p < matrix[0].size(); p++) {
            std::vector<std::vector<int> > tempMatrix; // to hold the new matrix (after columns and rows are removed with respect to cofactor);
            // iterate through rows not including cofacters (from top to bottom)
            for (int q = 1; q < matrix.size(); q++) {
                std::vector<int> tempRow; //creates row for tempMatrix
                // iterate across rows exluding the current column of cofactor (p) (left to right)
                for (int i = 0; i < matrix[q].size(); i++)
                    if (i != p) tempRow.push_back(matrix[q][i]);//add current int to row of new matrix
                if (tempRow.size() > 0) tempMatrix.push_back(tempRow); //adds row to new matrix after all values are stored from row
            }
            det = det + matrix[0][p] * pow(-1, p) * CalcDet(tempMatrix);//recursive call using recursive formula passing new matrix
        }
        return det;
    }
}

std::vector<std::vector<double>> Matrix::Inverse_matrix() {
    std::vector<std::vector<double>> inv_vec;
    std::vector<std::vector<int>> tra_vec;
    tra_vec = this->Transform_to_vector();
    std::vector<std::vector<double>> tpos_vec;
    std::vector<std::vector<int>> det_vec;
    std::vector<int> oned_det_vec;
    std::vector<double> new_row;
    double det = CalcDet(tra_vec);
    double result;
    if (this->num_cols != this->num_rows || det == 0) {
        std::ofstream output( newFileM);
        output << " This is not a square matrix or det of your matrix is 0, the inverse of it always 0 vector matrix";
        output.close();
        inv_vec.push_back(new_row);
        return inv_vec;
    }
    else {
        for (int i = 0; i < this->num_rows; i++) {
            for (int k = 0; k < this->num_cols; k++) {
                for (int z = 0; z < this->num_rows; z++) {
                    for (int y = 0; y < this->num_cols; y++) {
                        if (z != i && y != k) {
                            oned_det_vec.push_back(get_exacl_data(z, y));
                        }
                    }
                    if (oned_det_vec.size() != 0) {
                        det_vec.push_back(oned_det_vec);
                    }
                    oned_det_vec.clear();
                }
                result = CalcDet(det_vec);
                result = result / det;
                if ((i + k) % 2 != 0 && result != 0) {
                    result = result * -1;
                }
                det_vec.clear();
                new_row.push_back(result);
            }
            inv_vec.push_back(new_row);
            new_row.clear();
        }
        for (int i = 0; i < this->num_rows; i++) {
            for (int k = 0; k < this->num_cols; k++) {
                new_row.push_back(inv_vec[k][i]);
            }
            tpos_vec.push_back(new_row);
            new_row.clear();
        }
    }
    return tpos_vec;
}



//Do not delete this !!!!!!!!


//Matrix* Matrix::Multiply(Matrix* matrix2){
    //find the max row and col for both matrix, compare both of them, make sure first matrix col = second matrix row, if not error.
    //First matrix row will be the new matrix row, second matrix col will be the new col;
    //set a two for loop, i go through form 0 to maxrow for the first matrix and inside loop, k form 0 to maxcol of the second matrix.
    //one more loop inside use x form 0 to first matrix col or second matrix row, use excal number form first matrix(i,x) times second matrix (i,k), at the end add all of them together
    //and then save it on new matrix position i and k.
//}
