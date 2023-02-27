#include <set>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


struct DefSort{
	int one;
	string bin;
};

class QuineMcCluskey{
    public:
        int variable;
		void mergeImplicant();
		void getPrime();
		void printPrime();
		void printPrime(ofstream &fout);
		vector<int> onSet;
        vector<int> dcSet;
        vector<DefSort> implicant;
        vector<DefSort> primeImplicant;
		//bool sortNumOne(DefSort, DefSort);
		//bool sortBinary(DefSort, DefSort);

    private:
		string decToBin(int dec, int *n);
		bool checkMerge(DefSort termA, DefSort termB, int *idx);
		//vector <DefSort> implicant;
};

//----------------------------------------------------
// determine of prime implicants
//----------------------------------------------------
//sort number of 1 as index  QuineMcCluskey::
bool sortNumOne(DefSort termA, DefSort termB){
	return (termA.one < termB.one);
}

//same index into groups, list binary ascending  of decimal
bool sortBinary(DefSort termA, DefSort termB){
	return (termA.bin < termB.bin);
}

//input
void QuineMcCluskey::getPrime(){

	//don't cares as required minterms
	int total = onSet.size() + dcSet.size();
	implicant.resize(total);
	//implicant.resize(onSet.size() + dcSet.size());
	
	int numOne, i;;
	int onSize = onSet.size();
	for(i=0; i<onSize; i++){
		implicant[i].bin = decToBin(onSet[i], &numOne);
		implicant[i].one = numOne;
	}
	
	for(; i<total; i++){
		implicant[i].bin = decToBin(dcSet[i-onSize], &numOne);
		implicant[i].one = numOne;
	}
	vector<DefSort> sortImplicant;
	sortImplicant = implicant;
	sort(implicant.begin(), implicant.end(), sortNumOne);
	mergeImplicant();
	implicant = sortImplicant;	
}

//represent each minterm by binary, find decimal for binary
string QuineMcCluskey::decToBin(int dec, int *n){
	*n = 0;
	string bin;
	for(int i=0; i<variable; i++){
		if(dec==0){
			bin += '0';
		}
		else if(dec!=0){
			bin += '0' + dec%2;
			dec /= 2;
			if(bin[i] == '1')
				*n += 1;
		}
	}
	reverse(bin.begin(), bin.end());
	return bin;
}

//from lowest index
bool QuineMcCluskey::checkMerge(DefSort termA, DefSort termB, int *idx){
	*idx = 0;
	int isMerge = 0;
	for(int i=0; i<variable; i++){
		if(termA.bin[i] != termB.bin[i]){
			*idx = i;
			isMerge++;
		}
	}
	return (isMerge==1);
}

//merge two implicants
// struct findMerge{
// 	string impA;
// 	findMerge(string impB):impA(impB){}
// 	bool operator()(const DefSort& m){
// 		//new implicant
// 		return m.bin == impA;
// 	}
// };

//----------------------------------------------------
// the prime implicant chart
//----------------------------------------------------

//check terms into merging
void QuineMcCluskey::mergeImplicant(){
	int idx;
	DefSort impA, impB;
	vector<DefSort> bk;
	//construct prime implicants chart
	vector<bool> check(implicant.size());
	int impSize;
	for(int i=0; i<variable-1; i++){
		impSize = implicant.size();
		//cout << "i = " << i << endl;
		for(int j=0; j<impSize; j++){
			for(int k=j+1; k<impSize; k++){
				impA = implicant[j];
				impB = implicant[k];
				if(checkMerge(impA, impB, &idx)){
					impA.bin[idx] = '-';
					bk.push_back(impA);
					// if(find_if(bk.begin(), bk.end(), findMerge(impA.bin)) == bk.end())
					// 	bk.push_back(impA);
					//cross under decimal contain prime implicant
					check[j] = true;
					check[k] = true;
					//cout << "Implicant: " << impA.bin << endl;

				}
				else if(impA.bin == impB.bin)
					j = j + 1;
			}
			//find single cross = prime implicants
			//left ones = prime implicant
			if(!check[j]){
				primeImplicant.push_back(implicant[j]);
				//cout << "Prime Implicant: " << primeImplicant.bin[j] <<endl;
			}
		}
		check.clear();
		implicant.clear();
		implicant = bk;
		bk.clear();
		check.resize(implicant.size());
	}
	sort(primeImplicant.begin(), primeImplicant.end(), sortBinary);
}

