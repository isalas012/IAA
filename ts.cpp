#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;




/*-------------------------------Data Structures-------------------------------*/

struct problem_data {
    int J; //Number of Jobs
    int M; //Number of Machines
    std::vector<vector<int>> OM; //Operations Matrix OM[row][column]
    std::vector<vector<int>> CM; //Cost Matrix CM[row][column]
};

struct result {
  int makeSpan;
  std::vector<vector<int>> RM; //Result Matrix OM[row][column]
};

struct node_info{
  int weight;
  int EST; //Erliest Start Time
  int LCT; //Latest Completition Time
};

struct node{
  int machine;
  int job;
  int operation;
  std::vector<int> from;
  std::vector<int> to;
  node_info info;
};

struct tabuList{
  std::vector<std::vector<int>> list;
  int max_len;
};


/*----------------------------Function Declaration----------------------------*/
problem_data getData(const char* file);
int job_compare(std::vector<int> v,  std::vector<vector<int>> time);
int job_compare_SPT(std::vector<int> v, std::vector<vector<int>> time);
int job_compare_LPT(std::vector<int> v, std::vector<vector<int>> time);
int job_compare_LRPT(std::vector<int> v, std::vector<vector<int>> time);
int job_compare_SRPT(std::vector<int> v, std::vector<vector<int>> time);
result greedy(problem_data data, const char* problem_name);
void printMatrix(std::vector<vector<int>> &M);
void printVector(vector<int> &M);
int elementNotInMatrix(std::vector<int> &M, int element);
void writeToFile(const char* problem_name, int time, std::vector<vector<int>> result);
void tabuSearch(problem_data data, const char* problem_name);
result CPM(problem_data data, result r, tabuList &tl);
result test_result();
problem_data test_data();
std::vector<std::vector<int>> makeSwap(std::vector<std::vector<int>> r, std::vector<int> swap);
int notTabu(std::vector<int> swap, tabuList &tl);



/*-----------------------------------Classes----------------------------------*/

