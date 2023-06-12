#include <gtest/gtest.h>
#include "./main.h"
#include<array> 

TEST(StringIsNumberTests, PositiveNumbers) 
{
    std::string test_values[] = {"123","312","1","999"};
    bool test_result = true;

    for(std::string single_string:test_values)
        if(!StringIsNumber(single_string))
        {
            test_result = false;
        }

    ASSERT_TRUE(test_result); 
}

TEST(StringIsNumberTests, NegativeNumbers) 
{
    std::string test_values[] = {"-123","-312","-1","-999"};
    bool test_result = true;

    for(std::string single_string:test_values)
        if(!StringIsNumber(single_string))
        {
            test_result = false;
        }
        
    ASSERT_TRUE(test_result); 
}

TEST(StringIsNumberTests, ZeroNumbers) 
{
    bool test_result = StringIsNumber("0");
    ASSERT_TRUE(test_result); 
}

TEST(StringIsNumberTests, NegativeTest) 
{
    std::string test_values[] = {""," ","0 1 2","0 ", "qwew"};
    bool test_result = true;

    for(std::string single_string:test_values)
        if(StringIsNumber(single_string))
        {
            test_result = false;
        }
        
    ASSERT_TRUE(test_result); 
}

TEST(GetNextItemInRow, PositiveTest) 
{
    bool test_result = true;
    std::string test_results_array[] = {"12","14","1","13"};
    std::string test_input[] = {"12,13","14,51","1,123,123,123,123","13,321,123"};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        std::string method_result;

        if(!GetNextItemInRow(test_input[i],method_result) || method_result != test_results_array[i])
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(GetNextItemInRow, EmptyInput) 
{
    bool test_result = false;
    std::string method_result,test_input;
    test_input = "";
    if(!GetNextItemInRow(test_input,method_result))
    {
        test_result = true;    
    }
    ASSERT_TRUE(test_result); 
}

TEST(GetDelimiterPositionInString, PositiveTest) 
{
    bool test_result = true;
    size_t test_results_array[] = {2,2,1,4};
    std::string test_input[] = {"12,13","14,51","1,123,123,123,123","1323,321,123"};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        size_t method_result;

        if(!GetDelimiterPositionInString(test_input[i],method_result) || method_result != test_results_array[i])
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(GetDelimiterPositionInString, NegativeTest) 
{
    bool test_result = true;
    std::string test_input[] = {"12 13","1451","  ","asdasdasd;321`123"};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        size_t method_result;

        if(GetDelimiterPositionInString(test_input[i],method_result))
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(GetDelimiterPositionInString, EmptyInput) 
{
    bool test_result = true;
    std::string test_input = "";
    size_t method_result;
    ASSERT_TRUE(!GetDelimiterPositionInString(test_input,method_result)); 
}

TEST(EraseFirstItemFromString, PositiveTest) 
{
    bool test_result = true;
    std::string test_input[] = {"12,13",",","FirstItem,SecondItem","TestString;321,123"};
    std::string test_results_array[] = {"13","","SecondItem","123"};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        std::string input = test_input[i];
        if(!EraseFirstItemFromString(input) || input != test_results_array[i])
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(EraseFirstItemFromString, NegativeTest) 
{
    bool test_result = true;
    std::string test_input[] = {"1213","","`TestString;'z."};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        std::string input = test_input[i];
        if(EraseFirstItemFromString(input))
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(GetCellNumber, PositiveTest) 
{
    bool test_result = true;
    int test_input[][3] = {{3,2,2},{4,5,4},{7,7,1}};
    int test_results_array[] = {8,24,50};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        if(GetCellNumber(test_input[i][0],test_input[i][1],test_input[i][2]) != test_results_array[i])
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(StringContainAnyOperation, PositiveTest) 
{
    bool test_result = true;
    std::string test_input[] = {"op1/op2","op1*op2","op1+op2","op1-op2"};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        if(!StringContainAnyOperation(test_input[i]))
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(StringContainAnyOperation, NegativeTest) 
{
    bool test_result = true;
    std::string test_input[] = {"op1.op2","op1`op2","","0"};
    
    for(int i=0; i<sizeof(test_input)/sizeof(test_input[0]);++i)
    {
        if(StringContainAnyOperation(test_input[i]))
        {
            test_result = false;
        }
    }

    ASSERT_TRUE(test_result); 
}

TEST(ReplaceValueToInfinite, PositiveTest) 
{
    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = "SomeValue";

    bool test_result = true;
    
    ReplaceValueToInfinite(new_cell, "Test message", false);

    if(new_cell->current_cell_value != "inf")
        test_result = false;

    ASSERT_TRUE(test_result); 
}

TEST(ReplaceValueToInfinite, EmptyInput) 
{
    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = "";

    bool test_result = true;
    
    ReplaceValueToInfinite(new_cell, "Test message", false);

    if(new_cell->current_cell_value != "")
        test_result = false;

    ASSERT_TRUE(test_result); 
}

TEST(ArgumentToResult, PositiveNumber) 
{

    const std::string& text_view = "123";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(ArgumentToResult(new_cell->current_cell_value, cell_by_name, result, reference_list) && result == stoi(text_view)) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(ArgumentToResult, NegativeNumber) 
{

    const std::string& text_view = "-123";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(ArgumentToResult(new_cell->current_cell_value, cell_by_name, result, reference_list) && result == stoi(text_view)) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(ArgumentToResult, ValueFromCorrectCell) 
{

    const std::string& text_view = "Cell5";
    const int cell_result = 5;

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    new_cell->result = cell_result;
    new_cell->result_is_ready = true;

    cell_by_name.emplace(text_view,new_cell);

    bool test_result = false;
    
    if(ArgumentToResult(new_cell->current_cell_value, cell_by_name, result, reference_list) 
                                                                    && result == cell_result) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(ArgumentToResult, ValueFromIncorrectCell) 
{

    const std::string& text_view = "Cell5";
    const int cell_result = 5;

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    new_cell->result = cell_result;
    new_cell->result_is_ready = true;

    cell_by_name.emplace("SomeCell",new_cell);

    bool test_result = false;
    
    if(!ArgumentToResult(new_cell->current_cell_value, cell_by_name, result, reference_list)) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(CalculateSingleValues, AddTwoNumbers) 
{
    const std::string& text_view = "=5+2";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(CalculateSingleValues(new_cell, cell_by_name, reference_list) && new_cell->result == 7) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(CalculateSingleValues, SubtractionTwoNumbers) 
{
    const std::string& text_view = "=5-2";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(CalculateSingleValues(new_cell, cell_by_name, reference_list) && new_cell->result == 3) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(CalculateSingleValues, MultiplyTwoNumbers) 
{
    const std::string& text_view = "=5*2";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    int result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(CalculateSingleValues(new_cell, cell_by_name, reference_list) && new_cell->result == 10) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(CalculateSingleValues, DivisionTwoNumbers) 
{
    const std::string& text_view = "=5/2";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    double result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(CalculateSingleValues(new_cell, cell_by_name, reference_list) && new_cell->result == 2.5) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(CalculateSingleValues, DivisionByZero) 
{
    const std::string& text_view = "=5/0";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    double result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> new_cell = std::make_shared<Cell>();
    new_cell->current_cell_value = text_view;
    cell_by_name.emplace("Test1",new_cell);

    bool test_result = false;
    
    if(!CalculateSingleValues(new_cell, cell_by_name, reference_list)) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(CalculateSingleValues, PlusAnotherCorrectCell) 
{
    const std::string& text_view = "=A1+2";

    std::unordered_map<std::string, std::shared_ptr<Cell>> cell_by_name;
    double result;
    std::unordered_set<std::shared_ptr<Cell>> reference_list;

    std::shared_ptr<Cell> first_cell = std::make_shared<Cell>();
    first_cell->current_cell_value = text_view;

    std::shared_ptr<Cell> second_cell = std::make_shared<Cell>();
    second_cell->result = 4;
    second_cell->result_is_ready =true;

    cell_by_name.emplace("Test1",first_cell);
    cell_by_name.emplace("A1",second_cell);

    bool test_result = false;
    
    if(CalculateSingleValues(first_cell, cell_by_name, reference_list) && first_cell->result == 6) 
    {
        test_result = true;   
    }

    ASSERT_TRUE(test_result); 
}

TEST(GetFileName, ValidFileName)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"

    char* argv[] = { "./main", "test.csv" };
    std::string file_name;
    bool result = GetFileName(argv, file_name);
    EXPECT_TRUE(result);

    #pragma GCC diagnostic pop
}

TEST(GetFileName, InvalidFileName)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"

    char* argv[] = { "program.exe", "data.txt" };
    std::string file_name;
    bool result = GetFileName(argv, file_name);
    EXPECT_TRUE(!result);

    #pragma GCC diagnostic pop
}

TEST(GetFileName, MissingCommandLineArgument)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"

    char* argv[] = { "program.exe", nullptr };
    std::string file_name;
    bool result = GetFileName(argv, file_name);
    EXPECT_TRUE(!result);

    #pragma GCC diagnostic pop
}

TEST(GetFileName, ShortFileName)
{

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"

    char* argv[] = { "program.exe", "data" };
    std::string file_name;
    bool result = GetFileName(argv, file_name);
    EXPECT_TRUE(!result);

    #pragma GCC diagnostic pop
}

TEST(GetFileName, EmptyCommandLineArguments)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"

    char* argv[] = { "program.exe", "" };
    std::string file_name;
    bool result = GetFileName(argv, file_name);
    EXPECT_TRUE(!result);

    #pragma GCC diagnostic pop
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
