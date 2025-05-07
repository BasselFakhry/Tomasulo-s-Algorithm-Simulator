# include <iostream>
# include <vector>
# include <string>
# include <queue>
# include <fstream>
# include <sstream>
# include <algorithm>
#include <cassert>

class Alu{
private:
    size_t stages;
    size_t latency;
    std::vector<Instruction> pipeline;

public:

    

};

class Multiplier_Divider{
private:
    size_t stages;
    size_t latency;    
    std::vector<Instruction> pipeline;

public:

    

};

class Memory{
private:
    size_t stages;
    size_t latency;
    std::vector<Instruction> pipeline;

public:

   
};




class Register{
private:
    unsigned int value;
    bool valid_bit;
    unsigned int tag;
public:
    Register(bool valid = 1) : value(0), valid_bit(valid), tag(-1) {}
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

    void print(){
        std::cout << "Value: " << value << std::endl;
        std::cout << "Valid Bit: " << valid_bit << std::endl;
        std::cout << "Tag: " << tag << std::endl;
    }
};

class Reservation_station{
private:
    size_t capacity;
    size_t occupancy;
    bool busy;
    std::vector<Register> src1;
    std::vector<Register> src2;
    
public:
    Reservation_station(size_t capacity): capacity(capacity), occupancy(0), busy(false){
        src1.resize(capacity);
        src2.resize(capacity);        
    }
    size_t addInstruction(Register src1, Register src2){
        assert(!isFull());
        for(size_t i = 0; i < capacity; ++i){
            if(!this->src1[i].isValid()){
                this->src1[i] = src1;
                this->src2[i] = src2;
                ++this->occupancy;
                return i;
            }
        }
    }
    bool isFull(){
        return occupancy == capacity;
    }
    Register getSrc1(unsigned int tag){
        return src1[tag];
    }
    Register getSrc2(unsigned int tag){
        return src2[tag];
    }
    
};

class Register_file{
private:
    Register registers[8];
public:
    Register_file(){
        for (int i = 0; i < 8; ++i) {
            registers[i] = Register();
        }
    }



        
    
};


class Instruction{
private:
    std::string opcode;
    std::string r1, r2, r3;
    std::string offset;
    size_t instruction_count;
public:
    Instruction(std::string instruction, size_t instruction_count): instruction_count(instruction_count){
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
        std::cout << "Instruction "<< this->instruction_count << ": " << std::endl;
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
    size_t num_instructions;
    Register_file register_file;
    Alu alu1;
    Alu alu2;
    Multiplier_Divider multiplier_divider;
    Memory memory;
    unsigned int cycle;
    unsigned int add_sub_time;
    unsigned int mul_time;
    unsigned int div_time;
    unsigned int load_store_time;


public:
    Tomasulo_simulator(std::string code): num_instructions(0), cycle(0), add_sub_time(2), mul_time(10), div_time(20), load_store_time(5){
        std::ifstream file(code);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                this->instructions.push(Instruction(line, this->num_instructions));
                ++ this->num_instructions;
            }
            file.close();
        }else {
            std::cerr << "Error opening file!" << std::endl;
        }
                
    }
    void print(){
        std::cout << "Num of instructions " << this->num_instructions << std::endl;
        
        std::queue<Instruction> instructions_copy = this->instructions;
        while (!instructions_copy.empty()) {
            instructions_copy.front().print();
            instructions_copy.pop();

        }
    }
};




int main(int argc, char *argv[]){

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;  
    }
    std::string file_path = argv[1];
    //std::cout << "File path: " << file_path << std::endl;
    Tomasulo_simulator simulator(file_path);
    simulator.print();

    return 0;
}