void QuineMcCluskey::printPrime(){
	int primeSize = primeImplicant.size();
    cout << ".p " << primeSize << endl;
	//int size = primeImplicant.size();
	if(primeSize>15){
		primeSize = 15;
		for(int i=0; i<primeSize; i++)
    		cout << primeImplicant[i].bin << endl;
	}	
	else if(primeSize<=15){
		for(int i=0; i<primeSize; i++)
			cout << primeImplicant[i].bin << endl;
	}
}

void QuineMcCluskey::printPrime(ofstream &fout){
	int primeSize = primeImplicant.size();
    fout << ".p " << primeSize << endl;
	//int size = primeImplicant.size();
	if(primeSize>15){
		primeSize = 15;
		for(int i=0; i<primeSize; i++)
    		fout << primeImplicant[i].bin << endl;
	}	
	else if(primeSize<=15){
		for(int i=0; i<primeSize; i++)
			fout << primeImplicant[i].bin << endl;
	}
}
//----------------------------------------------------
// petrick's method
//----------------------------------------------------

class PetrickMethod{
	public:
		int variable, literal;
		int idx;
		//bool sortSOP(const set<string>, const set<string>);
		//void printMinCover(vector<DefSort>, vector<DefSort>, int);
		void Pfunction(vector<DefSort> onset, vector<DefSort> prime, int size);
		void printMinCover();
		void printMinCover(ofstream &fout);
		//vector< <vector<string> > pos;
		//vector< set<string> > sop;
		vector< vector<string> > pos;
  		vector< set<string> > sop;

	private:
		string toString(set<string> &);
		bool removeCover(string, string);
		void toPOS(vector<DefSort>, vector<DefSort>, int);
		void PosToSop(set<string>, int, int);
		//void Pfunction(vector<DefSort> onset, vector<DefSort> prime, int size);
		set<string> sopString;
};

//essential prime implicants and minterms they cover removed
bool PetrickMethod::removeCover(string onset, string prime){
	for(int i=0; i<variable; i++){
		if(prime[i] != '-'  &&  prime[i] != onset[i])
			return false; 
		// if(prime[i] == '-'  ||  prime[i] == onset[i])
		// 	return true; 
	}
	return true;
	// return false; 
}

void PetrickMethod::toPOS(vector<DefSort> onset, vector<DefSort> prime, int num){
	vector<string> posString;
	int primeSize = prime.size();
	for(int i=0; i<num; i++){
		posString.clear();
		for(int j=0; j<primeSize; j++){
			if(removeCover(onset[i].bin, prime[j].bin))
				posString.push_back(prime[j].bin);
		}
		pos.push_back(posString);
	}
}

string PetrickMethod::toString(set<string> &bkString){
	string s;
	stringstream ss;
	set<string>::iterator it;
	set<string>::iterator beg = bkString.begin();
	set<string>::iterator end = bkString.end();

	for(it=beg; it!=end; ++it)
        ss << *it;

	// int bkSize = bkString.size();
	// for(int i=0; i<bkSize; i++)
	// 	ss << *bkString[i];

	// int bkSize = bkString.size();
	// for(size_t i=0; i<bkString.size(); ++i)
	// 	ss = bkString[i];
	ss >> s;
	return s;
}

//reduce P to minimum sop/pos
bool sortSOP(const set<string> sopA, const set<string> sopB){
	return (sopA.size() < sopB.size());
}

