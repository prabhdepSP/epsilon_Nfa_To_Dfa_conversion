#include <bits/stdc++.h>
using namespace std;
string getLexicString(string arg){
	sort(arg.begin(),arg.end());
	return arg;
}
bool contain(string str1,char ch){
	int length = str1.length();
	for(int i=0;i<length;i++){
		if(str1[i] == ch) return true;
	}
	return false;
}
string removeDuplicate(string sName){
	string retValue;
	unordered_set<char> rmDup;
	for(int i=0;i<(int)sName.length();i++) rmDup.insert(sName[i]);
	for(auto itr = rmDup.begin();itr != rmDup.end();itr++) retValue = retValue+(*itr);
	return getLexicString(retValue);
}
string takeUnionFromClosure(string closureString,map<char,string>& table){
	int length = closureString.length();
	string retValue;
	for(int i=0;i<length;i++){
		auto itr = table.find(closureString[i]);
		if(itr != table.end()){
			string closure = itr->second;
			retValue = retValue+closure;
		}
	}
	return removeDuplicate(retValue);
}
string takeUnionFromTable(string closureString,char input,map<char,map<char,vector<string>>>& table){
	int length = closureString.length();
	string retValue;
	for(int i=0;i<length;i++){
		auto itr = table.find(closureString[i]);
		if(itr != table.end()){
			map<char,vector<string>> value = itr->second;
			auto tmpItr = value.find(input);
			if(tmpItr != value.end()){
				vector<string> reachedState = tmpItr->second;
				retValue = retValue+reachedState[0];
			}
		}
	}
	return removeDuplicate(retValue);
}
string calculateClosure(map<char,map<char,vector<string>>>& table,int numStates,char start){
	string retValue;
	bool visited[26];
	for(int i=0;i<26;i++) visited[i] = false;
	visited[start-'A'] = true;
	vector<string> closure;
	stack<char> dfs_stack;
	dfs_stack.push(start);
	char star = start;
	while(!dfs_stack.empty()){
		//char closure = dfs_stack.top();
		auto itr = table.find(star);
		if(itr != table.end()){
			map<char,vector<string>> tmp =  itr->second;
			auto itr1 = tmp.find('$');
			if(itr1 != tmp.end()){
				vector<string> tmpVec = itr1->second;
				for(auto itr2 = tmpVec.begin();itr2 != tmpVec.end();itr2++){
					char ch = (*itr2)[0];
					if(!visited[ch-'A']){
						visited[ch-'A'] = true;
						dfs_stack.push(ch);
					}
				}
			}
		}
		char closure = dfs_stack.top();
		dfs_stack.pop();
		retValue = retValue+closure;
		star = closure;
	}
	return retValue;
}
void nfaToDfa(map<char,map<char,vector<string>>>& table,map<char,string>& closureTable,string inputSymbolsArray,int numInput,map<string,map<char,string>>& dfaTable,char& beginState){
	queue<string> newStates;
	int j;
	auto itr1 = closureTable.find(beginState);
	if(itr1 != closureTable.end())
		newStates.push(itr1->second);
	while(!newStates.empty()){
		string dfaState = newStates.front();
		map<char,string> tmp;
		for(int i=0;i<numInput;i++){
			//tmp.clear();
			char input = inputSymbolsArray[i];
			if(input != '$'){
				string retUnionTable = takeUnionFromTable(dfaState,input,table);
			//	cout<<"from retuniontable: "<<retUnionTable<<endl;
				string retUnionclosure = takeUnionFromClosure(retUnionTable,closureTable);
			//	cout<<"from retUnionclosure: "<<retUnionclosure<<endl;
			//	cin>>j;
				if(dfaTable.find(retUnionclosure) == dfaTable.end()){
					newStates.push(retUnionclosure);
				}
				tmp.insert(pair<char,string>(input,retUnionclosure));	
			}
		}
		dfaTable[dfaState] = tmp;
		tmp.clear();
		newStates.pop();
	}
}
void takeInput(map<char,map<char,vector<string>>>& table,int numStates,char& beginState,char& finalState){
	int stag = numStates;
	while(numStates--){
		char sName;
		cout<<"enter state name\n";
		cin>>sName;
		if(numStates == 0) finalState = sName;
		if(numStates == stag-1) beginState = sName;
		int choice =1;
		cout<<"press 0 to enter information for this state 1 to not\n";
		cin>>choice;
		map<char,vector<string>> tmpInfo;
		for(int i=0;choice == 0;i++){
			char input;
			vector<string> tmp;
			cout<<"enter input symbol\n";
			cin>>input;
			bool newMadeState = false;
			cout<<"enter reached state\n";
			string reachedState;
			cin>>reachedState;
			if(input == '$'){
				auto itr = tmpInfo.find('$');
				if(itr != tmpInfo.end()){
					tmp = itr->second;
					newMadeState = true;
					tmp.push_back(reachedState);
					itr->second = tmp;
				}
				else{
					newMadeState = true;
					tmp.push_back(reachedState);
				}
			}
			else{
				auto itr = tmpInfo.find(input);
				if(itr != tmpInfo.end()){
					tmp  = itr->second;
					string stateName = tmp[0]+reachedState;
					tmp[0] = (tmp[0]+reachedState);
					itr->second = tmp;
				}
				else{
					newMadeState = true;
					if(tmp.size() == 0) tmp.push_back(reachedState);
				}
			}
			if(newMadeState) tmpInfo.insert(pair<char,vector<string>>(input,tmp));
			cout<<"To continue entering more information press 0 otherwise press 1\n";
			cin>>choice;
		}
		table[sName] = tmpInfo;
	}
}
int main(void){
	int numStates;
	cout<<"nter number of states :";
	cin>>numStates;
	cout<<""<<endl;
	int numInputSymbol;
	cout<<"enter number of input symbols\n";
	cin>>numInputSymbol;
	cout<<"enter all input symbol in string format\n";
	string inputSymbols;
	cin>>inputSymbols;
	map<char,map<char,vector<string>>> nfaTransTable;
	map<char,string> closureTable;
	char beginState;
	char finalState;
	cout<<"enter begining state fisrt and final sate at last\n";
	takeInput(nfaTransTable,numStates,beginState,finalState);
	/*
	   takeInput(nfaTransTable,numStates,numInputSymbol,beginState,finalState);
	   }*/
	for(auto itr = nfaTransTable.begin();itr != nfaTransTable.end();itr++){
		cout<<"state name:"<<itr->first<<"\n";
		auto value = itr->second;
		for(auto itr1 = value.begin();itr1 != value.end();itr1++){
			cout<<itr1->first<<"   ";
			vector<string> value2=itr1->second;
			int size = value2.size();
			for(int i=0;i<size;i++){
				cout<<value2[i]<<"  ";
			}
			cout<<"\n";
		}
	}

for(auto itr = nfaTransTable.begin();itr != nfaTransTable.end();itr++){
	string closure = calculateClosure(nfaTransTable,numStates,itr->first);
	closure = getLexicString(closure);
	closureTable.insert(pair<char,string>(itr->first,closure));
}
for(auto itr = closureTable.begin();itr != closureTable.end();itr++){
	cout<<"state name: "<<itr->first<<"   closure: "<<itr->second<<"\n";
}
map<string,map<char,string>> dfaTransTable;
cout<<beginState<<" final state  "<<finalState<<endl;
nfaToDfa(nfaTransTable,closureTable,inputSymbols,numInputSymbol,dfaTransTable,beginState);

cout<<"minimised dfa table\n";
for(auto itr = dfaTransTable.begin();itr != dfaTransTable.end();itr++){

	cout<<"state name: "<<itr->first<<endl;
	map<char,string> tmp = itr->second;
	for(auto itr1 = tmp.begin();itr1 != tmp.end();itr1++){
		cout<<"input : "<<itr1->first<<"  stateReached :"<<itr1->second<<endl;
	}
}
cout<<"list of final states\n";
for(auto itr = dfaTransTable.begin();itr != dfaTransTable.end();itr++){
	if(contain(itr->first,finalState)) cout<<itr->first<<"\n";
}

}
