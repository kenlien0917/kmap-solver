#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
using namespace std;

//Initialization
void initialize(char**& kmap, int numVar, int* rows, int* cols)
{
	//switch diff. cases
	switch (numVar)
	{
	case 2:
		kmap = new char* [2];
		for (int i = 0; i < 2; i++) 
		{
			kmap[i] = new char[2];
		}
		*rows = 2; *cols = 2;
		break;
	case 3:
		kmap = new char* [2];
		for (int i = 0; i < 2; i++) 
		{
			kmap[i] = new char[4];
		}
		*rows = 2; *cols = 4;
		break;
	case 4:
		kmap = new char* [4];
		for (int i = 0; i < 4; i++)
		{
			kmap[i] = new char[4];
		}
		*rows = 4; *cols = 4;
		break;
	}
}

//Set_terms
void set_terms(const int numVar, char**& kmap, const char term, const vector<int> terms) 
{
	vector<pair<int, int>> Chart; // List the coordinates of K-map since it uses graycode style which makes it difficult to use for loop
	switch (numVar) 
	{
	case 2:
				  //row	x-coords
		Chart = { { 0, 0 }, { 1, 0 }, //col y-coords
				  { 0, 1 }, { 1, 1 } };
		for (size_t i : terms)
		{
			kmap[Chart[i].first][Chart[i].second] = term;
		}
		break;
	case 3:
				  //row	x-coords
		Chart = { { 0, 0 }, { 1, 0 }, //col y-coords
				  { 0, 1 }, { 1, 1 }, 
				  { 0, 3 }, { 1, 3 }, 
				  { 0, 2 }, { 1, 2 } };
		for (size_t i : terms)
		{
			kmap[Chart[i].first][Chart[i].second] = term;
		}
		break;
	case 4:
				  //row	x-coords
		Chart = { { 0, 0 }, { 1, 0 }, { 3, 0 }, { 2, 0 }, //col y-coords
				  { 0, 1 }, { 1, 1 }, { 3, 1 }, { 2, 1 },
				  { 0, 3 }, { 1, 3 }, { 3, 3 }, { 2, 3 },
				  { 0, 2 }, { 1, 2 }, { 3, 2 }, { 2, 2 } };
			
		for (size_t i : terms) 
		{
			kmap[Chart[i].first][Chart[i].second] = term;
		}
		break;
	}
}

// Change 1 (covered minterms) into 2 (dontcare)
void change_1(vector<vector<int>>& kmap, const vector<vector<pair<int, int>>>& primes)
{
	for (const auto& implicant : primes)
	{
		for (const auto& coordinates : implicant) 
		{
			if (kmap[coordinates.first][coordinates.second]) //Whether it is mintern
				kmap[coordinates.first][coordinates.second] = 2; //Change it into dontcare to prevent incorrect reuse
		}
	}
}

//<Key> Recognize Groups
int group_recognize(const vector<vector<int>>& kmap, int a, int b, int i, int j, vector<vector<pair<int, int>>>& primes) 
{
	int rows = kmap.size();
	int cols = kmap[0].size();
	int covered_minterm = 0; // record covered minterm
	bool dontcare = false; // check if there's dontcare

	// Check if group is valid
	for (int x = i; x < i + a; ++x) //a*b is the area of the group
	{
		for (int y = j; y < j + b; ++y) 
		{
			int check = kmap[x % rows][y % cols];
			if (check == 0)
			{
				return 0; // including 0, return false
			}
			else if (check == 1)
			{
				covered_minterm++; //record 1s
			}
			else if (check == 2) {
				dontcare = true; // record 2s
			}
		}
	}

	// If group includes no 1s
	if (covered_minterm == 0)
	{
		return 0;
	}

	// record the coordinates of valid group
	vector<pair<int, int>> coordinates;
	for (int x = i; x < i + a; ++x)
	{
		for (int y = j; y < j + b; ++y) 
		{
			pair<int, int> coordinate = { x % rows, y % cols }; // % rows or cols make sure that crossed-border groups can be record
			coordinates.push_back(coordinate);
		}
	}

	// sort and return coordinates
	sort(coordinates.begin(), coordinates.end());
	primes.push_back(coordinates);

	return covered_minterm; 
}