class Graph{
public:
  std::vector<node> nodes;
  std::vector<int> critialPath;
  Graph(problem_data data, std::vector<vector<int>> RM){
    std::vector<node> nodes_temp;
    for (int i = 0; i < data.M; ++i)
    {
      for (int j = 0; j < data.J; ++j)
      {
        int actual_job = RM[i][j];
        for (int k = 0; k < data.M; ++k)
        {
          if (data.OM[actual_job][k] == i )
          {
            node actual;
            actual.machine = i;
            actual.job = actual_job;
            actual.operation = k;
            actual.info.weight = data.CM[actual_job][k];
            actual.info.EST = -1;
            actual.info.LCT = -1;
            nodes_temp.push_back(actual);
          }
        }
      } 
    }
    node start;
    start.machine = -1;
    start.job = -1;
    start.operation = -1;
    start.info.weight = 0;
    start.info.EST = 0;
    start.info.LCT = -1;
    nodes.push_back(start);
    for (int i = 0; i < nodes_temp.size(); ++i)
    {
      nodes.push_back(nodes_temp[i]);
    }
    node end;
    end.machine = -2;
    end.job = -2;
    end.operation = -2;
    end.info.weight = 0;
    end.info.EST = -1;
    end.info.LCT = -1;
    nodes.push_back(end);
    //printNodes();
    for (int i = 0; i < data.M; ++i)
    {
      nodes[0].to.push_back(i*data.J +1);
      nodes[nodes.size()-1].from.push_back(i*data.J + data.J);
    }
    //printNodes();
    for (int i = 0; i < nodes[0].to.size(); ++i)
    {
      for (int j = 0; j <data.J; ++j)
      {
        if (j==0)
        {
          nodes[nodes[0].to[i]+j].from.push_back(0);
        }
        else{
          nodes[nodes[0].to[i]+j].from.push_back(nodes[0].to[i]+(j-1));
        }
        if (j == data.J-1)
        {
         nodes[nodes[0].to[i]+j].to.push_back(nodes.size()-1);
        }
        else{
          nodes[nodes[0].to[i]+j].to.push_back(nodes[0].to[i]+(j+1));
        }
      }
    }
    //printNodes();
    for (int i = 0; i < data.J; ++i)
    {
      std::vector<int> jp;
      jp.assign(data.M, -1);
      for (int j = 0; j < nodes.size(); ++j)
      {
        if (nodes[j].job == i)
        {
          jp[nodes[j].operation] = j;
        }
      }
      for (int j = 0; j < jp.size(); ++j)
      {
        if (j!=0)
        {
          nodes[jp[j]].from.push_back(jp[j-1]);
        }
        if (j != jp.size()-1)
        {
          nodes[jp[j]].to.push_back(jp[j+1]);
        }
      }
    }
    //printNodes();
    EST2();  
    nodes[nodes.size()-1].info.LCT = nodes[nodes.size()-1].info.EST;
    //nodes[nodes.size()-1].info.LCT  = EST(0);
    //printNodes();
    LCT(0);
    //printNodes();
    critialPath = getCritialPath();
    //cout << "makeSpan: " << getMakeSpan() << endl;

  };
  int EST(int id){
    if (nodes[id].info.EST != -1)
    {
      return nodes[id].info.EST;
    }
    else{
      std::vector<int> v;
      for (int i = 0; i < nodes[id].from.size(); ++i)
      {
        v.push_back(EST(nodes[id].from[i]) + nodes[nodes[id].from[i]].info.weight);
      }
      int max = -1000;
      for (int j = 0; j < v.size(); ++j)
      {
        if (v[j]> max)
        {
          max = v[j];
        }
      } 
      nodes[id].info.EST = max;

      return max;
    }
  };
  void EST2(){
    std::vector<int> v = {0};
    int count = 0;
    int last = nodes.size()-1;
    //cout << nodes[v[0]].info.EST << endl;
    while(count < nodes.size()-1){
      //cout << count << endl;
      //printVector(v);
      if (nodes[v[0]].info.EST != -1)
      {
        for (int i = 0; i < nodes[v[0]].to.size(); ++i)
        {
          if (nodes[nodes[v[0]].to[i]].info.EST == -1)
          {
            v.push_back(nodes[v[0]].to[i]);
          }
        }
        v.erase (v.begin());
      }
      else{
        int ready = 1;
        std::vector<int> prev_est;
       for (int i = 0; i < nodes[v[0]].from.size(); ++i)
        {
          node prev = nodes[nodes[v[0]].from[i]];
          if (prev.info.EST == -1)
          {
            v.push_back(nodes[v[0]].from[i]);
            v.push_back(v[0]);
            v.erase (v.begin());
            ready = 0;
            break;
          }
          else{
            prev_est.push_back(prev.info.EST + prev.info.weight);
          }
        }
        if (ready)
        {
          int max = -1000;
          for (int j = 0; j < prev_est.size(); ++j)
          {
            if (prev_est[j]> max)
            {
              max = prev_est[j];
            }
          }
          nodes[v[0]].info.EST = max;
          count++; 
        }
      }
    }
  }
  int LCT(int id){
    if (nodes[id].info.LCT != -1)
    {
      return nodes[id].info.LCT;
    }
    else{
      std::vector<int> v;
      for (int i = 0; i < nodes[id].to.size(); ++i)
      {
        v.push_back(LCT(nodes[id].to[i]));
      }
      int min = 1000000000;
      for (int i = 0; i < v.size(); ++i)
      {
        if (v[i] < min)
        {
          min = v[i];
        }
      } 
      nodes[id].info.LCT = min-nodes[id].info.weight;
      return min-nodes[id].info.weight;
    }
  };
  std::vector<int> getCritialPath(){
    //cout<< "Critical Path: " << endl; 
    std::vector<int> v;
    int id = 0;
    v.push_back(id);
    int len = nodes.size() - 1; 
    while(id != len){ 
      int end = 0;
      for (int i = 0; i < nodes[id].to.size(); ++i)
      {
        if(nodes[nodes[id].to[i]].info.EST == nodes[nodes[id].to[i]].info.LCT && (nodes[nodes[id].to[i]].info.LCT - nodes[id].info.LCT) == nodes[id].info.weight){
          if (nodes[id].to[i] ==  len)
          {
            end = 1;
          }
          else{
            v.push_back(nodes[id].to[i]);
            id = nodes[id].to[i];
            end = 0;
            break;
          }
        }
      }
      if (end)
      {
        id = len;
        v.push_back(len);
      }
    }
    /*
    for (int i = 0; i < v.size(); ++i)
    {
      cout<< v[i] <<" | ";
    }
    cout<< endl;*/
    return v;
  };
  int getMakeSpan(){
    int total = 0;
    for (int i = 0; i < critialPath.size(); ++i)
    {
      total = total + nodes[critialPath[i]].info.weight;
    }
    return total;
  };
  void printNodes(){
    cout<< "Print Nodes: " << endl; 
    for (int i = 0; i < nodes.size(); ++i)
    {
      cout<< i <<" ||| ";
      cout<<nodes[i].info.weight<<" | "; 
      cout<<nodes[i].machine<<" | "; 
      cout<<nodes[i].job <<" | "; 
      cout<<nodes[i].operation <<" || "; 
      cout<<nodes[i].info.EST<<" || "; 
      cout<<nodes[i].info.LCT<<" || "; 
      if (nodes[i].from.size()>0)
      {
        for (int j = 0; j < nodes[i].from.size(); ++j)
        {
          cout<<nodes[i].from[j] <<" / " ;
        }
      }
      cout <<" || "; 
      if (nodes[i].to.size()>0)
      {
        for (int j = 0; j < nodes[i].to.size(); ++j)
        {
          cout<<nodes[i].to[j] <<" / "; 
        }
      }
      cout <<" || " << endl; 
    }
  };
  std::vector<std::vector<int>> swapCandidates(){
    int cand;
    std::vector<std::vector<int>> v;
    for (int i = 1; i < critialPath.size()-1; ++i)
    {
      cand = critialPath[i];
      if (nodes[critialPath[i]].machine == nodes[critialPath[i+1]].machine)
      {
        v.push_back({nodes[critialPath[i]].machine, nodes[critialPath[i]].job, nodes[critialPath[i+1]].job}); //machine, node1, node2
      }
    }
    return v;
  };
};