//form a true logic P function covered all minterms
void PetrickMethod::Pfunction(vector<DefSort> onset, vector<DefSort> prime, int size){
	toPOS(onset, prime, size);
	//removeCover(onset, prime);

	set<string> product;
	PosToSop(product, 0, pos.size());
	sort(sop.begin(), sop.end(), sortSOP);

	vector<int> bkLiteral;
	set<string>::iterator it;
	int sopSize = sop.size();
	for(int i=0; i<sopSize; i++){
		literal = 0;
		set<string>::iterator beg = sop[i].begin();
		set<string>::iterator end = sop[i].end();
		for(it=beg; it!=end; ++it){
			for(int i=0; i<variable; i++){
				//count 1 and 0 in sop
				if((*it)[i]!='-')
					literal++;
			}
		}
		// for(it=sop[i].begin(); it!=sop[i].end(); ++it){
		// 	for(int i=0; i<variable; i++){
		// 		//count 1 and 0 in sop
		// 		if((*it)[i]!='-')
		// 			literal++;
		// 	}
		// }
		bkLiteral.push_back(literal);
	}
	literal = *min_element(bkLiteral.begin(), bkLiteral.end());

	//select one has minimum # of prime implicant, minimum # of lterals
	idx = min_element(bkLiteral.begin(), bkLiteral.end())-bkLiteral.begin();
}

//pos to sop
void PetrickMethod::PosToSop(set<string> product, int npos, int nMax){
	int posSize = pos[npos].size();
	if(npos==nMax){
		string pString = toString(product);
		if(sopString.find(pString)==sopString.end()){
			sop.push_back(product);
			sopString.insert(pString);
		}
	}
	for(int i=0; i<posSize; i++){
		if(product.find(pos[npos][i])==product.end()){
			product.insert(pos[npos][i]);
			PosToSop(product, npos+1, nMax);
			product.erase(pos[npos][i]);
		}
		else
			PosToSop(product, npos+1, nMax);
	}
}

void PetrickMethod::printMinCover(){
	set<string>::iterator it;
	set<string>::iterator beg = sop[idx].begin();
	set<string>::iterator end = sop[idx].end();
	int sopSize = sop[idx].size();
	cout << "\n.mc " << sopSize << endl;
  	for(it = beg; it!= end; ++it)
    	cout << *it << endl;
  	cout << "literal=" << literal << endl; 
}

void PetrickMethod::printMinCover(ofstream &fout){
	set<string>::iterator it;
	set<string>::iterator beg = sop[idx].begin();
	set<string>::iterator end = sop[idx].end();
	int sopSize = sop[idx].size();
	fout << "\n.mc " << sopSize << endl;
  	for(it = beg; it!= end; ++it)
    	fout << *it << endl;
  	fout << "literal=" << literal; 
}

//----------------------------------------------------
// main
//----------------------------------------------------

void Init(ifstream &fin, QuineMcCluskey &QM);
void Output(ofstream &fout, QuineMcCluskey &QM, PetrickMethod &PM);

int main(int argc, char *argv[])
{
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
	//cout << "here" <<endl;
	QuineMcCluskey QM;
	Init(fin, QM);
	//cout << QM.onSet.size() <<endl;
	int onSize = QM.onSet.size();
	//cout << onSize <<endl;
	QM.getPrime();
	//cout << "here" <<endl;
	//QM.printPrime();
	//cout << "here" <<endl;

	PetrickMethod PM;
	PM.variable = QM.variable;
	PM.Pfunction(QM.implicant, QM.primeImplicant, onSize);
	//PM.printMinCover();

	Output(fout, QM, PM);
	fin.close();
	fout.close();
	return 0;
}

void Init(ifstream &fin, QuineMcCluskey &QM) {
	string s;
	fin >> s;
	if(s==".i"){
		fin >> s;
		QM.variable = atoi(s.c_str());
	}
	fin >> s;
	if(s==".m") {
		while(fin >> s){
			if(s!=".d")
				QM.onSet.push_back(atoi(s.c_str()));
			else 
				break;
		}		
	}
	//int onSize = QM.onSet.size();
	if(s==".d") {
		while(fin >> s) 
			QM.dcSet.push_back(atoi(s.c_str()));
	}
}

void Output(ofstream &fout, QuineMcCluskey &QM, PetrickMethod &PM){
	//QM.printPrime();
	QM.printPrime(fout);
	//PM.printMinCover();
	PM.printMinCover(fout);
	//cout<< endl;
}