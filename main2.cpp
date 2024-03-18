#include <iostream>
#include <map>
#include "server.hpp"
#include "user.hpp"

int main ()
{
  std::map<int,User> mymap;
  std::map<int,User>::iterator it;

  mymap.insert(std::pair<int, User>(0, User()));
  mymap.insert(std::pair<int, User>(1, User()));

  it = mymap.find(1);
  if (it != mymap.end())
  {
	// User tmp;
	// tmp = it->second;
    // tmp.setStatus("come va=");
    it->second.setStatus("come va=");
  }
  mymap[0].setStatus("ciaooo");

  // print content:
  std::cout << "elements in mymap:" << '\n';
  std::cout << "0 => " << mymap.find(0)->second.getStatus() << '\n';
  std::cout << "1 => " << mymap.find(1)->second.getStatus() << '\n';
  
  return 0;
}

// int main ()
// {
//   std::map<char,int> mymap;
//   std::map<char,int>::iterator it;

//   mymap['a']=50;
//   mymap['b']=100;
//   mymap['c']=150;
//   mymap['d']=200;

//   it = mymap.find('b');
//   if (it != mymap.end())
//     it->second = 3;

//   // print content:
//   std::cout << "elements in mymap:" << '\n';
//   std::cout << "a => " << mymap.find('a')->second << '\n';
//   std::cout << "b => " << mymap.find('b')->second << '\n';
//   std::cout << "c => " << mymap.find('c')->second << '\n';
//   std::cout << "d => " << mymap.find('d')->second << '\n';

//   return 0;
// }