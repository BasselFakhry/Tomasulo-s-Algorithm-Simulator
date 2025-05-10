# include <iostream>
# include <vector>
# include <string>
# include <queue>
# include <fstream>
# include <sstream>
# include <algorithm>
# include <cassert>
class Instruction{
private:
    std::string opcode;
    std::string r1, r2, r3;
    std::string offset;
    std::string current_state;
    size_t instruction_count;
    size_t start;
    size_t end;
    size_t duration;
    size_t stalls;

public:

    Instruction(std::string instruction, size_t instruction_count): current_state("Out"), instruction_count(instruction_count),duration(0), stalls(0){
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
    bool isDone(){
        return this->current_state == "Done";
    }
    bool hasStarted(){
        return this->current_state != "Out";
    }
    void setStart(size_t start){
        this->start = start;
    }
    void setEnd(size_t end){
        this->end = end;
    }
    void stall(){
        ++ this->stalls;
    }
    void incrementDuration(){
        ++ this->duration;
    }
    void setState(std::string state){
        this->current_state = state;
    }
    size_t getStalls(){
        return this->stalls;
    }
    size_t getStart(){
        return this->start;
    }
    size_t getEnd(){
        return this->end;
    }
    size_t getDuration(){
        return this->duration;
    }
    std::string getOpcode(){
        return this->opcode;
    }
    std::string getState(){
        return this->current_state;
    }
    float getStallRate(){
        return this->stalls / this->duration;
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
    void print_summary(){
        print();
        std::cout << "Start: " << start << std::endl;
        std::cout << "End: " << end << std::endl;
        std::cout << "Duration: " << duration << std::endl;
        std::cout << "Stalls: " << stalls << std::endl;
        std::cout << "Stall rate: " << getStallRate() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
      
};
    

class Register{
    private:
        size_t value;
        bool valid_bit;
        size_t tag;
    public:
        Register(bool valid = 1) : value(0), valid_bit(valid), tag(-1) {}
        bool isValid(){
            return valid_bit;
        }
        size_t getValue(){
            return value;
        }
        size_t getTag(){
            return tag;
        }
        void setValue(size_t value){
            this->value = value;
        }
        void setValidBit(bool valid_bit){
            this->valid_bit = valid_bit;
        }
        void setTag(size_t tag){
            this->tag = tag;
        }
    
        void print(){
            std::cout << "Value: " << value << std::endl;
            std::cout << "Valid Bit: " << valid_bit << std::endl;
            std::cout << "Tag: " << tag << std::endl;
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


class Reservation_station{
    private:
        size_t capacity;
        size_t occupancy;
        bool busy;
        std::vector<Register> src1;
        std::vector<Register> src2;
        
    public:
        Reservation_station(size_t capacity): capacity(capacity), occupancy(0), busy(false), src1(capacity,Register(false)), src2(capacity,Register(false)){
                
        }

        size_t addInstruction(Register src1, Register src2){
            assert(!isFull());
            for(size_t i = 0; i < capacity; ++i){
                if(!this->src1[i].isValid()){
                    this->src1[i] = src1;
                    this->src2[i] = src2;
                    if(++this->occupancy == capacity){
                        this->busy = true;
                    }
                    return i;
                }
            }
        }
        bool isFull(){
            return occupancy == capacity;
        }
        Register getSrc1(size_t tag){
            return src1[tag];
        }
        Register getSrc2(size_t tag){
            return src2[tag];
        }
        
};
    



class Alu{
private:
    size_t latency;
    size_t capacity;
    std::vector<size_t> pipeline;
    Reservation_station reservation_station;

public:
    Alu(size_t latency, size_t capacity, Reservation_station reservation_station):latency(latency), capacity(capacity), pipeline(latency,0), reservation_station(reservation_station){

    }
    size_t next_cycle(){
        size_t out = this->pipeline[this->latency - 1];
        for(size_t i = this-> latency - 1; i >= 1; --i){
            this->pipeline[i] = this->pipeline[i - 1];
        }
        this->pipeline[0] = 0;
        return out;
    }
    bool isFull(){
        return this->reservation_station.isFull();
    }

    

};

class Multiplier_Divider{
private:
    size_t latency;
    size_t capacity;
    std::vector<size_t> pipeline;
    Reservation_station reservation_station;
public:
    Multiplier_Divider(size_t latency, size_t capacity):latency(latency), pipeline(latency,0), capacity(capacity), reservation_station(capacity){

    }

    size_t next_cycle(){
        size_t out = this->pipeline[this->latency - 1];
        for(size_t i = this-> latency - 1; i >= 1; --i){
            this->pipeline[i] = this->pipeline[i - 1];
        }
        this->pipeline[0] = 0;
        return out;
    }
    bool isFull(){
        return this->reservation_station.isFull();
    }
    

};

class Memory{
private:
    size_t latency;
    size_t capacity;
    std::vector<size_t> pipeline;
    Reservation_station reservation_station;
public:
    Memory(size_t latency, size_t capacity):latency(latency), capacity(capacity), pipeline(latency,0), reservation_station(capacity){
        
    }
    bool isFull(){
        return this->reservation_station.isFull();
    }
};











class Tomasulo_simulator{
private:
    std::vector<Instruction> instructions;
    Alu alu1;
    Alu alu2;
    size_t num_instructions;
    Register_file register_file;    
    Multiplier_Divider multiplier_divider;
    Memory memory;
    size_t cycle;
    size_t add_sub_time;
    size_t mul_time;
    size_t div_time;
    size_t load_store_time;
    size_t add_sub_capacity;
    size_t mul_capacity;
    size_t div_capacity;
    size_t load_store_capacity;
    size_t remaining_instructions;
    std::queue<size_t> common_data_bus;
    size_t issue;
    

public:
    Tomasulo_simulator(std::string code): alu1(2, 2, Reservation_station(2)), alu2(1, 1, Reservation_station(1)), multiplier_divider(0, 0), memory(0, 0),issue(0){
        this->num_instructions = 0;
        this->cycle = 0;
        this->add_sub_time = 2;
        this->mul_time = 10;
        this->div_time = 20;
        this->load_store_time = 5;
        this->add_sub_capacity = 3;
        this->mul_capacity = 2;
        this->div_capacity = 2;
        this->load_store_capacity = 4;
        this->register_file = Register_file();
        Reservation_station alu_shared_rs (this->add_sub_capacity);
        this->alu1 = Alu(add_sub_time, add_sub_capacity, alu_shared_rs);
        this->alu2 = Alu(mul_time, mul_capacity, alu_shared_rs);
        this->multiplier_divider = Multiplier_Divider(mul_time, mul_capacity);
        this->memory = Memory(load_store_time, load_store_capacity);
        this->remaining_instructions = 0;
        
        std::ifstream file(code);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                this->instructions.push_back(Instruction(line, this->num_instructions ++));
            }
            file.close();
        }else {
            std::cerr << "Error opening file!" << std::endl;
        }

        this->remaining_instructions = this->num_instructions;
        

        while(this->remaining_instructions){

            ++this->cycle;

            size_t alu1_out = this->alu1.next_cycle();
            size_t alu2_out = this->alu2.next_cycle();
            size_t mul_out = this->multiplier_divider.next_cycle();
            //size_t mem_out = this->memory.next_cycle();

            for(size_t i = 0; i < this->num_instructions; ++i){
                if(this->instructions[i].isDone()){
                    continue;
                }else if(this->instructions[i].hasStarted()){
                    this->instructions[i].incrementDuration();

                    // check if the instruction in issue stage can proceed to the next stage
                    if(i == this->issue){
                        if(this->instructions[i].getOpcode() == "ADD" || this->instructions[i].getOpcode() == "SUB"){
                            if(this->alu1.isFull()){
                                this->instructions[i].stall();
                                break;
                            }else{

                                if(i < this->num_instructions - 1){
                                    this->instructions[i+1].setStart(this->cycle);
                                    this->instructions[i+1].setState("Issue");
                                    this->issue = i + 1;
                                    this->instructions[i+1].incrementDuration();
                                    break;
                                }
                            }
                        }else if(this->instructions[i].getOpcode() == "MUL" || this->instructions[i].getOpcode() == "DIV"){
                            if(this->multiplier_divider.isFull()){
                                this->instructions[i].stall();
                                break;
                            }else{

                                if(i < this->num_instructions - 1){
                                    this->instructions[i+1].setStart(this->cycle);
                                    this->instructions[i+1].setState("Issue");
                                    this->issue = i + 1;
                                    this->instructions[i+1].incrementDuration();
                                    break;
                                }
                            }
                        }else if(this->instructions[i].getOpcode() == "LOAD" || this->instructions[i].getOpcode() == "STORE"){
                            if(this->memory.isFull()){
                                this->instructions[i].stall();
                                break;
                            }else{

                                if(i < this->num_instructions - 1){
                                    this->instructions[i+1].setStart(this->cycle);
                                    this->instructions[i+1].setState("Issue");
                                    this->issue = i + 1;
                                    this->instructions[i+1].incrementDuration();
                                    break;
                                }
                            }
                        }
                    }else{
                        
                        if(this->instructions[i].getState() == "Reservation_Ex"){

                        }else if(this->instructions[i].getState() == "Execute"){
                            if(this->instructions[i].getOpcode() == "ADD" || this->instructions[i].getOpcode() == "SUB"){
                                if(alu1_out == i){
                                    common_data_bus.push(i);
                                    this->instructions[i].setState("CDB");
                                }else if(alu2_out == i){
                                    common_data_bus.push(i);
                                    this->instructions[i].setState("CDB");
                                }else{

                                }
                            }else if(this->instructions[i].getOpcode() == "MUL" || this->instructions[i].getOpcode() == "DIV"){
                                if(mul_out == i){
                                    common_data_bus.push(i);
                                    this->instructions[i].setState("CDB");
                                }else{
                                    
                                }
                            }else if(this->instructions[i].getOpcode() == "LOAD" || this->instructions[i].getOpcode() == "STORE"){

                            }
                            
                        }

                    }
                }
            }




            // write back            
            if(!common_data_bus.empty()){
                size_t instruction = common_data_bus.front();
                common_data_bus.pop();
                this->instructions[instruction].setState("Done");
                this->instructions[instruction].setEnd(this->cycle);
                this->instructions[instruction].print_summary();
                -- this->remaining_instructions;
                if(!common_data_bus.empty()){
                    std::queue<size_t> temp;
                    while(!common_data_bus.empty()){
                        size_t k = common_data_bus.front();
                        common_data_bus.pop();
                        this->instructions[k].stall();                        
                    }
                }
            }


        }
                
    }


    void print(){
        std::cout << "Num of instructions " << this->num_instructions << std::endl;
        
        
        for(size_t i = 0; i < this->num_instructions; ++i){
            this->instructions[i].print();
        }
        
    }

    void print_report(){
        std::cout << "Report: "<< std::endl;
        std::cout << "Num of instructions " << this->num_instructions << std::endl;
        std::cout << "Total execution time = " << this-> cycle <<" clock cycles." << std::endl;
        std::cout << "Average number of instructions per cycle = " << this->cycle / this->num_instructions << " (IPC)." << std::endl;
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

