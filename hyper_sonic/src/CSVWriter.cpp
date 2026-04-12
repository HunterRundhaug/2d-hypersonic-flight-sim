#include "../include/CSVWriter.h"


CSVWriter::CSVWriter(std::string file_name) :file_name{ file_name }{
	my_file.open(file_name);
}

void CSVWriter::writeHeader(std::vector<std::string> values) {
	for (int i = 0; i < values.size(); i++) {
		my_file << values[i];
		// only write a comma if this is not the last element
		if (i + 1 < values.size()) {
			my_file << ", ";
		}
	}
	my_file << "\n";
}

void CSVWriter::writeRow(std::vector<std::string> values) {
	for (int i = 0; i < values.size(); i++) {
		my_file << values[i];
		// only write a comma if this is not the last element
		if (i + 1 < values.size()) {
			my_file << ", ";
		}
	}
	my_file << "\n";
}

void CSVWriter::closeFile(){
	my_file.close();
}