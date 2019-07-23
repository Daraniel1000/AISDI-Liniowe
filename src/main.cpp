#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Vector.h"
#include "LinkedList.h"
#define nrtests 2

using namespace std;
using namespace chrono;


void perfomTest ()
{
  time_point<system_clock> start, stop;
  duration<double> dl[7][2];

  aisdi::LinkedList<int> list;
  aisdi::Vector<int> vector1;
  aisdi::Vector<int> vector2;
  const int size_n = 10000;

  start = system_clock::now();
  for(int i = 0; i < size_n; i++)
    list.append(5);
  stop = system_clock::now();
  dl[0][0] = stop - start;

  start = system_clock::now();
  for(int i=0; i < size_n; i++)
    list.prepend(5);
  stop = system_clock::now();
  dl[1][0] = stop-start;

  start = system_clock::now();
  for(int i = 0; i < size_n; i++)
    list.popLast();
  stop = system_clock::now();
  dl[2][0] = stop-start;

  start = system_clock::now();
  for(int i = 0; i < size_n; i++)
    list.popFirst();
  stop = system_clock::now();
  dl[3][0] = stop-start;

  for(int i = 0; i < size_n; i++)
    list.append(5);
  start = system_clock::now();
    list.erase(list.cbegin(), list.cend());
  stop = system_clock::now();
  dl[4][0] = stop-start;

  for(int i = 0; i < size_n; i++)
    list.append(5);
  start = system_clock::now();
  list.insert(list.cbegin() + 10, 6);
  stop = system_clock::now();
  dl[5][0] = stop-start;

  start = system_clock::now();
  list.erase(list.cbegin() + 10);
  stop = system_clock::now();
  dl[6][0] = stop-start;


//##############VECTOR##############


  start = system_clock::now();
  for(int i = 0; i < size_n; i++)
    vector1.prepend(5);
  stop = system_clock::now();
  dl[0][1] = stop-start;

  start = system_clock::now();
  for(int i=0; i < size_n; i++)
    vector2.append(5);
  stop = system_clock::now();
  dl[1][1] = stop-start;

  start = system_clock::now();
  for(int i=0; i < size_n; i++)
    vector1.popFirst();
  stop = system_clock::now();
  dl[2][1] = stop-start;

  start = system_clock::now();
  for(int i=0; i < size_n; i++)
    vector2.popLast();
  stop = system_clock::now();
  dl[3][1] = stop-start;

  for(int i=0; i < size_n; i++)
    vector2.append(5);
  start = system_clock::now();
  vector2.erase(vector2.cbegin(), vector2.cend());
  stop = system_clock::now();
  dl[4][1] = stop-start;

  for(int i = 0; i < size_n; i++)
    vector2.append(5);
  start = system_clock::now();
  vector2.insert(vector2.cbegin() + 10, 6);
  stop = system_clock::now();
  dl[5][1] = stop-start;

  start = system_clock::now();
  vector2.erase(vector2.cbegin() + 10);
  stop = system_clock::now();
  dl[6][1] = stop-start;

  cout<<"prepend "<<size_n<<" elements:\nLinkedList time: "<<dl[0][0].count()<<"\nVector time: "<<dl[0][1].count()<<"\n\n";
  cout<<"append "<<size_n<<" elements:\nLinkedList time: "<<dl[1][0].count()<<"\nVector time: "<<dl[1][1].count()<<"\n\n";
  cout<<"popFirst "<<size_n<<" elements:\nLinkedList time: "<<dl[2][0].count()<<"\nVector time: "<<dl[2][1].count()<<"\n\n";
  cout<<"popLast "<<size_n<<" elements:\nLinkedList time: "<<dl[3][0].count()<<"\nVector time: "<<dl[3][1].count()<<"\n\n";
  cout<<"erase from begin to end "<<size_n<<" elements:\nLinkedList time: "<<dl[4][0].count()<<"\nVector time: "<<dl[4][1].count()<<"\n\n";
  cout<<"insert inside of collection\nLinkedList time: "<<dl[5][0].count()<<"\nVector time: "<<dl[5][1].count()<<"\n\n";
  cout<<"erase inside of collection\nLinkedList time: "<<dl[6][0].count()<<"\nVector time: "<<dl[6][1].count()<<"\n\n";

}

int main(/*int argc, char** argv*/)
{
  for(int i=0; i<nrtests; i++)
  {
    cout<<"##########################################################\n\n";
    perfomTest();
  }
  return 0;
}