/*------------------------------------Main------------------------------------*/

int main(int argc, char const *argv[])
{
  ofstream file;
  file.open("results.txt");
  file.close();

  if (argc>1)
  {
    string file_name = argv[1];
    cout << "Problem name:  " << file_name << endl;
    char *file_name_c = &file_name[0u];
    problem_data data = getData(file_name_c);
    cout << "Jobs: " << data.J << endl;
    cout << "Machines: " << data.M << endl;
    cout << "Operation Matrix: " << endl;
    printMatrix(data.OM);
    cout << "Cost Matrix: " << endl;
    printMatrix(data.CM);
    //printMatrix(data.J, data.M, data.OM);
    //printMatrix(data.J, data.M, data.CM);
    char *instance_c = &file_name[0u];
    tabuSearch(data, instance_c);
  }
  else{
    cout << "Se debe especificar la instancia. " << endl;
  }
  /*
    std::vector<string> instance = {"orb01", "orb02", "orb03", "orb04", "abz5", "abz6", "abz7", "abz8", "swv01", "swv02", "swv03", "swv11", "swv12", "swv13"};
    for (size_t i = 0; i < instance.size(); i++) {
      string file_name = "instancias/" + instance[i] + ".txt";
      cout << "nombre:  " << file_name << endl;
      char *file_name_c = &file_name[0u];
      problem_data data = getData(file_name_c);
    	cout << "Jobs: " << data.J << endl;
    	cout << "Machines: " << data.M << endl;
      printMatrix(data.OM);
      printMatrix(data.CM);
      //printMatrix(data.J, data.M, data.OM);
      //printMatrix(data.J, data.M, data.CM);
      char *instance_c = &instance[i][0u];
      greedy(data, instance_c);
    }
  */
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
    //cout << J << endl;
    //cout << M << endl;
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
    //cout << "OK " << "\n";
    data.J = J;
    data.M = M;
    //cout << "OK2" << "\n";
    data.OM = OM;
    data.CM = CM;
    return data;
}


