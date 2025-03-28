#include <vector>
#include <string>

#pragma once

using namespace std;

class Scheme : public vector<string> {

    public:
   
     Scheme(vector<string> names) : vector<string>(names) { }
     
     Scheme() { }
   
   };