//Show implicants
string show_answer(int numVar, const vector<pair<int, int>> coordinates) {
	map<pair<int, int>, vector<int>> answer; // answer includes coords and minterm
	vector<vector<int>> terms;
	string result;

	//Let coordinates represent the binary code
	switch (numVar)
	{
	case 2:
		answer = 
		{
			//{{coords}, {binary code}}
			{{0, 0}, {0, 0}},
			{{1, 0}, {0, 1}}, 
			{{0, 1}, {1, 0}}, 
			{{1, 1}, {1, 1}} 
		};
		for (pair<int, int> term : coordinates)
		{
			terms.push_back(answer[term]);
		}
		break;

	case 3:
		answer =
		{
			{{0, 0}, {0, 0, 0}}, 
			{{1, 0}, {0, 0, 1}}, 
			{{0, 1}, {0, 1, 0}}, 
			{{1, 1}, {0, 1, 1}}, 
			{{0, 2}, {1, 1, 0}}, 
			{{1, 2}, {1, 1, 1}}, 
			{{0, 3}, {1, 0, 0}}, 
			{{1, 3}, {1, 0, 1}}  
		};
		for (pair<int, int> term : coordinates)
		{
			terms.push_back(answer[term]);
		}
		break;

	case 4:
		answer =
		{
			{{0, 0}, {0, 0, 0, 0}}, 
			{{1, 0}, {0, 0, 0, 1}}, 
			{{2, 0}, {0, 0, 1, 1}}, 
			{{3, 0}, {0, 0, 1, 0}}, 
			{{0, 1}, {0, 1, 0, 0}}, 
			{{1, 1}, {0, 1, 0, 1}}, 
			{{2, 1}, {0, 1, 1, 1}}, 
			{{3, 1}, {0, 1, 1, 0}}, 
			{{0, 2}, {1, 1, 0, 0}}, 
			{{1, 2}, {1, 1, 0, 1}}, 
			{{2, 2}, {1, 1, 1, 1}}, 
			{{3, 2}, {1, 1, 1, 0}}, 
			{{0, 3}, {1, 0, 0, 0}}, 
			{{1, 3}, {1, 0, 0, 1}}, 
			{{2, 3}, {1, 0, 1, 1}}, 
			{{3, 3}, {1, 0, 1, 0}}  
		};
		for (pair<int, int> term : coordinates) 
		{
			terms.push_back(answer[term]);
		}
		break;
	}

	while (terms.size() > 1)
	{
		// Compare two implicants to simplify
		vector<int>& implicant1 = terms[0];
		vector<int>& implicant2 = terms[1];

		// Create a new implicat
		vector<int> new_implicant;
		for (size_t i = 0; i < implicant1.size(); ++i) 
		{
			if (implicant1[i] == implicant2[i])
			{
				new_implicant.push_back(implicant1[i]);
			}
			else 
			{
				new_implicant.push_back(-1); // -1 means the variable is removed
			}
		}

		// Erase two implicants and insert the new implicant
		terms.erase(terms.begin(), terms.begin() + 2);
		terms.insert(terms.begin(), new_implicant);
	}
	//Create implicants
	if (terms[0][0] == 1)
		result += "a";
	else if (terms[0][0] == 0)
		result += "a'";
	if (terms[0][1] == 1)
		result += "b";
	else if (terms[0][1] == 0)
		result += "b'";
	if (numVar >= 3) {
		if (terms[0][2] == 1)
			result += "c";
		else if (terms[0][2] == 0)
			result += "c'";
		if (numVar >= 4) {
			if (terms[0][3] == 1)
				result += "d";
			else if (terms[0][3] == 0)
				result += "d'";
		}
	}

	// all chart
	if (result.empty()) {
		result += "1";
	}

	return result;
}

//Check whether any minterm is left
bool is_left(const vector<vector<int>> &kmap) {
	bool Left = false;

	for (int i = 0; i < kmap.size(); i++)
	{
		for (int j = 0; j < kmap[i].size(); j++)
		{
			if (kmap[i][j] == 1)
			{
					Left = true; // there's a minterm left
			}
		}
	}
	
	return Left;
}

struct KmapSolution {
	int numVar;
	char **kmap;
	vector<string> primes;
	vector<string> essentials;
	vector<string> minimumSop;
};

