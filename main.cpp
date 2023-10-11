//This is MPI version of
//Algorithm 1 Diversification-Driven Tabu Search (D2TS) for UBQP
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector> 
#include <algorithm>
#include <tuple>
#include <utility>
#include <bits/stdc++.h>
#include <chrono>
#include <random>
///user headers
#include "tabu_search.h"
#include "bqp.h"
#include <mpi.h>

using namespace std;

tuple<int, int, int> read_qubo(string fname);
std::vector<std::vector<double>> fill_qubo(string fname,int nNodes,int nmin);
//structure to store local energy and its rank
struct eloc
{
  double energy;
   int location;
};


int main(int argc, char *argv[])
{
//Mpi initialize
	MPI_Init(NULL,NULL);
	int com_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//variables for all nodes
int iter_max;
long int time_out;
  int nmin,nmax,nNodes;
vector<vector<double>> Q1;
 struct eloc localE,globalE; //storing local energy and its rank
  	unsigned int seed=0;
//parsing arg and file reading

  if(rank==0)
  {
  char *inFileName = NULL;
  FILE *inFile = NULL;
  iter_max=0;
  time_out=0; //milli seconds
	//BQP bqp(Q);
  //parse arguments
  for(int i=1;i<argc;++i)
	{
	 // cout << argv[i] << "\n";
	  string s=argv[i];
	  //cout<<s<<endl;
	  if(s.compare("-i")==0)  
	    {
	      inFileName=argv[i+1];
	    }
	  if(s.compare("-it")==0)  
	    {
	      iter_max=atoi(argv[i+1]);
	    }
	
	if(s.compare("-t")==0) 
	    {
	      time_out=atoi(argv[i+1]);
	    }
	if(s.compare("-s")==0) 
	    {
	      seed=atoi(argv[i+1]);
	    }
	} 


  //read file
  string fname(inFileName);
tie(nmin,nmax,nNodes) = read_qubo(fname);
	
	cout<<"found nodes "<<nNodes<<"("<<nmin<<","<<nmax<<")"<<endl;
	//fill qubo matrix 2D vector
	//allot Q
	//vector<vector<double>> Q1; 

	Q1=fill_qubo(fname,nNodes,nmin);
  
	//bqp.printQ();
//set paramters  default values if not provided
    if(iter_max==0) iter_max=20*nNodes; //T*:=max(500000,5000n) /* set maximum number of iterations */
	if(time_out==0) time_out=300;
  
//display paramters	
  cout<<"tabu_search will run using supplied file "<<inFileName <<" and "; 
  cout<<"max_iterations ="<<iter_max<<" or time_out = "<<time_out<<" ms"<<endl;
  }
  

  //broadcast necessary variables
  int root=0;
  MPI_Bcast(&iter_max,1,MPI_INT,root,MPI_COMM_WORLD);
   MPI_Bcast(&time_out,1,MPI_LONG_INT ,root,MPI_COMM_WORLD);
  MPI_Bcast(&nNodes,1,MPI_INT ,root,MPI_COMM_WORLD); 
  MPI_Bcast(&seed,1,MPI_INT ,root,MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);
//set seed
  	if(seed==0)
	{
   	srand(time(0));
	}
	else
	{srand(seed);}
  MPI_Barrier(MPI_COMM_WORLD);

//bcast Q1
//resize vectors

if(rank!=0)
{
  //	cout<<nNodes<<" "<<iter_max<<" "<<time_out<<endl;
	 //2d vector for qubo matrix storage on each process
	Q1.resize( nNodes , vector<double>( nNodes , 0.0 ) );
	//  vector<vector<double>> Q1( nNodes , vector<double> (nNodes, 0.0));

}
MPI_Barrier(MPI_COMM_WORLD);

for(int i=0;i<Q1.size();++i)
{	
	  MPI_Bcast(&Q1[i][0],nNodes,MPI_DOUBLE,root,MPI_COMM_WORLD);
}

MPI_Barrier(MPI_COMM_WORLD);
  ///start of code
	//initiate local solution
	std::vector<int> initSol(nNodes,0);
	for (int i = 0; i < nNodes;i++) 
	{
	  initSol[i]=rand()%2;
	}
	
///constants values 
	int tenure=min(20,nNodes/4);
	int numRestarts=100;
	seed=rand();
	double energyThreshold=-100000.00;
	double t2,t1=MPI_Wtime();
	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); //start timer

	//TABU SEARCH
	int local_time_out=time_out/com_size;
	TabuSearch ts=TabuSearch(Q1, initSol,tenure,local_time_out,numRestarts,seed,energyThreshold);
	//double energy_local=ts.bestEnergy();
	std::vector<int> solution_local;
	solution_local=ts.bestSolution();
	localE.energy=ts.bestEnergy();
	localE.location=rank;
	MPI_Allreduce(&localE, &globalE, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	t2 = MPI_Wtime() - t1;
	
	if(rank==globalE.location)
	  {
	    std::cout<<" num restarts "<<ts.numRestarts()<<std::endl;
	    std::cout<<"energy = "<<localE.energy<<std::endl;
        //print solution                                                                                                                                                                             
	    cout<<"best solution is"<<endl;
	    for(int i=0;i<nNodes;++i) cout<<solution_local[i];
	    cout<<endl;

	  }
	MPI_Barrier(MPI_COMM_WORLD);

	  if(rank==0)
	    {    
	      cout<<" total time taken"<<t2<<endl;
	    }
	MPI_Finalize();
	
	return 0;
}

