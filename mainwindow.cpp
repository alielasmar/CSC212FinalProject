#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "matrix.h"
#include "matrix.cpp"
#include <iostream>
#include <QFile>
#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <QMessageBox>
#include <vector>

std::string filename1; //stores matrix 1 filename
std::string filename2; //stores matrix 2 filename
std::string opp;       // tracks operation selected
std::string newFileM;  // stores file for storage filename
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void read_file(Matrix* matrix, std::string filename) {  //reads file passed to it for data and inputs into newFileM
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




void MainWindow::on_pushButton_clicked() //brings file system to select .txt file
{
    QString filter = "Text File (*.txt)";
    QString file1 = QFileDialog::getOpenFileName(this,"Open a file", QDir::homePath(),filter);
    QMessageBox::information(this,"....", file1);
    filename1 = file1.toStdString();
}


void MainWindow::on_pushButton_2_clicked() //brings file system to select .txt file
{
    QString filter = "Text File (*.txt)";
    QString file2 = QFileDialog::getOpenFileName(this,"Open a file", QDir::homePath(),filter);
    QMessageBox::information(this,"....", file2);
    filename2 = file2.toStdString();
}


void MainWindow::on_pushButton_3_clicked() //stores opperation chosen
{
    QString tmp = "+";
    opp = tmp.toStdString();
}


void MainWindow::on_pushButton_4_clicked() //stores opperation chosen
{
    QString tmp = "x";
    opp = tmp.toStdString();
}

void MainWindow::on_pushButton_7_clicked(){ //shows result for inputted values
    bool error = false;
    std::ofstream output(newFileM);


    if (opp == "+" || opp =="x"|| opp == "-"){ // if opp is one of the above check if file2, only det and inv can have file2 be empty
                if(filename2.empty()){
                output<< "Please choose your file 2\n";
                error = true;}
            }

    if(filename1.empty()){ // if file1 empty then its an error
                output<< "Please choose your file 1\n";
                error = true;
            }

    if (opp != "+"&&opp!="-"&&opp!="I"&&opp!="x"&&opp!="D"){ //if opp isn't selected then it's an error
                output << "Please choose an operator before htting the 'Get Matrix' button\n";
                error = true;
            }


            output.close();
            Matrix* matrix1 = new Matrix();
            Matrix* matrix2 = new Matrix();
            std::string filename = newFileM;
            read_file(matrix1, filename1);
            read_file(matrix2, filename2);

//operations will only run if no known error is detected all opperation occur in matrix.cpp, this function only serves to display the collected data

            if(opp == "+" && error == false){
            Matrix* new_matrix = matrix1->add(matrix2);
            new_matrix->write_to_file("matrix_new.txt");
            delete new_matrix;
}

            if(opp == "x"&& error == false){
            Matrix* product = matrix1->multiply(matrix2);
            product->write_to_file("matrix_new.txt");
            delete product;
            }

            if(opp == "-"&& error == false){
                Matrix* sub = matrix1->subtract(matrix2);
                sub->write_to_file("sub.txt");
                delete sub;
            }

            if (opp == "I"&& error == false){
                std::ofstream fileOut;
                fileOut.open(newFileM);
                if (!filename1.empty()){
                std::vector<std::vector<double> > inv_matrix= matrix1->Inverse_matrix();
                for (int i = 0; i < inv_matrix.size();i++){
                    for(int j = 0; j < inv_matrix[0].size();j++){
                        fileOut<<inv_matrix[i][j]<<"    ";
                    }
                    fileOut<<"\n";
                }
                }
            }

            if(opp == "D" && error == false){
                std::ofstream fileOut;
                fileOut.open(newFileM);
                std::vector<std::vector<double>> new_vec = matrix1->Inverse_matrix();
                        for (int i = 0; i < new_vec.size(); i++) {
                            for (int j = 0; j < new_vec[0].size(); j++) {
                                fileOut << new_vec[i][j] << "   ";
                            }
                            fileOut << "\n";
                        }
                        fileOut.close();
            }


        QString qNewFileM = QString::fromStdString(newFileM);
        QFile file(qNewFileM);
//error message if storage file is not open
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "title", "file not open");
        }

        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setPlainText(text);
        file.close();
        delete matrix1;
        delete matrix2;
}

//stores opperation as -
void MainWindow::on_pushButton_5_clicked() //stores opperation chosen
{
    opp = "-";
}

//stores opperation as I
void MainWindow::on_pushButton_6_clicked() //stores opperation chosen
{
    opp = "I";
}

//stores opperation as D
void MainWindow::on_pushButton_8_clicked() //stores opperation chosen
{
    opp = "D";
}

//opens file system for storage file, only .txt files can be selected
void MainWindow::on_pushButton_9_clicked()
{
    QString filter = "Text File (*.txt)";
    QString newFile = QFileDialog::getOpenFileName(this,"Open a file", QDir::homePath(),filter);
    QMessageBox::information(this,"....", newFile);
    newFileM = newFile.toStdString();
}