KmapSolution *solveKmap(int numVar, const vector<int> minterms, const vector<int> dontcares) {
	KmapSolution *sol = new KmapSolution;

	// implement your solution here

	// Set kmap size
	sol->numVar = numVar;
	int rows, cols;
	vector<vector<int>> kmapp;
	
	initialize(sol->kmap, sol->numVar, &rows, &cols);

	// Setting 0s
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++)
		{
			sol->kmap[i][j] = '0';
		}
	}

	// Setting 1s
	set_terms(sol->numVar, sol->kmap, '1', minterms);

	// Setting Xs
	set_terms(sol->numVar, sol->kmap, 'x', dontcares);

	// fill the kmap into vector kmapp to do calculations
	vector<vector<int>> temp_kmap;
	for (int i = 0; i < rows; i++)
	{
		vector<int> row;
		for (int j = 0; j < cols; j++)
		{
			if (sol->kmap[i][j] == '0')// 0
				row.push_back(0);
			else if (sol->kmap[i][j] == '1')// 1
				row.push_back(1);
			else if (sol->kmap[i][j] == 'x')// dont care
				row.push_back(2);
		}
		kmapp.push_back(row);
	}
	temp_kmap = kmapp;

	// start to recoginize groups
	vector<vector<pair<int, int>>> primes; // record the coords of primes
	int x; int y; // area of thee group

	// 4*4 (only for numVar == 4)
	if (cols == 4 && rows == 4)
	{
		x = 4; y = 4;
		for (int i = 0; i < rows; i++) 
		{
			for (int j = 0; j < cols; j++) 
			{
				group_recognize(temp_kmap, x, y, i, j, primes);
			}
		}
		change_1(temp_kmap, primes);
	}

	//check if there is still minterm left
	if (is_left(temp_kmap))
	{
		// 2*4 ( only for numVar >= 3)
		if (rows >= 2 && cols == 4) 
		{
			x = 2; y = 4;
			for (int i = 0; i < rows; i++) 
			{
				for (int j = 0; j < cols; j++) 
				{
					group_recognize(temp_kmap, x, y, i, j, primes);
				}
			}
		}

		// 4*4 (only for numVar == 4)
		if (cols == 4 && rows == 4) 
		{
			// 4*2
			x = 4; y = 2;
			for (int i = 0; i < rows; i++) 
			{
				for (int j = 0; j < cols; j++) 
				{
					group_recognize(temp_kmap, x, y, i, j, primes);
				}
			}
		}
		change_1(temp_kmap, primes);
	}

	//check if there is still minterm left
	if (is_left(temp_kmap))
	{
		// 2*2
		x = 2; y = 2;
		for (int i = 0; i < rows; i++) 
		{
			for (int j = 0; j < cols; j++) 
			{
				group_recognize(temp_kmap, x, y, i, j, primes);
			}
		}
		// 1*4 (only for numVar >= 3)
		if (cols == 4) {
			
			x = 1; y = 4;
			for (int i = 0; i < rows; i++) 
			{
				for (int j = 0; j < cols; j++) 
				{
					group_recognize(temp_kmap, x, y, i, j, primes);
				}
			}
		}
		// 4*1 (only for numVar == 4)
		if (rows == 4) 
		{
			x = 4; y = 1;
			for (int i = 0; i < rows; i++) 
			{
				for (int j = 0; j < cols; j++) 
				{
					group_recognize(temp_kmap, x, y, i, j, primes);
				}
			}
		}

		change_1(temp_kmap, primes);
	}

	//check if there is still minterm left
	if (is_left(temp_kmap))
	{
		// 1*2
		x = 1; y = 2;
		for (int i = 0; i < rows; i++) 
		{
			for (int j = 0; j < cols; j++) 
			{
				group_recognize(temp_kmap, x, y, i, j, primes);
			}
		}

		// 2*1
		x = 2; y = 1;
		for (int i = 0; i < rows; i++) 
		{
			for (int j = 0; j < cols; j++) 
			{
				group_recognize(temp_kmap, x, y, i, j, primes);
			}
		}
		change_1(temp_kmap, primes);
	}

	//check if there is still minterm left
	if (is_left(temp_kmap)) 
	{
		// 1*1
		x = 1; y = 1;
		for (int i = 0; i < rows; i++) 
		{
			for (int j = 0; j < cols; j++) 
			{
				group_recognize(temp_kmap, x, y, i, j, primes);
			}
		}
	}

	// erase repititive implicants
	sort(primes.begin(), primes.end());
	primes.erase(unique(primes.begin(), primes.end()), primes.end());

	// essentials

	map<pair<int, int>, int> minterm_count;
	// Go through primes check each implicant
	for (const vector<pair<int, int>>& implicant : primes) {
		// Go through implicant check each coordinates
		for (const pair<int, int>& coordinates : implicant) {
			//count how many times one minterm gets covered
			minterm_count[coordinates]++;
		}
	}

	vector<vector<pair<int, int>>> essential_primes;
	vector<vector<pair<int, int>>> SOPs;

	// find all essential prime implicants
	for (int i = 0; i < primes.size(); i++) {
		bool is_essential = false;
		for (int j = 0; j < primes[i].size(); j++) {
			if (minterm_count[primes[i][j]] == 1
				&& kmapp[primes[i][j].first][primes[i][j].second] == 1) {
				is_essential = true;
			}
		}
		if (is_essential) {
			essential_primes.push_back(primes[i]);
			SOPs.push_back(primes[i]); // SOP must include essentials primes
		}
	}

	change_1(kmapp, SOPs);

	while (is_left(kmapp)) //check if there're still some minterms left
	{
		vector<pair<int, int>> temp_SOP;
		int temp_SOP_minterm = 0;
		int temp_size = 0;
		for (size_t i = 0; i < primes.size(); i++)
		{
			int count_minterm = 0;
			for (size_t j = 0; j < primes[i].size(); j++)
			{
				if (kmapp[primes[i][j].first][primes[i][j].second] == 1)
					count_minterm++;
			}
			if (count_minterm > temp_SOP_minterm || (primes[i].size() > temp_size && count_minterm == temp_SOP_minterm))
			{
				//add the new prime into SOP
				temp_SOP = primes[i];
				temp_SOP_minterm = count_minterm;
				temp_size = primes[i].size();
			}
		}
		SOPs.push_back(temp_SOP);
		change_1(kmapp, SOPs);
	}

	// output primes
	for (size_t i = 0; i < primes.size(); i++)
	{
		sol->primes.push_back(show_answer(numVar, primes[i]));
	}
	sort(sol->primes.begin(), sol->primes.end());
	sol->primes.erase(unique(sol->primes.begin(), sol->primes.end()), sol->primes.end());

	// output essential primes
	for (size_t i = 0; i < essential_primes.size(); i++) 
	{
		sol->essentials.push_back(show_answer(numVar, essential_primes[i]));
	}
	sort(sol->essentials.begin(), sol->essentials.end());
	sol->essentials.erase(unique(sol->essentials.begin(), sol->essentials.end()), sol->essentials.end());

	//ouput minimumSOP
	for (size_t i = 0; i < SOPs.size(); i++) 
	{
		sol->minimumSop.push_back(show_answer(numVar, SOPs[i]));
	}
	sort(sol->minimumSop.begin(), sol->minimumSop.end());
	sol->minimumSop.erase(unique(sol->minimumSop.begin(), sol->minimumSop.end()), sol->minimumSop.end());

	return sol;
}


