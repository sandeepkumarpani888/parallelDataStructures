#include <bits/stdc++.h>
#include "synchronise.hpp"

int main(){
	synchronise syc;
	syc.init("../data/data.txt");
	syc.initTries();
	syc.testFunction();
}