void printMatrix(std::vector<vector<int>> &M){
  for(std::vector<vector<int>>::size_type x=0;x<M.size();x++)  // loop 3 times for three lines
    {
      for(std::vector<int>::size_type y=0;y<M[x].size();y++)  // loop for the three elements on the line
      {
        cout<<M[x][y]<<" | " << flush;  // display the current element out of the array
      }
      cout<<endl;  // when the inner loop is done, go to a new line
    }
};


void printVector(std::vector<int> &M){
    for(std::vector<int>::size_type y=0;y<M.size();y++)  // loop for the three elements on the line
    {
      cout<<M[y]<<" | " << endl;  // display the current element out of the array
    }
}

int elementNotInMatrix(std::vector<int> &M, int element){
      for(std::vector<int>::size_type y=0;y<M.size();y++)  // loop for the three elements on the line
      {
        if (M[y] == element) {
          return 0;
        }
      }
    return 1;
}


result greedy(problem_data data, const char* problem_name){
  std::vector<vector<int>> r_jobs = data.OM; // remaining jobs
  std::vector<vector<int>> r_times = data.CM; // remaining times
  std::vector<int> machines_o; //machines operation
  std::vector<int> t_machines; //machines remaining time

  std::vector<vector<int>> finalOrder(data.M); //machines queues

  int total_time = 0;
  for (int i = 0; i < data.M; i++) {
    t_machines.push_back(0);
    machines_o.push_back(-1);
  }
  while (1) {
    std::vector<vector<int>> q_machines(data.M); //machines queues
    for (int i = 0; i < data.M; i++) {
      q_machines[i].push_back(-1);
    }
    for (int i = 0; i < data.J; i++) {
      if (r_jobs[i].size() >= 1) {
        if (elementNotInMatrix(machines_o, i)) {
          int m = r_jobs[i][0];
          if (q_machines[m][0]==-1) {
            q_machines[m][0] = i;
          }
          else{
            q_machines[m].push_back(i);
          }
        }
      }

    }
    //cout << "Jobs remaining" << endl;
    //printMatrix(r_jobs);
    //cout << "Machines queues" << endl;
    //printMatrix(q_machines);
    for (int i = 0; i < data.M; i++) {
      if (machines_o[i] == -1) {
        if (q_machines[i].size() > 1) {
          int selected_job = job_compare_SPT(q_machines[i], r_times);
          //int selected_job = job_compare_LPT(q_machines[i], r_times);
          //int selected_job = job_compare_SRPT(q_machines[i], r_times);
          //int selected_job = job_compare_LRPT(q_machines[i], r_times);
          //q_machines[i].erase(q_machines[i].begin(), q_machines[i].begin() + q_machines[i].size());
          machines_o[i] = selected_job;
          t_machines[i] = r_times[selected_job][0];
          //r_jobs[selected_job].erase(r_jobs[selected_job].begin());
          //r_times[selected_job].erase(r_times[selected_job].begin());
        }
        else{
          if (q_machines[i][0] != -1) {
            machines_o[i] = q_machines[i][0];
            t_machines[i] = r_times[q_machines[i][0]][0];
          }
        }
      }
    }
    int end = true;
    for (int i = 0; i < data.M; i++) {
      if (machines_o[i] !=-1) {
        end = false;
      }
    }
    if (end) {
      cout << "Final Order Greedy Algorithm" << endl;
      printMatrix(finalOrder);
      cout << "Total time: " << total_time << endl;
      //writeToFile(problem_name, total_time, finalOrder);
      result r;
      r.RM = finalOrder;
      r.makeSpan = total_time;
      return r;
    }
    ////cout << "Machines queues" << endl;
    ////printMatrix(q_machines);
    //cout << "Machines time" << endl;
    //printVector(t_machines);
    int lt = 1000000000; //lowest time
    for (int i = 0; i < data.M; i++) {
      if (t_machines[i] != 0 && t_machines[i] < lt) {
        lt = t_machines[i];
      }
    }
    total_time = total_time + lt;
    for (int i = 0; i < data.M; i++) {
      if (t_machines[i] != 0 ) {
        t_machines[i] = t_machines[i] - lt;
      }
    }
    //cout << "Machines time" << endl;
    //printVector(t_machines);
    for (int i = 0; i < data.M; i++) {
      if (t_machines[i] == 0) {
        if (machines_o[i] !=-1) {
          r_jobs[machines_o[i]].erase(r_jobs[machines_o[i]].begin());
          r_times[machines_o[i]].erase(r_times[machines_o[i]].begin());
          finalOrder[i].push_back(machines_o[i]);
          machines_o[i] = -1;
        }
      }
    }

    //cout << "FinaleOrder" << endl;
    //printMatrix(finalOrder);
  }
}

