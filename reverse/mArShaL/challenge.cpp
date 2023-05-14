#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ios>
#include <stdio.h>
#include <sys/ptrace.h>
#include <iostream>
#include <fstream>
#include "./json/single_include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

string glob;

void check() __attribute__((destructor));
void check(){
 
  json obj;
  if (ptrace(PTRACE_TRACEME,0) < 0) {
  int i = 0;
  int j = 0;
  int k = 0;

  while (i < 10) {
    if (j < 5) {
      j++;
    } else {
      j--;
    }
    i++;
  }

  for (int n = 0; n < 10; n++) {
    if (n % 2 == 0) {
      k += 2;
    } else {
      k -= 1;
    }
  }

  switch (k) {
    case 0:
      i++;
      break;
    case 1:
      j++;
      break;
    case 2:
      i--;
      break;
    case 3:
      j--;
      break;
    default:
      k = 0;
  }

  do {
    i++;
  } while (i < 20);

  for (int m = 0; m < 5; m++) {
    if (m == 0) {
      k += 1;
    } else if (m == 1) {
      j += 2;
    } else if (m == 2) {
      i -= 1;
    } else if (m == 3) {
      k -= 2;
    } else {
      j -= 1;
    }
  }
      printf("result : eW91J3JlIGNsb3NlIGhlaGU=\n");
      exit(0);
  }
  else {
    ofstream myfile;
    myfile.open ("obj.marshal", ios::out | ios::binary);   
    try {
    std::ifstream f(glob);
    obj = json::parse(f);
  }
catch (const std::exception& e) {
  std::cout <<"Enter a valid name of a JSON formatted file."<<std::endl;
  exit(1);
}
    vector<std::uint8_t> v_bson = json::to_bson(obj);
    int n=v_bson.size();
    int c=0; 
    unsigned char arr[n];
    for (auto i : v_bson) {
        arr[c]=(i+c)%256;
        c++;
    }
    int temp;
    for(int i = 0; i<n/2; i++){
        temp = arr[i];
        arr[i] = arr[n-i-1];
        arr[n-i-1] = temp;
    }
    for (char i : arr) {
      myfile<<i;
    }
  myfile.close();
}}
int main(int argc, char** argv)
{
  cout<<R"(
 __  __         ____        _   _         _     
|  \/  |  __ _ |  _ \  ___ | | | |  __ _ | |    
| |\/| | / _` || |_) |/ __|| |_| | / _` || |    
| |  | || (_| ||  _ < \__ \|  _  || (_| || |___ 
|_|  |_| \__,_||_| \_\|___/|_| |_| \__,_||_____|
                                                
)";
  cout<<"Enter the the name object you want to securly marshal in JSON format (e.g. file.json) : ";
  cin>>glob; 
  int i = 0;
  if (i == 0) {
    i++;
  } else if (i == 1) {
    i--;
  }
  switch (i) {
    case 0:
      i += 2;
      break;
    case 1:
      i -= 2;
      break;
    default:
      i = 0;
  }
  while (i < 10) {
    i++;
  }
  do {
    i--;
  } while (i > 0);
  for (int j = 0; j < 5; j++) {
    i *= 2;
  }
  for (int j = 0; j < 5; j++) {
    i /= 2;
  }
    return 0;
}
