#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;




/*-------------------------------Data Structures-------------------------------*/

struct problem_data {
    int J; //Number of Jobs
    int M; //Number of Machines
    std::vector<vector<int>> OM; //Operations Matrix OM[row][column]
    std::vector<vector<int>> CM; //Cost Matrix CM[row][column]
};


/*----------------------------Function Declaration----------------------------*/
problem_data getData(const char* file);
void printMatrix(int rows, int cols, std::vector<vector<int>> M);
void greedy(data data)




/*------------------------------------Main------------------------------------*/

int main(int argc, char const *argv[])
{
	problem_data data = getData("instance.txt");
	cout << "Jobs: " << data.J << endl;
	cout << "Machines: " << data.M << endl;
  printMatrix(data.J, data.M, data.OM);
  printMatrix(data.J, data.M, data.CM);
	return 0;
}




/*---------------------------------Functions---------------------------------*/


problem_data getData(const char* file){
	ifstream inFile;
	problem_data data;

	//std::vector<int> PS(3); // Problem size: [N, D, S]
    inFile.open(file);
    if (!inFile) {
        cout << "No es posible abrir el archivo";
        exit(1); // terminate with error
    }
    char line[100];
    inFile.getline(line, 100);
    std::stringstream stream(line);
    //cout << line << endl;
    int J;
    int M;
    for (int i = 0; i < 2; ++i)
    {
	   int n;
	   stream >> n;
	   if(!stream)
	      break;
	   //cout << "Found integer: " << n << "\n";
	  if (i==0)
	  {
	  	J = n;
	  }
	  else{
	  	M = n;
	  }
    }
    //inFile.getline(line, 100);
    cout << J << endl;
    cout << M << endl;
    std::vector<vector<int>> OM; //Operation Matrix: OM[row][column]
    std::vector<vector<int>> CM; //Cost Matrix: CM[row][column]
    for (int i = 0; i < J; ++i)
    {
	    inFile.getline(line, 100);
	    //cout << line << endl;
	    std::stringstream stream2(line);
	    std::vector<int> OM_row;
	    std::vector<int> CM_row;
	    int j=0;
	    while (1)
	    {
		   int n;
		   stream2 >> n;
		   if(!stream2)
		      break;
		   //cout << "Found integer: " << n << "\n";
		  if (j%2==0)
		  {
		  	OM_row.push_back(n);
		  }
		  else{
		  	CM_row.push_back(n);
		  }
		   j++;
	    }
      OM.push_back(OM_row);
      CM.push_back(CM_row);
	  }
    inFile.close();
    cout << "OK " << "\n";
    data.J = J;
    data.M = M;
    cout << "OK2" << "\n";
    data.OM = OM;
    data.CM = CM;
    return data;
}

void printMatrix(int rows, int cols, std::vector<vector<int>> M){
    //cout << "printMatrixFunction----------------------------------------------------------------------------------" << '\n';
	cout<<"rows: "<< rows <<"\n";
	cout<<"cols: "<< cols <<"\n";
	for(int x=0;x<rows;x++)  // loop 3 times for three lines
    {
        for(int y=0;y<cols;y++)  // loop for the three elements on the line
        {
            cout<<M[x][y]<<" | " << flush;  // display the current element out of the array
        }
    	cout<<endl;  // when the inner loop is done, go to a new line
    }
}

void greedy(problem_data data){

}