/*void printKmapSolution(KmapSolution* sol)
{
	int nRow = 0, nCol = 0;
	switch (sol->numVar) {
	case 2:
		nRow = 2; nCol = 2;
		break;
	case 3:
		nRow = 2; nCol = 4;
		break;
	case 4:
		nRow = 4; nCol = 4;
		break;
	default:
		cerr << "Input Error" << endl;
	}

	cout << "K-map:\n";
	for (int i = 0; i < nRow; ++i) {
		for (int j = 0; j < nCol; ++j)
			cout << sol->kmap[i][j] << " ";
		cout << endl;
	}

	cout << "Prime implicants: ";
	for (int i = 0; i < sol->primes.size(); ++i) {
		cout << sol->primes[i];
		if (i != sol->primes.size() - 1) cout << ", ";
	}
	cout << endl;

	cout << "Essential prime implicants: ";
	for (int i = 0; i < sol->essentials.size(); ++i) {
		cout << sol->essentials[i];
		if (i != sol->essentials.size() - 1) cout << ", ";
	}
	cout << endl;

	cout << "Minimum SOP: ";
	for (int i = 0; i < sol->minimumSop.size(); ++i) {
		cout << sol->minimumSop[i];
		if (i != sol->minimumSop.size() - 1) cout << " + ";
	}
	cout << endl;
}

int main() {

	int numVar;
	vector <int> minterms;
	vector <int> dontcares;
	KmapSolution* sol;

	numVar = 4;
	minterms = { 0, 4, 5, 10, 11 };
	dontcares = { 1, 13, 14, 15 };

	sol = solveKmap(numVar, minterms, dontcares);
	printKmapSolution(sol);

	return 0;
}*/
