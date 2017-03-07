#ifndef TRIE_H_
#define TRIE_H_

#include <bits/stdc++.h>
#include "information.hpp"

class Trie{
private:
	information info;
	Trie* next[256];
public:
	Trie(bool isEnding=false){
		info.initialise(isEnding);
		for(int i=0;i<256;i++){
			this->next[i]=NULL;
		}
	}

	Trie* getNextNode(int index){
		if(this->next[index]==NULL){
			return NULL;
		}
		else{
			return this->next[index];
		}
	}

	void addNode(int index,bool isEnding=false){
		if(this->next[index]==NULL){
			this->next[index]= new Trie(isEnding);
		}
	}

	void addLastNode(int index,std::string _str){
		if(this->next[index]==NULL){
			this->next[index]=new Trie(true);
		}
		assert(this->next[index]!=NULL);
		this->next[index]->info.setMemo(_str);
	}

	void lockAccess(){
		this->info.lockM();
	}

	void unlockAccess(){
		this->info.unlockM();
	}

	bool checkIfNull(int index){
		if(this->next[index]==NULL){
			return true;
		}
		else{
			return false;
		}
	}
};

#endif