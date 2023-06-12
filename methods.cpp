#include "main.h"
bool StringIsNumber(std::string string)
{
	bool is_number = true;
	int i = 0;

	if(string == "")
	{
		return false;
	}

	if(string[0] == '-')
	{
		i = 1;
	}

	for (; i < string.length(); ++i) 
	{
		if (!std::isdigit(string[i])) 
		{
			is_number = false;
	  		break;
		}
	}
	
	if(string.length() == 0)
	{
		return false;
	}

	return is_number;
}

bool GetFileName(char * argv[], std::string & file_name)
{
	if(argv[1])
	{
		file_name = argv[1];
		if(file_name.length() < 5 || file_name.substr(file_name.length()-4) != ".csv")
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool GetDelimiterPositionInString(const std::string & string, std::size_t & position)
{
	position = string.find(ITEMDELIMITER);
	return (position == std::string::npos) ? false : true;
}

bool EraseFirstItemFromString(std::string & string)
{
	std::size_t position = 0;
	if(GetDelimiterPositionInString(string,position))
	{
		string.erase(0, position + ITEMDELIMITER.length());
		return true;
	}
	else
	{
		return false;
	}
}

bool GetNextItemInRow(std::string & line, std::string & result)
{
		if(line == "")
		{
			return false;
		}

		std::size_t position = 0;
		if(GetDelimiterPositionInString(line,position))
		{
			result = line.substr(0,position);
			return true;
		}
		else 
		{
			result = line;
			line = "";
			return true;
		}
}

int GetCellNumber(int column_size, int row_number, int column_number)
{
	return column_size * row_number + column_number;
}

void PrintValues(const std::vector<std::string>& row_headers,const std::vector<std::string>& column_headers, std::unordered_map<int, std::shared_ptr<Cell>> cell_by_index)
{

	for(std::string_view column : column_headers)
	{
		std::cout << column << "\t";
	}

	for(int i=0; i < row_headers.size(); ++i)
	{
		std::cout << std::endl <<row_headers[i] << "\t";
		for(int j=1;j<column_headers.size();++j)
		{
			std::unordered_map<int,std::shared_ptr<Cell>>::const_iterator find_result = cell_by_index.find(GetCellNumber(column_headers.size(),i,j));
			
			if (find_result == cell_by_index.end())
			{
				std::cout << "\t";
			}
			else
			{
				if (find_result->second->result_is_ready)
				{
					std::cout <<  find_result->second->result << "\t";
				}
				else 
				{
					std::cout << find_result->second->current_cell_value << "\t";
				}
			}
		}
	}

}

void AddCell(std::string& line, std::unordered_map<std::string, std::shared_ptr<Cell>>& cell_by_Name, std::unordered_map<int, std::shared_ptr<Cell>>& cell_by_index, 
								const std::vector<std::string>&column_headers,const std::vector<std::string>&row_headers,int& column_counter,const int& row_counter,const std::string& value, 
								std::vector<std::shared_ptr<Cell>>&calculated_cell) 
{
	++column_counter;



	if(column_counter >= column_headers.size() || row_counter-1 >= row_headers.size() )
	{
		if(row_counter != 1 )
		{
			std::cout << "Value \t" << value << " [" << row_counter-1 << "][" << column_counter << "] will be skipped because column without name" << std::endl; 	
		}		
	}
	else
	{
		std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();

		new_cell->current_cell_value = value;

		new_cell->cell_name = column_headers[column_counter] + row_headers[row_counter - 1]; 

		std::pair<std::string,std::shared_ptr<Cell>> byNamePair(column_headers[column_counter] + row_headers[row_counter - 1],new_cell);
		std::pair<int,std::shared_ptr<Cell>> byNumberPair(GetCellNumber(column_headers.size(),(row_counter - 1),column_counter),new_cell);

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
	}


	EraseFirstItemFromString(line);

}

bool StringContainAnyOperation(std::string_view value)
{
	return value.find('+') != std::string::npos || value.find('-') != std::string::npos || 
	value.find('*') != std::string::npos || value.find('/') != std::string::npos;
}

void ReplaceValueToInfinite(std::shared_ptr<Cell>& value, std::string error_message, bool display_error)
{	
	if(value->current_cell_value == "")
	{
		return;
	}	

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
			if(CalculateSingleValues(cell->second,cell_by_name, reference_list))
			{
				result = cell->second->result;
				return true;
			}
			else
			{
				return false;
			}

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
	{
		operation = '+';
	}

	if(text_view.find('-') != std::string::npos)
	{
		operation = '-';
	}
	
	if(text_view.find('*') != std::string::npos)
	{
		operation = '*';
	}

	if(text_view.find('/') != std::string::npos)
	{
		operation = '/';
	}

	size_t position = text_view.find(operation);

	std::string first_cell_name = text_view.substr(0, position);
	std::string second_cell_name = text_view.substr(position+1,text_view.length());
	int first_cell_number;
	int second_cell_number;

	if(!ArgumentToResult(first_cell_name,cell_by_name,first_cell_number,reference_list))
	{
		if(reference_list.find(value) == reference_list.end())
		{
			ReplaceValueToInfinite(value,"Can't get first argument " + first_cell_name,true);
			
		}
		else
		{
			ReplaceValueToInfinite(value,"",false);
		}
		return false;
		
	}

	if(!ArgumentToResult(second_cell_name,cell_by_name,second_cell_number,reference_list))
	{
		if(reference_list.find(value) == reference_list.end())
		{
			ReplaceValueToInfinite(value,"Can't get second argument " + second_cell_name,true);
		}
		else
		{
			ReplaceValueToInfinite(value,"",false);
		}
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
		if(!CalculateSingleValues(single_cell,cell_by_name, reference_list) && single_cell->current_cell_value != "")
		{
			all_values_calculated = false;
		}
	}

	return all_values_calculated;
}
