#include "main.h"


int main(int argc, char* argv[])
{
	std::string file_name;
	std::string line;

	std::vector<std::string> column_headers;
	std::vector<std::string> row_headers;
	std::vector<std::shared_ptr<Cell>>calculated_cell;

	std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
	std::unordered_map<int, std::shared_ptr<Cell>> cell_by_index;

	if(!GetFileName(argv,file_name))
	{
		std::cout << "Please try again and input correct filename";
		return -1;	
	}

	std::fstream file;
	file.open(file_name,std::ios::in);

	getline(file,line);
	std::string single_item;
	
	while(GetNextItemInRow(line,single_item))
	{
		column_headers.emplace_back(single_item);
		EraseFirstItemFromString(line);
	}

	if(column_headers.size() == 0)
	{
		std::cout << "First row doesn't containt column Name";
		return -2;
	}

	int row_counter = 0;
	int column_counter = 0;

	while(getline(file,line))
	{
		column_counter = 0;
		++row_counter;

		if(GetNextItemInRow(line,single_item) && single_item != "")
		{
			row_headers.emplace_back(single_item);
			EraseFirstItemFromString(line);
		}
		else
		{
			std::cout << "Line \t" << row_counter << " will be skipped because it does not contain the name of the line" << std::endl; 
			continue;
		}

		while(GetNextItemInRow(line,single_item))
		{
			AddCell(line,cell_by_name,cell_by_index,column_headers,row_headers,column_counter,row_counter,single_item,calculated_cell);
		}
	}

	if(!CalculateValues(calculated_cell,cell_by_name))
	{
		std::cout << "One or more cells cannot be calculated. The value has been replaced with inf" << std::endl;
	}

	PrintValues(row_headers, column_headers, cell_by_index);
	std::cout << std::endl;
	return 0;
}