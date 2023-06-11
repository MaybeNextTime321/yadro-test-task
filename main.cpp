#include "iostream"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

const std::string ITEMDELIMITER = ",";

struct Cell
{
	std::string current_cell_value;
	std::string cell_name;
	float result;
	bool result_is_ready = false;
};

bool StringIsNumber(std::string string)
{
	bool is_number = true;

	for (int i = 0; i < string.length(); ++i) 
	{
		if (!std::isdigit(string[i])) 
		{
			is_number = false;
	  		break;
		}
	}
	
	if(string.length() == 0)
		return false;

	return is_number;
}

bool GetFileName(char * argv[], std::string & file_name)
{
	if(argv[1])
	{
		file_name = argv[1];
		return true;
	}
	else
		return false;
}

bool GetItemPositionInString(const std::string & string, std::size_t & position)
{
	position = string.find(ITEMDELIMITER);

	return (position == std::string::npos) ? false : true;
}

bool RemoveFirstItemFromString(std::string & string)
{
	std::size_t position = 0;
	if(GetItemPositionInString(string,position))
	{
		string.erase(0, position + ITEMDELIMITER.length());
		return true;
	}
	else
		return false;
}

bool GetNextItemInRow(std::string & row, std::string & result)
{
		if(row == "")
			return false;

		std::size_t position = 0;
		if(GetItemPositionInString(row,position))
		{
			result = row.substr(0,position);
			return true;
		}
		else 
		{
			result = row;
			row = "";
			return true;
		}
}

int GetCellNumber(int column_size, int row_number, int column_number)
{
	return column_size * row_number + column_number;
}

bool PrintValues(const std::vector<std::string>& row_name,const std::vector<std::string>& column_name, std::unordered_map<int, std::shared_ptr<Cell>> cell_by_index)
{

	for(std::string_view column : column_name)
	{
		std::cout << column << "\t";
	}

	for(int i=0; i < row_name.size(); ++i)
	{
		std::cout << std::endl <<row_name[i] << "\t";
		for(int j=1;j<column_name.size();++j)
		{
			std::unordered_map<int,std::shared_ptr<Cell>>::const_iterator find_result = cell_by_index.find(GetCellNumber(column_name.size(),i,j));
			
			if ( find_result == cell_by_index.end())
				std::cout << "\t";
			else
			{
				if (find_result->second->result_is_ready)
					std::cout <<  find_result->second->result << "\t";
				else 
					std::cout << find_result->second->current_cell_value << "\t";
			}
		}
	}

	return true;
}

void AddCell(std::string& row, std::unordered_map<std::string, std::shared_ptr<Cell>>& cell_by_Name, std::unordered_map<int, std::shared_ptr<Cell>>& cell_by_index, 
								const std::vector<std::string>&column_name,const std::vector<std::string>&row_name,int& column_counter,const int& row_counter,const std::string& value, 
								std::vector<std::shared_ptr<Cell>>&calculated_cell) 
{
	++column_counter;

	std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();

	new_cell->current_cell_value = value;
	new_cell->cell_name = column_name[column_counter] + row_name[row_counter - 1]; 

	std::pair<std::string,std::shared_ptr<Cell>> byNamePair(column_name[column_counter] + row_name[row_counter - 1],new_cell);
	std::pair<int,std::shared_ptr<Cell>> byNumberPair(GetCellNumber(column_name.size(),(row_counter - 1),column_counter),new_cell);

	cell_by_Name.insert(byNamePair);
	cell_by_index.insert(byNumberPair);

	if(StringIsNumber(value))
	{
		new_cell->result = std::stoi(value);
		new_cell->result_is_ready = true;
	}
	else
	{
		calculated_cell.push_back(new_cell);
	}

	RemoveFirstItemFromString(row);

}

bool StringContainAnyOperation(std::string_view value)
{
	return value.find('+') != std::string::npos || value.find('-') != std::string::npos || value.find('*') != std::string::npos || value.find('/') != std::string::npos;
}

void ReplaceValueToInfinite(std::shared_ptr<Cell>& value, std::string error_message, bool display_error=true)
{	
	if(display_error)
	{
		std::cout<< error_message;
		std::cout << "\tValue replased to \"inf\"" << std::endl;
	}

	value->current_cell_value = "inf";
}

bool CalculateSingleValues(std::shared_ptr<Cell> value, const std::unordered_map<std::string, std::shared_ptr<Cell>>& cell_by_name, std::unordered_set<std::shared_ptr<Cell>>& reference_list);

bool ArgumentToResult(const std::string& text_view, const std::unordered_map<std::string, std::shared_ptr<Cell>>& cell_by_name, int &result, std::unordered_set<std::shared_ptr<Cell>>& reference_list)
{
	std::unordered_map<std::string,std::shared_ptr<Cell>>::const_iterator cell = cell_by_name.find(text_view);
	if ( cell == cell_by_name.end() && !StringIsNumber(text_view))
	{
		std::cout<< "Can't find element: " << text_view << std::endl;  
		return false;
	}

	if(StringIsNumber(text_view))
	{
		result = stoi(text_view);
		return true;
	}
	else
	{
		if(cell->second->result_is_ready)
		{
			result = cell->second->result;
			return true;
		}
		else 
		{
			//std::cout << "Start calculate new cell: " << cell->second->current_cell_value << std::endl;
			if(CalculateSingleValues(cell->second,cell_by_name, reference_list))
			{
				result = cell->second->result;
				return true;
			}
			else
				return false;

		}
	}
}

