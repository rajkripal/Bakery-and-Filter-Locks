#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <random>
#include <chrono>
#include <vector>

using namespace std;

class Bakery {
public:
  bool * flag;
  long long * label;
  int n;

  Bakery (int num) {
    flag = new bool[num];
    label = new long long[num];
    for (int i = 0; i < num; i ++) {
      flag[i] = false;
      label[i] = 0;
    }
    n = num;
  }
  ~Bakery () {
    delete[] flag;
    delete[] label;
  }

  void lock (int threadId) {
    int i = threadId;
    flag[i] = true;
    long long max = label[0];
    for (int j = 1; j < n; j ++) {
      if (label[j] > max) {
        max = label[j];
      }
    }
    if (max == LLONG_MAX) {
      cout << "ERROR: Label Value Overflows!!!" << endl;
      exit (-1);
    }
    label[i] = max + 1;

    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      } else {
        while (flag[j] && ((label[j] < label[i]) ||( label[i] == label[j] && j < i))) {};
      } 
    }
  }

  void unlock (int threadId) {
    flag[threadId] = false;
  }
};

Bakery * lock;

vector<string> list;


struct ThreadMeta {
  int id;
  int k;
  int l1;
  int l2;
  int count;
};

string printTime (time_t curtime) {
  struct tm* ptime = localtime(&curtime);
  char currentTime[80];
  strftime(currentTime, 80, "%T", ptime);
  return currentTime;
}

void* testCS (void* arg1) {
  struct ThreadMeta* data = (struct ThreadMeta*) arg1;
  int k = data -> k;
  int l1 = data -> l1;
  int l2 = data -> l2;
  int id = data -> id;

  double const exp_dist_lambda_1 = l1;
  double const exp_dist_lambda_2 = l2;


  int seed = chrono::system_clock::now().time_since_epoch().count();

  default_random_engine rg(seed);
  exponential_distribution <double> dist_1(exp_dist_lambda_1);
  exponential_distribution <double> dist_2(exp_dist_lambda_2);

  double t1d = dist_1(rg) + 0.5;
  double t2d = dist_2(rg) + 0.5;

  int t1 = (int)t1d;
  int t2 = (int)t2d;
  
  string strfn = "outputFile";
  stringstream ss;
  ss << id;
  strfn = strfn + ss.str();
  strfn = strfn + ".txt";

  char* fileName = new char[strfn.length() + 1];
  strcpy(fileName, strfn.c_str());
  ofstream ofile;
  ofile.open(fileName);

  for (int i = 0; i < k; ++i) {
    time_t reqEnterTime = time(NULL);
    data -> count ++;
    string vecstring = to_string(data -> count) + "th CS request at ";
    vecstring = vecstring + printTime(reqEnterTime);
    vecstring = vecstring + " of thread ";
    vecstring = vecstring + to_string(id);
    list.push_back(vecstring);
    ofile << data -> count << "th CS request at ";
    ofile << printTime(reqEnterTime);
    ofile <<  " of thread " << id << endl;

    lock -> lock(id);
    time_t actEnterTime = time(NULL);
    string vecstring1 = to_string(data -> count) + "th CS entry at ";
    vecstring1 = vecstring1 + printTime(actEnterTime);
    vecstring1 = vecstring1 + " of thread ";
    vecstring1 = vecstring1 + to_string(id);
    list.push_back(vecstring1);
    ofile << data -> count << "th CS entry at ";
    ofile << printTime(actEnterTime);
    ofile <<  " of thread " << id << endl;
    sleep(t1);
    lock -> unlock(id);

    time_t exitTime = time(NULL);
    string vecstring2 = to_string(data -> count) + "th CS exit at ";
    vecstring2 = vecstring2 + printTime(exitTime);
    vecstring2 = vecstring2 + " of thread ";
    vecstring2 = vecstring2 + to_string(id);
    list.push_back(vecstring2);
    ofile << data -> count << "th CS exit at ";
    ofile << printTime(exitTime);
    ofile <<  " of thread " << id << endl;
    sleep(t2);
  }
  
  ofile.close();
}

int main () {
  int n, k, l1, l2, check;
  ifstream infile("inp-params.txt");

  while(!infile.eof()) {
    infile >> n >> k >> l1 >> l2;
  }

  cout << "n: " << n << endl << "k: " << k << endl << "l1: " << l1 << endl << "l2: " << l2 << endl;
  
  infile.close()

  lock = new Bakery (n);

  pthread_t threads[n];
  struct ThreadMeta data[n];
  for (int i = 0; i < n; i++) {
    data[i].id = i;
    data[i].k = k;
    data[i].l1 = l1;
    data[i].l2 = l2;
    data[i].count = 0;
    check = pthread_create(&threads[i], NULL, testCS, (void*)&data[i]);
    if (check) {
      cout << "Error in creating thread " << i << " " << endl;
      exit(-1);
    }
  }
  for (int i = 0; i < n; i++) {
    pthread_join(threads[i], NULL);
  }
  delete lock;

  ofstream outfile;
  outfile.open("outputFile.txt");
  for(vector<string>::iterator it = list.begin(); it != list.end(); ++it) {
    outfile<<*it<<endl;
  }
  outfile.close();

  return 0;
}
