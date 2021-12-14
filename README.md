# CSC212FinalProject
Instructions to use command line arguments:
1. Operation
2. Output file
3. Matrix 1 file
4. Matrix 2 file

Possible operations: add, subtract, multiply, determinant, inverse

Example:

g++ main.cpp matrix.cpp
./a.out add output.txt add1.txt add2.txt

./a.out inverse output.txt inverse.txt


Some sample files are included in the repository



Instructions to use GUI:
Computer Requirements : Unfortunantly our GUI only supports Mac Os 12.00+ 

In your terminal you must type in 'chmod 755 -R 755' then drag the application into the terminal and place it after what you have just typed

Upon opening the application you must select an operator and 2 files using their designated file, not doing so will cause errors and you will be prompted to do so

When using the file buttons you will be brought to your file manager where you will be prompted to select a file

You will only be able to select .txt files, the file manager will not allow the selection of anoy other types of files

When using the 'inverse' or 'determinant' opertors 

The "+" operator will add the matrices together and print the result on the screen via the text box

The "-" operator will subtract the matrices and print the result on the screen via the text box

The "x" operator will multiply the matrices and print the result on the screen via the text box

The "Inverse" operator will provide the inverse of the matrix entered and print the result on the screen via the text box

The "Determinant" operator will provide the determinant of the matrix entered and print the result on the screen via the text box

note: when using the determinant or inverse operators only Matrix1 will be used, Matrix2 can be left empty or you may choose to have it filled, it will not affect the result

Pressing the Get New Matrix button will execute the operation

The text box is editable but it is recommended that you do not save any sensitive information here, as soon as the 'Get New Matrix' button is pressed, everything will be erased