bool CalculateSingleValues(std::shared_ptr<Cell> value, const std::unordered_map<std::string, std::shared_ptr<Cell>>& cell_by_name, std::unordered_set<std::shared_ptr<Cell>>& reference_list)
{
	
	std::string text_view = value->current_cell_value;

	if(text_view[0] != '=')
	{
		ReplaceValueToInfinite(value, "Incorrect input. Cell: " + value->cell_name + "\tValue: " + text_view, true);
		return false;
	}

	if(reference_list.find(value) != reference_list.end())
	{

		ReplaceValueToInfinite(value,"Cell error: Circular Dependency Detected in Cell: " + value->cell_name, true);
		return false;
	}
	else
		reference_list.insert(value);


	text_view.erase(0, 1);

	if(!StringContainAnyOperation(text_view))
	{
		ReplaceValueToInfinite(value, "Cell doesn't containt any operator"); 
		return false;
	}

	char operation;
	
	if(text_view.find('+') != std::string::npos)
		operation = '+';

	if(text_view.find('-') != std::string::npos)
		operation = '-';
	
	if(text_view.find('*') != std::string::npos)
		operation = '*';

	if(text_view.find('/') != std::string::npos)
		operation = '/';

	size_t position = text_view.find(operation);

	std::string first_cell_name = text_view.substr(0, position);
	std::string second_cell_name = text_view.substr(position+1,text_view.length());
	int first_cell_number;
	int second_cell_number;


	if(!ArgumentToResult(first_cell_name,cell_by_name,first_cell_number,reference_list))
	{
		if(reference_list.find(value) == reference_list.end())
			ReplaceValueToInfinite(value,"Can't get first argument " + first_cell_name,true);
		else
			ReplaceValueToInfinite(value,"",false);
		return false;
	}

	if(!ArgumentToResult(second_cell_name,cell_by_name,second_cell_number,reference_list))
	{
		if(reference_list.find(value) == reference_list.end())
			ReplaceValueToInfinite(value,"Can't get second argument " + second_cell_name,true);
		else
			ReplaceValueToInfinite(value,"",false);
		return false;
	}

	if(operation == '+')
	{
		value->result = first_cell_number + second_cell_number;
		value->result_is_ready = true;
	}

	if(operation == '-')
	{
		value->result = first_cell_number - second_cell_number;
		value->result_is_ready = true;
	}

	if(operation == '*')
	{
		value->result = first_cell_number * second_cell_number;
		value->result_is_ready = true;
	}

	if(operation == '/')
	{
		if(second_cell_number == 0)
		{
			ReplaceValueToInfinite(value,"Division by 0 is not possible",true);
			return false;
		}

		value->result = (float)first_cell_number / second_cell_number;
		value->result_is_ready = true;
	}

	return true;
}

bool CalculateValues(std::vector<std::shared_ptr<Cell>>calculated_cell, std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name)
{
	bool all_values_calculated = true;

	std::unordered_set<std::shared_ptr<Cell>>reference_list;

	for(std::shared_ptr<Cell> single_cell: calculated_cell)
	{
		reference_list.clear();
		if(!CalculateSingleValues(single_cell,cell_by_name, reference_list))
			all_values_calculated = false;
	}

	return all_values_calculated;
}

int main(int argc, char* argv[])
{
	std::string file_name;
	std::string row;

	std::vector<std::string> column_name;
	std::vector<std::string> row_name;
	std::vector<std::shared_ptr<Cell>>calculated_cell;

	std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
	std::unordered_map<int, std::shared_ptr<Cell>> cell_by_index;

	if(!GetFileName(argv,file_name))
	{
		std::cout << "Please try again and input filename";
		return -1;	
	}

	std::fstream file;
	file.open(file_name,std::ios::in);

	getline(file,row);
	std::string single_item;
	
	while(GetNextItemInRow(row,single_item))
	{
		column_name.emplace_back(single_item);
		RemoveFirstItemFromString(row);
	}

	if(column_name.size() == 0)
	{
		std::cout << "First row doesn't containt column Name";
		return -2;
	}

	int row_counter = 0;
	int column_counter = 0;

	while(getline(file,row))
	{
		column_counter = 0;
		++row_counter;


		if(GetNextItemInRow(row,single_item))
		{
			row_name.emplace_back(single_item);
			RemoveFirstItemFromString(row);
		}
		else
			break;

		while(GetNextItemInRow(row,single_item))
		{
			AddCell(row,cell_by_name,cell_by_index,column_name,row_name,column_counter,row_counter,single_item,calculated_cell);
		}
	}

	if(!CalculateValues(calculated_cell,cell_by_name))
	{
		std::cout << "One or more cells cannot be calculated. The value has been replaced with inf" << std::endl;
	}

	PrintValues(row_name, column_name, cell_by_index);
	std::cout << std::endl;
	return 0;
}