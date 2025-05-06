# include <iostream>
# include <vector>
# include <string>
# include <queue>
# include <fstream>
# include <sstream>
# include <algorithm>

class Alu{

};

class Multiplier_Divider{

};

class Memory{

};
class Register{
private:
    unsigned int value;
    bool valid_bit;
    unsigned int tag;
public:
    Register() : value(0), valid_bit(true), tag(-1) {}
    bool isValid(){
        return valid_bit;
    }
    unsigned int getValue(){
        return value;
    }
    unsigned int getTag(){
        return tag;
    }
    void setValue(unsigned int value){
        this->value = value;
    }
    void setValidBit(bool valid_bit){
        this->valid_bit = valid_bit;
    }
    void setTag(unsigned int tag){
        this->tag = tag;
    }
};
class Register_file{
public:
    Register registers[8];    
};


class Instruction{
private:
    std::string opcode;
    std::string r1, r2, r3;
    std::string offset;
public:
    Instruction(std::string instruction){
        std::istringstream iss(instruction);
            std::string word;

            while (iss >> word) {                
                word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                this->opcode = word; 
                if(word == "ADD" || word == "SUB" || word == "MUL" || word == "DIV"){                    
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    this->r1 = word;
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    this->r2 = word;
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    this->r3 = word;
                }else if(word == "LOAD"){
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    this->r1 = word;
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    size_t pos = word.find('(');
                    this-> offset = word.substr(0, pos);
                    this->r2 = word.substr(pos + 1, word.length() - pos - 2);
                }else if(word == "STORE"){
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    size_t pos = word.find('(');
                    this->offset = word.substr(0, pos);
                    this->r2 = word.substr(pos + 1, word.length() - pos - 2);
                    iss >> word;
                    this->r1 = word;
                }
            }
    }
    void print(){
        std::cout << "Opcode: " << opcode << std::endl;        
        if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL" || opcode == "DIV"){
            std::cout << "Dest: " << r1 << std::endl;
            std::cout << "src1: " << r2 << std::endl;
            std::cout << "src2: " << r3 << std::endl;
        } else if (opcode == "LOAD"){
            std::cout << "Dest: " << r1 << std::endl;
            std::cout << "Offset: " << offset << std::endl;
            std::cout << "src1: " << r2 << std::endl;
        } else if (opcode == "STORE"){
            std::cout << "Offset: " << offset << std::endl;
            std::cout << "src1: " << r2 << std::endl;
            std::cout << "Dest: " << r1 << std::endl;
        }
    }
  
};

class Tomasulo_simulator{
private:
    std::queue<Instruction> instructions;
    size_t instruction_count;

public:
    Tomasulo_simulator(std::string code){
        std::ifstream file(code);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                this->instructions.push(Instruction(line));
                ++ this->instruction_count;
            }
            file.close();
        }else {
            std::cerr << "Error opening file!" << std::endl;
        }
                
    }
};




int main(int argc, char *argv[]){

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;  
    }
    std::string file_path = argv[1];
    std::cout << "File path: " << file_path << std::endl;

    return 0;
}

