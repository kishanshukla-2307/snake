#include<bits/stdc++.h>
#include<unistd.h>
#include<boost/algorithm/string.hpp> 
#include<time.h>
#include<iostream>
#include<string>

using namespace std;

class CSVReader
{
	std::string fileName;
	std::string delimeter;
 
public:
	CSVReader(std::string filename, std::string delm = ",") :
			fileName(filename), delimeter(delm)
	{ }
 
	// Function to fetch data from a CSV File
	std::vector<std::vector<std::string> > getData();
};

std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);
 
	std::vector<std::vector<std::string> > dataList;
 
	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();
 
	return dataList;
}


int main()
{
	CSVReader reader("leaderboard.csv");
	std::vector<std::vector<std::string> > data = reader.getData();
	cout <<  "RANK" << "\t\tPlayer Name\t" << "\tHighest Score\n"; 
	for (int i = 0; i < data.size(); ++i)
	{
		cout << i+1 << "\t\t" << data[i][0] << "\t\t\t" << data[i][1] << "\n";
	}
	return 0;
}