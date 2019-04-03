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
    std::vector<vector<int>> OM; //Operations Matrix
    std::vector<vector<int>> CM; //Cost Matrix
};


/*----------------------------Function Declaration----------------------------*/
problem_data getData(const char* file);




/*------------------------------------Main------------------------------------*/

int main(int argc, char const *argv[])
{
	problem_data data = getData("instance.txt");
	//cout << "Jobs: " << data.J << endl;
	//cout << "Machines: " << data.J << endl;
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
    for (int i = 0; i < 2; ++i)
    {
	   int n;
	   stream >> n;
	   if(!stream)
	      break;
	   cout << "Found integer: " << n << "\n";
	  if (i==0)
	  {
	  	data.J = n;
	  }
	  else{
	  	data.M = n;
	  }
    }
    inFile.getline(line, 100);
    //cout << line << endl;
    std::vector<vector<int>> OM(data.J); //Operation Matrix: OM[row][column]
    std::vector<vector<int>> CM(data.J); //Cost Matrix: CM[row][column]
    for (int i = 0; i < (data.J); ++i)
    {
	    inFile.getline(line, 100);
	    //cout << line << endl;
	    std::stringstream stream2(line);
	    OM[i].resize(data.M);
	    CM[i].resize(data.M);
	    int j=0;
	    while (1)
	    {
		   int n;
		   stream2 >> n;
		   if(!stream2)
		      break;
		   //cout << "Found integer: " << n << "\n";
		  if (i%2==0)
		  {
		  	OM[i][j] = n;
		  }
		  else{
		  	CM[i][j] = n;
		  }
		   j++;
	    }
	}
    data.OM = OM;
    data.CM = CM;
	return data;
}