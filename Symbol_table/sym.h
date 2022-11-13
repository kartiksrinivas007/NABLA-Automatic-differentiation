#include <iomanip>
#include <vector>
#include <unordered_map>
#include <iostream>

class SymTabItem{
    public:
	std::string name;
	std::string dataType;
	std::string type;
	std::vector<int> Dims;
	int rowNum;
	int colNum;
};


void insertToSymbTab(std::unordered_map<std::string,SymTabItem>* ,char* ,int ,int ,std::string ,std::string);
void Undeclaration_Error(std::unordered_map<std::string,SymTabItem>* ,std::string );
SymTabItem* search(std::unordered_map<std::string,SymTabItem>* ,std::string);