int job_compare(std::vector<int> v,  std::vector<vector<int>> time){
  int num = v[0];
  return num;
}


int job_compare_SPT(std::vector<int> v, std::vector<vector<int>> time){
  std::vector<int> job_time;
  for (std::vector<int>::size_type i = 0; i < v.size(); i++) {
    int t = time[v[i]][0];
    job_time.push_back(t);
  }
  int max_time = 1000000;
  int max_pos = -1;
  for (std::vector<int>::size_type i = 0; i < job_time.size(); i++) {
    if (job_time[i] < max_time) {
      max_time = job_time[i];
      max_pos = i;
    }
  }
  return v[max_pos];
}


int job_compare_LPT(std::vector<int> v, std::vector<vector<int>> time){
  std::vector<int> job_time;
  for (std::vector<int>::size_type i = 0; i < v.size(); i++) {
    int t = time[v[i]][0];
    job_time.push_back(t);
  }
  int max_time = -1;
  int max_pos = -1;
  for (std::vector<int>::size_type i = 0; i < job_time.size(); i++) {
    if (job_time[i] > max_time) {
      max_time = job_time[i];
      max_pos = i;
    }
  }
  return v[max_pos];
}


int job_compare_LRPT(std::vector<int> v, std::vector<vector<int>> time){
  std::vector<int> job_time;
  for (std::vector<int>::size_type i = 0; i < v.size(); i++) {
    int t = 0;
    for (std::vector<int>::size_type j = 0; j < time[v[i]].size(); j++) {
      t = t + time[v[i]][j];
    }
    job_time.push_back(t);
  }
  int max_time = -1;
  int max_pos = -1;
  for (std::vector<int>::size_type i = 0; i < job_time.size(); i++) {
    if (job_time[i] > max_time) {
      max_time = job_time[i];
      max_pos = i;
    }
  }
  return v[max_pos];
}


int job_compare_SRPT(std::vector<int> v, std::vector<vector<int>> time){
  std::vector<int> job_time;
  for (std::vector<int>::size_type i = 0; i < v.size(); i++) {
    int t = 0;
    for (std::vector<int>::size_type j = 0; j < time[v[i]].size(); j++) {
      t = t + time[v[i]][j];
    }
    job_time.push_back(t);
  }
  int max_time = 1000000;
  int max_pos = -1;
  for (std::vector<int>::size_type i = 0; i < job_time.size(); i++) {
    if (job_time[i] < max_time) {
      max_time = job_time[i];
      max_pos = i;
    }
  }
  return v[max_pos];
}

void writeToFile(const char* problem_name, int time, std::vector<vector<int>> result){
  ofstream file;
  file.open("results.txt", std::ios_base::app);
  file << problem_name;
  file << "\n  \n";
  file << time;
  file << "\n  \n";
  for(std::vector<vector<int>>::size_type x=0;x<result.size();x++)  // loop 3 times for three lines
    {
      for(std::vector<int>::size_type y=0;y<result[x].size();y++)  // loop for the three elements on the line
      {
        file << result[x][y];
        file << " ";
      }
      file << "\n";
    }
  file << "----------------------------------------------------------------\n";
  file.close();
}


result test_result(){
  std::vector<std::vector<int>> vec = {{1, 0, 2}, {2, 1, 0}, {0, 1, 2}};
  result r;
  r.RM = vec;
  r.makeSpan = 40;
  return r;
}


