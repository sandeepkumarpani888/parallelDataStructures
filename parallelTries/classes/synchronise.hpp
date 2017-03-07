#ifndef SYNCHRONISE_H_
#define SYNCHRONISE_H_

#include <bits/stdc++.h>
#include "trie.hpp"
#include "information.hpp"
#include <boost/algorithm/string.hpp>

class synchronise{
	std::vector<std::string> testString;
	std::vector<std::vector<std::string> > files;
	std::vector<Trie*> triesCollection;
	std::atomic<int> counter;
public:

	void insertToTrie(Trie *trie,const std::string &str){
		int strLen=str.size();
		Trie *root=trie;
		for(int i=0;i<strLen;i++){
			root->lockAccess();
			if(i==strLen-1){
				root->addLastNode(str[i],str);
			}
			else{
				root->addNode(str[i]);
			}
			root->unlockAccess();
			root=root->getNextNode(str[i]);
		}
		root=trie;
	}

	void makeTries(Trie *trie,const std::vector<std::string> &file){
		std::vector<std::thread> threads;
		std::vector<std::future<void> > ftr;
		for(auto& str: file){
			ftr.push_back(std::move(std::async(
				&synchronise::insertToTrie,this,
				trie,str)));
		}
		for(auto& fut:ftr){
			fut.get();
		}
	}

	//PARSE THE FILE AND MAKE std::vector
	void init(std::string fileName){
		this->counter=0;
		std::ifstream infile;
		infile.open(fileName);
		std::string line;
		std::vector<std::string> strs;
		if(infile.is_open()){
			std::vector<std::string> strings;
			while(getline(infile,line)){
				// error(line);
				boost::split(strings,line,boost::is_any_of(" "));
				strs.insert(strs.end(),strings.begin(),strings.end());
			}
		}
		this->testString=strs;
		int totalSize=strs.size();
		std::cout<<"Total Size: "<<totalSize<<std::endl;
		totalSize/=PARTS;
		for(int i=0;i<PARTS;i++){
			int start1=totalSize*i;
			int end1=std::min(totalSize*(i+1)-1,(int)strs.size()-1);
			std::vector<std::string> stringVector;
			std::cout<<"start1 : end1 "<<"("<<start1<<" : "<<end1<<")\n";
			for(int j=start1;j<=end1;j++){
				stringVector.push_back(strs[j]);
			}
			this->files.push_back(stringVector);
		}
	}

	void initTries(){
		for(int i=0;i<files.size();i++){
			triesCollection.push_back(new Trie());
		}
		std::vector<std::thread> threads;
		std::vector<std::future<void> > ftr;
		for(int i=0;i<files.size();i++){
			ftr.push_back(std::move(std::async(
				&synchronise::makeTries,this,
				triesCollection[i],
				files[i])));
		}
		for(auto& th : ftr){
			th.get();
		}
	}

	bool serachStringInTrie(Trie* trie,std::string strs){
		Trie *root=trie;
		this->counter++;
		int x1=this->counter.load(std::memory_order_relaxed);
		for(int i=0;i<strs.size();i++){
			if(root->checkIfNull(strs[i])){
				this->counter--;
				return false;
			}
			else{
				root=root->getNextNode(strs[i]);
			}
		}
		this->counter--;
		x1=this->counter.load(std::memory_order_relaxed);
	}

	bool serachString(const std::string strs){
		std::vector<std::future<bool> > fut;
		std::vector<bool> possVals;
		bool isPossible=false;
		for(int i=0;i<this->files.size();i++){
			possVals.push_back(false);
			fut.push_back(std::move(std::async(
				std::launch::async,
				&synchronise::serachStringInTrie,this,
				triesCollection[i],
				strs)));
		}
		for(int i=0;i<fut.size();i++){
			isPossible=isPossible | fut[i].get();
		}
		return isPossible;
	}

	void testFunction(){
		clock_t t;t=clock();
		for(auto& str:this->testString){
			serachString(str);
		}
		t=clock()-t;
		printf("%lld === %.6lf,%d\n",t,((double)t)/CLOCKS_PER_SEC,(int)(this->counter));
	}
};

#endif