#include <stdio.h>
#include <iostream>
#include <tuple>
#include <vector> 
#include <algorithm>
#include <fstream>
using namespace std;
tuple<int, int, int> read_qubo(string fname)
{
  int nmin, nmax, nNodes;
    string a;
  //file pointer
    std::ifstream ifp(fname, ios::in);
    if (ifp.fail())
    {
      cout<<"\n error -- no input file (-i option) specified"<<"\n\n"<<"ms2ts"<<endl;
      exit(9);
    }
    ifp.ignore(1000,'\n'); //ignore first line

  int f1,f2;
  double f3;
  vector <int> entries;
  int c=0;
  while(!ifp.eof())
    {
      c++;
      ifp >> f1;
      ifp >> f2;
      ifp >> f3;
      entries.push_back(f1);
      entries.push_back(f2);
      //  cout<<c<<" "<<f1<<" "<<f2<<" "<<f3<<endl;
    }
  nmax=*max_element(entries.begin(), entries.end());
  nmin=*min_element(entries.begin(), entries.end());
  nNodes=nmax-nmin+1;
  // cout<<"found nodes "<<nNodes<<"("<<nmin<<","<<nmax<<")"<<endl;
  return make_tuple(nmin, nmax, nNodes);
}
std::vector<std::vector<double>> fill_qubo(string fname,int nNodes,int nmin)
{
  //initiate a zero vector
  std::vector<std::vector<double>> qq;
  vector <double> aa;
  for(int i=0;i<nNodes;i++)aa.push_back(0);
  for(int i=0;i<nNodes;i++)qq.push_back(aa);
  
  int f1,f2;
  double f3;
  string a;
  //file pointer
  std::ifstream ifp(fname, ios::in);
  
  ifp.ignore(1000,'\n'); //ignore first line
  int c=0;
  //  while(!ifp.eof())
  while (!ifp.eof())
    {
      
      ifp >> f1;
      ifp >> f2;
      ifp >> f3;
      c++;
      if(f1!=f2)
	{	qq[f1-nmin][f2-nmin]=f3/2;  //equivalent symmetric matrix
	  qq[f2-nmin][f1-nmin]=f3/2;
	}
	else
	  {
	    qq[f2-nmin][f1-nmin]=f3;
	  }
      
      //      cout<<f1<<" "<<f2<<" "<<f3<<endl;
      // if(i % 9 == 0) v.resize(v.size() + 10);
    }
  //for(i=0;i<c;i++)cout<<v[i]<<" "<<v1[i]<<" "<<v2[i]<<endl;
  //  std::vector<int>::iterator it;
  // for(it = v.begin(); it != v.end(); ++it )
  // std:: cout << *it << ' ';
  //std::cout << endl;
  
  return qq;	
}