problem_data test_data(){
  problem_data data;
  std::vector<std::vector<int>> operations = {{2, 0, 1}, {0, 1, 2}, {1, 0, 2}};
  std::vector<std::vector<int>> cost = {{14, 11, 6}, {6, 6, 14}, {7, 6, 9}};
  data.J = 3;
  data.M = 3;
  data.OM = operations;
  data.CM = cost;
  return data;
}

void tabuSearch(problem_data data, const char* problem_name){
  result r = greedy(data, problem_name);
  //result r = test_result();
  //problem_data data2 = test_data();
  std::cout << "TabuSearch Algorithm: " << '\n';
  //printMatrix(r.RM);
  result aux; 
  tabuList tl;
  tl.list = {};
  tl.max_len = (data.M + data.J)/2; //largo lista tabu
  std::cout << "Largo lista tabu: " << tl.max_len << '\n';
  result first_result = r;
  result best_result = r;
  for (int i = 0; i < 1000; ++i)
  {
    std::cout << "tabuSearch iteration " << i << "\n";
    //printMatrix(tl.list);
     aux = CPM(data, r, tl);
     if (aux.makeSpan == 100000000)
     {
       cout << "No hay mas movimientos posibles, iteracion final: " << i << endl;
       break;   
     }
     else{
      if (aux.makeSpan < best_result.makeSpan)
      {
        best_result = aux;
      }
      r = aux;
     }
     //printMatrix(r.RM);
  }
  cout << "La solución inicial tiene un tiempo de " << first_result.makeSpan << "." << endl;
  cout << "El mejor resultado con un tiempo de " << best_result.makeSpan << " es:" << endl;
  printMatrix(best_result.RM);
  writeToFile(problem_name, best_result.makeSpan, best_result.RM);
}

result CPM(problem_data data, result r, tabuList &tl){
    //cout << "CMP" << endl;
    Graph graph(data, r.RM);
    std::vector<std::vector<int>> swap = graph.swapCandidates();
    //cout << "SWAP: " << swap.size() << endl;
    //printMatrix(swap);
    result new_best;
    new_best.makeSpan = 100000000;
    std::vector<int> best_swap = {};
    for (int j = 0; j < swap.size(); ++j)
    {
      if (notTabu(swap[j], tl))
      { 
        //std::cout << "New Result" << '\n';
        std::vector<std::vector<int>> new_result = makeSwap(r.RM, swap[j]);
        //printMatrix(new_result);
        Graph aux(data, new_result);
        //cout << aux.getMakeSpan() << endl;
        if (aux.getMakeSpan() < new_best.makeSpan)
        {
          new_best.RM = new_result;
          new_best.makeSpan = aux.getMakeSpan();
          best_swap = swap[j];
        }
      }
    }
    /*std::cout << "New Best" << '\n';
    std::cout << new_best.makeSpan << '\n';*/
    //std::cout << "TabuList Size: " << tl.list.size() << '\n';
    if (tl.list.size() < tl.max_len)
    {
      tl.list.push_back(best_swap);
    }
    else if(tl.list.size() == tl.max_len){
      tl.list.erase(tl.list.begin());
      tl.list.push_back(best_swap);
    }
    //std::cout << "TabuList Size: " << tl.list.size() << '\n';
    return new_best;
}

std::vector<std::vector<int>> makeSwap(std::vector<std::vector<int>> r, std::vector<int> swap){
  std::vector<std::vector<int>> newResult = r;
  int machine = swap[0];
  int node1 = swap[1];
  int node2 = swap[2];
  for (int i = 0; i < newResult[machine].size()-1; ++i)
  {
    if (newResult[machine][i] == node1)
    {
      newResult[machine][i] = node2;
      newResult[machine][i+1] = node1;
      break;
    }
  }
  return newResult;

}

int notTabu(std::vector<int> swap, tabuList &tl){
  for (int i = 0; i < tl.list.size(); ++i)
  {
    if (tl.list[i][0] == swap[0] && ((tl.list[i][1] == swap[1] && tl.list[i][2] == swap[2]) || (tl.list[i][1] == swap[2] && tl.list[i][2] == swap[1])))
    {
      return 0;
    }
  }
  return 1;
}
