#ifndef INFORMATION_H_
#define INFORMATION_H_

#include <bits/stdc++.h>

class information{
private:
	bool isEnding;
	std::string memo;
	std::mutex mtx;

public:
	information(){
		this->isEnding=false;
		this->memo="";
	}

	bool getIsEnding(){
		return this->isEnding;
	}

	void initialise(bool _isEnding = false){
		this->isEnding=_isEnding;
	}

	void setMemo(std::string _str=""){
		int len1=this->memo.size();
		int len2=_str.size();
		if(len1<len2){
			this->memo=_str;
		}
	}

	void lockM(){
		this->mtx.lock();
	}

	void unlockM(){
		this->mtx.unlock();
	}
};

#endif