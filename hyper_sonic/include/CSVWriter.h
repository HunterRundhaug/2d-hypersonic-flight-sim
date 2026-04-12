#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


class CSVWriter {
private:
	std::ofstream my_file;
	std::string file_name;

public:
	CSVWriter(std::string file_name);

	void writeHeader(std::vector<std::string> values);
	void writeRow(std::vector<std::string> values);
	void closeFile() ;
};