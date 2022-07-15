/*@Author
Student Name: Ufuk Yarisan
Student ID : 070170206
Date: 24.11.2021
*/


int COLUMN_COUNT = 6;
int STARTING_INDEX = 0;

#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include <time.h>
#include <stdlib.h>


using namespace std;

//Tweets will be rows and rows will be represented as a struct.
struct Row{
	string tweet_id;
	string date_time;
	string unix_time;
	string artist;
	string track;
	string country;
}; 


// To read csv file into table. Rows is going to be handled in a vector.
void read_rows(vector<Row> &table, const int &N, const string &I);

// To write to table into a csv file. They are seperated by using '\t'.
void write_rows(vector<Row> &table, const int &N, const string &O);

//A is the mode of the quicksort, p is the starting index, r is the last index.
void quick_sort(vector<Row> &table, const int A, const int &p, const int &r);

//Pivot the last index.
int partition(vector<Row> &table, const int &p, const int &r);

//Chose a random index and pivot it by using partition function.
int random_partition(vector<Row> &table, const int &p, const int &r);

int main(int argc, char **argv){

	//Check the arguments of the main function.
	if(argc != 5){
		exit(1);
	}

	int N = stoi(argv[1]); // second argument, number of the tweets to be sorted
	int A; // third argument, mode of the quicksort
	if(!strcmp(argv[2], "randomized")){
		A = 0;
	}
	else if (!strcmp(argv[2], "deterministic")){
		A = 1;
	}
	else{
		cerr << "In valid quick sort mode is entered." << endl;
		exit(1);
	}

	string I = argv[3]; //input file path
	string O = argv[4]; //output filepath
	
	vector<Row> table;
	read_rows(table, N, I);

	// Sorting time is going to be calculated.
	clock_t t;
	int f;
	t = clock();
 	quick_sort(table, A, STARTING_INDEX, N-1); //0 burada başlangıç indisi 
	t = clock() - t;
	cout << "Sorted in " << ((float)t) * 1000 /CLOCKS_PER_SEC << " miliseconds." << endl;

	write_rows(table, N, O);
	return EXIT_SUCCESS;
}


void read_rows(vector<Row> &table, const int &N, const string &I){
	fstream fp;
	fp.open(I, ios::in | ios::out);
	if(!fp.is_open()){
		cerr << "File couldn't be open." << endl;
		exit(1);
	}

	string line, column;

	getline(fp, line); // in order to skip headers

	for(int i=0; i < N; i++){ //could be defined as a new function
		
		getline(fp, line);
		stringstream s(line);
		Row element;
		getline(s, column, '\t');
		element.tweet_id = column;
		getline(s, column, '\t');
		element.date_time = column;
		getline(s, column, '\t');
		element.unix_time = column;
		getline(s, column, '\t');
		element.artist = column;
		getline(s, column, '\t');
		element.track = column;
		getline(s, column, '\t');
		element.country = column;
		
		table.push_back(element);

	}
	fp.close();
}

void write_rows(vector<Row> &table, const int &N, const string &O){
	fstream fp;
	fp.open(O, ios::out | ios:: app);

	fp << "tweet_id\ttweet_datetime\ttweet_unixtime\tartist_name\ttrack_title\tcountry\n"; //in order to write the headers

	for(int i = 0; i < N; i++){ //could be written as a seperate function
		fp << table[i].tweet_id << "\t" <<
		table[i].date_time << "\t" <<
		table[i].unix_time << "\t" <<
		table[i].artist << "\t" <<
		table[i].track << "\t" <<
		table[i].country << "\n";
	}
	fp.close();
}


int random_partition(vector<Row> &table, const int &p, const int &r){
	int i = rand() % (r-p+1) + p; //picks a random integer between p and r, boundaries are included
	iter_swap(table.begin()+r, table.begin()+i);
	return partition(table, p, r);
}


int partition(vector<Row> &table, const int &p, const int &r){
	
	string pivot = table[r].country; //picks a pivot
	transform(pivot.begin(), pivot.end(), pivot.begin(), ::tolower); //to make it case insensitive
	
	int i = p - 1;

	for(int j = p; j < r; j++){
		string temp = table[j].country; //picks a temporary string
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower); //to make it case insensitive
		if(temp < pivot){
			i++;
			iter_swap(table.begin()+i, table.begin()+j);
		}
		//if countries are same then test the artist name
		else if(temp == pivot){
			string pivot = table[r].artist; //same pivot but different feature
			transform(pivot.begin(), pivot.end(), pivot.begin(), ::tolower); //to make it case insensitive
			
			string temp = table[j].artist; //same temporary variable but different feature
			transform(temp.begin(), temp.end(), temp.begin(), ::tolower); //to make it case insensitive
			if(temp < pivot){
				i++;
				iter_swap(table.begin()+i, table.begin()+j);
			}
			//if artists are same the test the unix date
			else if(temp == pivot){
				string pivot = table[r].unix_time;

				string temp = table[j].unix_time;
				if (temp <= pivot){
					i++;
					iter_swap(table.begin()+i, table.begin()+j);
				}
			}
		}
	}
	// after all of these change pivot location
	iter_swap(table.begin()+i+1, table.begin()+r);
	return i + 1;
};

void quick_sort(vector<Row> &table, const int A, const int &p, const int &r){
	if(p < r){
		int q;
		if(A){
			q = partition(table, p, r);	//deterministic case
		}
		else{
			q = random_partition(table, p, r); // randomized case
		}
		quick_sort(table, A, p, q-1);
		quick_sort(table, A, q+1, r);
	}
}