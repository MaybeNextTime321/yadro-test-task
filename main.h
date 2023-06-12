#ifndef MYCODE_H
#define MYCODE_H

#include <iostream>
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

bool StringIsNumber(std::string string);
bool GetFileName(char *argv[], std::string &file_name);
bool GetDelimiterPositionInString(const std::string &string, std::size_t &position);
bool EraseFirstItemFromString(std::string &string);
bool GetNextItemInRow(std::string &line, std::string &result);
int GetCellNumber(int column_size, int row_number, int column_number);
void PrintValues(const std::vector<std::string> &row_headers, const std::vector<std::string> &column_headers, std::unordered_map<int, std::shared_ptr<Cell>> cell_by_index);
void AddCell(std::string &line, std::unordered_map<std::string, std::shared_ptr<Cell>> &cell_by_Name, std::unordered_map<int, std::shared_ptr<Cell>> &cell_by_index,
             const std::vector<std::string> &column_headers, const std::vector<std::string> &row_headers, int &column_counter, const int &row_counter, const std::string &value,
             std::vector<std::shared_ptr<Cell>> &calculated_cell);
bool StringContainAnyOperation(std::string_view value);
void ReplaceValueToInfinite(std::shared_ptr<Cell> &value, std::string error_message, bool display_error = true);
bool CalculateSingleValues(std::shared_ptr<Cell> value, const std::unordered_map<std::string, std::shared_ptr<Cell>> &cell_by_name, std::unordered_set<std::shared_ptr<Cell>> &reference_list);
bool ArgumentToResult(const std::string &text_view, const std::unordered_map<std::string, std::shared_ptr<Cell>> &cell_by_name, int &result, std::unordered_set<std::shared_ptr<Cell>> &reference_list);
bool CalculateSingleValues(std::shared_ptr<Cell> value, const std::unordered_map<std::string, std::shared_ptr<Cell>> &cell_by_name, std::unordered_set<std::shared_ptr<Cell>> &reference_list);
bool CalculateValues(std::vector<std::shared_ptr<Cell>> calculated_cell, std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name);
int main(int argc, char *argv[]);

#endif // MYCODE_H
