#include <iostream>
#include <memory>
#include <vector>
#include <thread>

using namespace std;

class Matrix{
private:
    vector<vector<double>> matrix;
    int rows;
    int cols;

public:
    Matrix(int rows, int cols){
        this->rows = rows;
        this->cols = cols;
        this->matrix = vector<vector<double>>(rows,vector<double>(cols,0));
    }

    void readData(){
        for(int i=0; i<rows; i++)
            for(int j=0; j<cols; j++)
                cin >> matrix[i][j];
    }

    int getData(int i, int j){
        return matrix[i][j];
    }

    void setData(int i, int j, int value){
        matrix[i][j] = value;
    }

    void writeMatrix(){
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++)
                cout << matrix[i][j] << " ";
            cout << endl;
        }
    }
};


//function for counting single cell of new matrix
void countSum(int rows, int i, int j, unique_ptr<Matrix> &firstMatrix, unique_ptr<Matrix> &secondMatrix, unique_ptr<Matrix> &multipliedMatrix){
    int sum = 0;

    for(int k = 0; k < rows; k++ )
       sum += firstMatrix->getData(i,k) * secondMatrix->getData(k,j);

    multipliedMatrix->setData(i,j,sum);
}

int main()
{
    int cols1, rows1, cols2, rows2;

    cout << "Enter the dimensions of the first matrix (number of rows and cols)" << endl;
    cin >> rows1 >> cols1;

    cout << "Enter the dimensions of the second matrix (number of rows and cols)" << endl;
    cin >> rows2 >> cols2;

    if(rows1 <= 0 || cols1 <= 0 || rows2 <= 0 || cols2 <= 0){
        cout << "Values of rows and cols can not be lower or equal to 0." << endl;
        return 0;
    }

    //this is basic rule for matrix multiplication
    if(cols1 != rows2){
        cout << "To multiply matrices, the number of columns in the first must be equal to the number of rows in the second." << endl;
        return 0;
    }

    unique_ptr<Matrix> firstMatrix(new Matrix(rows1, cols1));
    unique_ptr<Matrix> secondMatrix(new Matrix(rows2, cols2));
    unique_ptr<Matrix> multipliedMatrix(new Matrix(rows1, cols2));

    //number of used threads
    int size = 4;
    thread threads[size];

    cout << "Enter first matrix data:" << endl;
    firstMatrix->readData();
    cout << "Enter second matrix data:" << endl;
    secondMatrix->readData();

    //counter to ensure using fixed number of threads
    int c = 0;

    for(int i=0; i<rows1; i++){
       for(int j=0; j<cols2; j++){
         threads[c] = thread(countSum,rows2,i,j,ref(firstMatrix),ref(secondMatrix), ref(multipliedMatrix));
         c++;

         if(c==size){
             for(int k=0; k<size; k++)
                 threads[k].join();
             c=0;
         }
       }
     }

    cout << "Multiplied matrix:" << endl;
    multipliedMatrix->writeMatrix();

    return 0;
}
