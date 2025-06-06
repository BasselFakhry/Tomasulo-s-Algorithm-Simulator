# include <iostream>
# include <vector>
# include <string>
# include <queue>
# include <fstream>
# include <sstream>
# include <algorithm>
# include <cassert>
#include <unordered_map>

class Instruction{
private:
    std::string opcode;
    size_t r1, r2, r3;
    std::string offset;
    std::string current_state;
    size_t instruction_count;
    int start;
    size_t end;
    size_t duration;
    size_t stalls;
    size_t structural_stalls;
    size_t data_stalls;
    size_t res_stalls;

public:

    Instruction(std::string instruction, size_t instruction_count): current_state("Out"), instruction_count(instruction_count),duration(0), stalls(0), structural_stalls(0), res_stalls(0), start(-1){
        std::istringstream iss(instruction);
            std::string word;

            while (iss >> word) {                
                word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                this->opcode = word; 
                if(word == "ADD" || word == "SUB" || word == "MUL" || word == "DIV"){                    
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    std::string temp = word;
                    this->r1 = temp[1]-'0';
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    temp = word;
                    this->r2 = temp[1]-'0';
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    temp = word;
                    this->r3 = temp[1]-'0';
                }else if(word == "LOAD"){
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    std::string temp = word;
                    this->r1 = temp[1]-'0';
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    size_t pos = word.find('(');
                    this-> offset = word.substr(0, pos);
                    temp = word.substr(pos + 1, word.length() - pos - 2);
                    this->r2 = temp[1]-'0';
                }else if(word == "STORE"){
                    iss >> word;
                    word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                    size_t pos = word.find('(');
                    this->offset = word.substr(0, pos);
                    std::string temp = word.substr(pos + 1, word.length() - pos - 2);
                    this->r2 = temp[1]-'0';
                    iss >> word;
                    temp = word;
                    this->r1 = temp[1]-'0';
                }
            }
    }
    bool isDone(){
        return this->current_state == "Done";
    }
    bool hasStarted(){
        return this->start != -1;
    }
    void setStart(size_t start){
        this->start = start;
    }
    void setEnd(size_t end){
        this->end = end;
        this->current_state = "Done";
    }
    void stall(std::string hazard){
        ++ this->stalls;
        if(hazard == "structural"){
            ++ this->structural_stalls;
        }else if (hazard == "structural_res"){
            ++ this->structural_stalls;
            ++ this-> res_stalls;
        }else{
            ++ this->data_stalls;
        }
        
    }
    void incrementDuration(){
        ++ this->duration;
    }
    void setState(std::string state){
        this->current_state = state;
    }
    size_t getR1(){
        return this->r1;
    }
    size_t getR2(){
        return this->r2;
    }
    size_t getR3(){
        return this->r3;
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
    float getStructuralStallRate(){
        return this->structural_stalls / this->duration;
    }
    float getDataStallRate(){
        return this->data_stalls / this->duration;
    }
    float getResStallRate(){
        return this->res_stalls / this->duration;
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
        std::cout << "Structural stalls: " << structural_stalls << std::endl;
        std::cout << "Data stalls: " << data_stalls << std::endl;
        std::cout << "Structural stall rate: " << getStructuralStallRate() << std::endl;
        std::cout << "Data stall rate: " << getDataStallRate() << std::endl;
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
    bool isValid(size_t index){
        return registers[index].isValid();
    }
    size_t getValue(size_t index){
        return registers[index].getValue();
    }
    size_t getTag(size_t index){
        return registers[index].getTag();
    }
    void setValue(size_t index, size_t value){
        registers[index].setValue(value);
    }
    void setValidBit(size_t index, bool valid_bit){
        registers[index].setValidBit(valid_bit);
    }
    void setTag(size_t index, size_t tag){
        registers[index].setTag(tag);
    }
    Register getRegister(size_t index){
        return registers[index];
    }
          
        
};


class Reservation_station{
    private:
        size_t capacity;
        size_t occupancy;
        std::vector<bool> busy;
        std::vector<Register> src1;
        std::vector<Register> src2;
        std::unordered_map<size_t, size_t> Insctuction_res_map;
        std::unordered_map<size_t, size_t> res_instruction_map;
        size_t avg_occupancy;
    public:
        Reservation_station(size_t capacity): capacity(capacity), occupancy(0), busy(capacity,false), src1(capacity,Register(false)), src2(capacity,Register(false)){
                
        }
        void occupancy_avg(){
            this->avg_occupancy += this->occupancy / this->capacity;
        }
        size_t addInstruction(size_t instruction_count, Register src1, Register src2 = NULL, std::string opcode = "Arithmetic"){
            assert(!isFull());
            if(opcode == "ALL"){                
                for(size_t i = 0; i < capacity; ++i){
                    if(!this->busy[i]){
                        this->busy[i] = true;
                        this->src1[i].setValidBit(src1.isValid());
                        this->src2[i].setValidBit(src2.isValid());
                        this->src1[i].setValue(src1.getValue());
                        this->src2[i].setValue(src2.getValue());
                        this->src1[i].setTag(src1.getTag());
                        this->src2[i].setTag(src2.getTag()); 
                        this->Insctuction_res_map[instruction_count] = i;
                        this->res_instruction_map[i] = instruction_count;                   
                        ++ this->occupancy;
                        return i;
                    }
                }
            }else if(opcode == "LOAD"){
                for(size_t i = 0; i < capacity; ++i){
                    if(!this->busy[i]){
                        this->busy[i] = true;
                        this->src1[i].setValidBit(src1.isValid());
                        this->src1[i].setValue(src1.getValue());
                        this->src1[i].setTag(src1.getTag());
                        this->Insctuction_res_map[instruction_count] = i;
                        this->res_instruction_map[i] = instruction_count;                   
                        ++ this->occupancy;
                        return i;
                    }
                }
            }
            return 0;
        }
        
        

        bool isbusy(size_t reservation_entry){
            return this->busy[reservation_entry];
        }
        bool isFull(){
            return occupancy == capacity;
        }
        
                     
        Register& getSrc1(size_t tag){
            return src1[tag];
        }
        Register& getSrc2(size_t tag){
            return src2[tag];
        }
        bool areOperandsReady(size_t instruction_count, std::string load = " "){
            
            size_t tag = Insctuction_res_map[instruction_count];
            if(load == "LOAD" && this->src1[tag].isValid()){
                return true;
            }
            if(this->src1[tag].isValid() && this->src2[tag].isValid()){
                return true;
            }else{
                return false;
            }
        }
        size_t getInstruction(size_t reservation_entry){
            return this->res_instruction_map[reservation_entry];
        }
        size_t getReservationEntry(size_t instruction_count){
            return this->Insctuction_res_map[instruction_count];
        }
        size_t getAvgOccupancy(){
            return this->avg_occupancy;
        }
        void clear(size_t reservation_entry){
            -- this->occupancy;
            this->busy[reservation_entry] = false;
            size_t instr = this->res_instruction_map[reservation_entry];
            this->res_instruction_map.erase(reservation_entry);
            this->Insctuction_res_map.erase(instr);
        }

        
};
    



class Alu{
private:
    size_t latency;
    size_t capacity;
    std::vector<int> pipeline;
    Reservation_station* reservation_station;
    Reservation_station* reservation_station_mul;
    Reservation_station* reservation_station_mem;

public:
    Alu(Reservation_station* reservation_station = nullptr, Reservation_station* reservation_station_mul = nullptr, Reservation_station* reservation_station_mem = nullptr, size_t latency = 0, size_t capacity = 0): reservation_station(reservation_station), reservation_station_mul(reservation_station_mul), reservation_station_mem(reservation_station_mem), latency(latency), capacity(capacity), pipeline(latency,-1){

    }
    // Function to get the next cycle
    int next_cycle(int ready = -1){
        int out = this->pipeline[this->latency - 1];
        for(size_t i = this-> latency - 1; i >= 1; --i){
            this->pipeline[i] = this->pipeline[i - 1];
        }
        this->pipeline[0] = ready;
        // returns the last element of the pipeline
        return out;
    }
    bool isFull(){
        return this->reservation_station->isFull();
    }
    size_t addInstruction(size_t instruction_count, Register src1, Register src2){
        assert(!isFull());
        size_t tag = this->reservation_station->addInstruction(instruction_count, src1, src2);     
        return tag;   
    }

    

};

class Multiplier_Divider{
private:
    size_t latency;
    size_t capacity;
    std::vector<int> pipeline;
    Reservation_station* reservation_station;
    Reservation_station* reservation_station_alu;
    Reservation_station* reservation_station_mem;
public:
    Multiplier_Divider(Reservation_station* reservation_station = nullptr, Reservation_station* reservation_station_alu = nullptr, Reservation_station* reservation_station_mem = nullptr, size_t latency = 0, size_t capacity = 0):reservation_station(reservation_station), reservation_station_alu(reservation_station_alu), reservation_station_mem(reservation_station_mem), latency(latency), pipeline(latency,-1), capacity(capacity){

    }

    int next_cycle(int ready = -1){
        int out = this->pipeline[this->latency - 1];
        for(size_t i = this-> latency - 1; i >= 1; --i){
            this->pipeline[i] = this->pipeline[i - 1];
        }
        this->pipeline[0] = ready;
               
        return out;
    }
    bool isFull(){
        return this->reservation_station->isFull();
    }
    size_t addInstruction(size_t instruction_count, Register src1, Register src2){
        assert(!isFull());
        size_t tag = this->reservation_station->addInstruction(instruction_count, src1, src2); 
        return tag;       
    }

};

class Memory{
private:
    size_t latency;
    size_t capacity;
    std::vector<int> pipeline;
    Reservation_station* reservation_station;
    Reservation_station* reservation_station_alu;
    Reservation_station* reservation_station_mul;
public:
    Memory(Reservation_station* reservation_station = nullptr, Reservation_station* reservation_station_alu = nullptr, Reservation_station* reservation_station_mul = nullptr, size_t latency = 0, size_t capacity = 0):reservation_station(reservation_station), reservation_station_alu(reservation_station_alu), reservation_station_mul(reservation_station_mul), latency(latency), pipeline(latency,-1), capacity(capacity){

    }
    bool isFull(){
        return this->reservation_station->isFull();
    }
    size_t addInstruction(size_t instruction_count, Register src1, Register src2 = NULL, std::string opcode = "ALL"){
        assert(!isFull());
        size_t tag = this->reservation_station->addInstruction(instruction_count, src1, src2, opcode);  
        return tag;      
    }
    int next_cycle(int ready = -1){
        int out = this->pipeline[this->latency - 1];
        for(size_t i = this-> latency - 1; i >= 1; --i){
            this->pipeline[i] = this->pipeline[i - 1];
        }
        this->pipeline[0] = ready;               
        return out;
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
    size_t avg_occupancy;
    

public:
    Tomasulo_simulator(std::string code){
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
        Reservation_station mul_shared_rs(load_store_capacity);
        Reservation_station load_store_shared_rs(load_store_capacity);
        this->alu1 = Alu(&alu_shared_rs, &mul_shared_rs, &load_store_shared_rs, add_sub_time, add_sub_capacity);
        this->alu2 = Alu(&alu_shared_rs, &mul_shared_rs, &load_store_shared_rs, add_sub_time, add_sub_capacity);
        this->multiplier_divider = Multiplier_Divider(&mul_shared_rs, &alu_shared_rs, &load_store_shared_rs, mul_time, mul_capacity);
        this->memory = Memory(&load_store_shared_rs, &alu_shared_rs, &mul_shared_rs, load_store_time, load_store_capacity);
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
        
        this->issue = 0; 
        this->instructions[0].setStart(1);
        this->instructions[0].setState("Issue"); 
        while(this->remaining_instructions > 0){
            
            ++this->cycle;
            std::string state_1 = "NOP";
            std::string state_2 = "NOP";
            
            // Check if there is ready instruction in the reservation station

            if(alu_shared_rs.isbusy(0)){
                state_1 = this->instructions[alu_shared_rs.getInstruction(0)].getState();
            }
            if(alu_shared_rs.isbusy(1)){
                state_2 = this->instructions[alu_shared_rs.getInstruction(1)].getState();
            }
            int ready_1 = -1;
            int ready_2 = -1;
            if(state_1 == "Reservation_ready"){
                ready_1 = alu_shared_rs.getInstruction(0);
                instructions[ready_1].setState("Execute");
                alu_shared_rs.clear(0);
            }
            if(state_2 == "Reservation_ready"){
                ready_2 = alu_shared_rs.getInstruction(1);
                instructions[ready_2].setState("Execute");
                alu_shared_rs.clear(1);
            }
            int alu1_out = this->alu1.next_cycle(ready_1);
            int alu2_out = this->alu2.next_cycle(ready_2);


            // After having new operands checking if any reservation station entry can be ready

            int ready_mul = -1;
            for(size_t i = 0; i < this->mul_capacity; ++i){
                if(mul_shared_rs.isbusy(i)){
                    int candidate = mul_shared_rs.getInstruction(i);
                    if(this->instructions[candidate].getState() == "Reservation_ready"){
                        ready_mul = candidate;
                        this->instructions[candidate].setState("Execute");
                        mul_shared_rs.clear(i);
                        break;
                    }
                }
            }
            int mul_out = this->multiplier_divider.next_cycle(ready_mul);

            int ready_mem = -1;
            for(size_t i = 0; i < this->load_store_capacity; ++i){
                if(load_store_shared_rs.isbusy(i)){
                    int candidate = load_store_shared_rs.getInstruction(i);
                    if(this->instructions[candidate].getState() == "Reservation_ready"){
                        ready_mem = candidate;
                        this->instructions[candidate].setState("Execute");
                        load_store_shared_rs.clear(i);
                        break;
                    }
                }
            }
            int mem_out = this->memory.next_cycle(ready_mem);
            if(mul_out != -1){
                
                size_t tag = 10 + mul_shared_rs.getReservationEntry(mul_out);
                for(size_t i = 0; i < this->add_sub_capacity; ++i){
                    if(alu_shared_rs.isbusy(i)){
                        size_t candidate = alu_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            size_t in_rs = alu_shared_rs.getReservationEntry(candidate);
                            bool flag = false;
                            if(alu_shared_rs.getSrc1(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc1(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(alu_shared_rs.getSrc2(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc2(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(alu_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                            
                        }
                }
                for(size_t i = 0; i < this->mul_capacity; ++i){
                    if(mul_shared_rs.isbusy(i)){
                        size_t candidate = mul_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(mul_shared_rs.getSrc1(i).getTag() == tag){
                                mul_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(mul_shared_rs.getSrc2(i).getTag() == tag){
                                mul_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(mul_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }                
                for(size_t i = 0; i < this->load_store_capacity; ++i){
                    if(load_store_shared_rs.isbusy(i)){
                        size_t candidate = load_store_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(load_store_shared_rs.getSrc1(i).getTag() == tag){
                                load_store_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(load_store_shared_rs.getSrc2(i).getTag() == tag){
                                load_store_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(load_store_shared_rs.areOperandsReady(candidate,this->instructions[candidate].getOpcode())){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }
            }
            if(alu1_out != -1){
                
                size_t tag = alu_shared_rs.getReservationEntry(alu1_out); 
                for(size_t i = 0; i < this->add_sub_capacity; ++i){
                    if(alu_shared_rs.isbusy(i)){
                        size_t candidate = alu_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            size_t in_rs = alu_shared_rs.getReservationEntry(candidate);
                            bool flag = false;
                            if(alu_shared_rs.getSrc1(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc1(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(alu_shared_rs.getSrc2(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc2(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(alu_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                            
                        }
                }
                for(size_t i = 0; i < this->mul_capacity; ++i){
                    if(mul_shared_rs.isbusy(i)){
                        size_t candidate = mul_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(mul_shared_rs.getSrc1(i).getTag() == tag){
                                mul_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(mul_shared_rs.getSrc2(i).getTag() == tag){
                                mul_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(mul_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }                
                for(size_t i = 0; i < this->load_store_capacity; ++i){
                    if(load_store_shared_rs.isbusy(i)){
                        size_t candidate = load_store_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(load_store_shared_rs.getSrc1(i).getTag() == tag){
                                load_store_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(load_store_shared_rs.getSrc2(i).getTag() == tag){
                                load_store_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(load_store_shared_rs.areOperandsReady(candidate,this->instructions[candidate].getOpcode())){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }
            }
            if(alu2_out != -1){
                
                size_t tag = alu_shared_rs.getReservationEntry(alu2_out); 
                for(size_t i = 0; i < this->add_sub_capacity; ++i){
                    if(alu_shared_rs.isbusy(i)){
                        size_t candidate = alu_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            size_t in_rs = alu_shared_rs.getReservationEntry(candidate);
                            bool flag = false;
                            if(alu_shared_rs.getSrc1(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc1(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(alu_shared_rs.getSrc2(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc2(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(alu_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                            
                        }
                }
                for(size_t i = 0; i < this->mul_capacity; ++i){
                    if(mul_shared_rs.isbusy(i)){
                        size_t candidate = mul_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(mul_shared_rs.getSrc1(i).getTag() == tag){
                                mul_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(mul_shared_rs.getSrc2(i).getTag() == tag){
                                mul_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(mul_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }                
                for(size_t i = 0; i < this->load_store_capacity; ++i){
                    if(load_store_shared_rs.isbusy(i)){
                        size_t candidate = load_store_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(load_store_shared_rs.getSrc1(i).getTag() == tag){
                                load_store_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(load_store_shared_rs.getSrc2(i).getTag() == tag){
                                load_store_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(load_store_shared_rs.areOperandsReady(candidate,this->instructions[candidate].getOpcode())){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }
            }
            if(mem_out != -1){
                
                size_t tag = 20 + load_store_shared_rs.getReservationEntry(mem_out);
                for(size_t i = 0; i < this->add_sub_capacity; ++i){
                    if(alu_shared_rs.isbusy(i)){
                        size_t candidate = alu_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            size_t in_rs = alu_shared_rs.getReservationEntry(candidate);
                            bool flag = false;
                            if(alu_shared_rs.getSrc1(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc1(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(alu_shared_rs.getSrc2(in_rs).getTag() == tag){
                                alu_shared_rs.getSrc2(in_rs).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(alu_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                            
                        }
                }
                for(size_t i = 0; i < this->mul_capacity; ++i){
                    if(mul_shared_rs.isbusy(i)){
                        size_t candidate = mul_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(mul_shared_rs.getSrc1(i).getTag() == tag){
                                mul_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(mul_shared_rs.getSrc2(i).getTag() == tag){
                                mul_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(mul_shared_rs.areOperandsReady(candidate)){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }                
                for(size_t i = 0; i < this->load_store_capacity; ++i){
                    if(load_store_shared_rs.isbusy(i)){
                        size_t candidate = load_store_shared_rs.getInstruction(i);
                        if(this->instructions[candidate].getState() == "Reservation_Ex"){
                            
                            bool flag = false;
                            if(load_store_shared_rs.getSrc1(i).getTag() == tag){
                                load_store_shared_rs.getSrc1(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(load_store_shared_rs.getSrc2(i).getTag() == tag){
                                load_store_shared_rs.getSrc2(i).setValidBit(true);
                                flag = true;                    
                            }
                            if(flag){
                                if(load_store_shared_rs.areOperandsReady(candidate,this->instructions[candidate].getOpcode())){
                                    this->instructions[candidate].setState("Reservation_ready");
                                }
                            }
                        }
                    }
                }
            }


            for(size_t i = 0; i < this->num_instructions; ++i){
                // Iterating over the instructions
                if(this->instructions[i].isDone()){
                    // if done proceed
                    std::cout<<"Done";                    
                    continue;
                }else if(this->instructions[i].hasStarted()){
                    // if started increment the duration

                    this->instructions[i].incrementDuration();
                
                    // check if the instruction in issue stage can proceed to the next stage
                    if(i == this->issue){
                        if(this->instructions[i].getOpcode() == "ADD" || this->instructions[i].getOpcode() == "SUB"){
                            if(this->alu1.isFull()){
                                this->instructions[i].stall("structural");
                                break;
                            }else{
                                this->instructions[i].setState("Reservation_Ex");
                                size_t src1 = this->instructions[i].getR2();
                                size_t src2 = this->instructions[i].getR3();
                                size_t dest = this->instructions[i].getR1();
                                size_t dest_tag = alu1.addInstruction(i, this->register_file.getRegister(src1), this->register_file.getRegister(src2));
                                this->register_file.setTag(dest, dest_tag);
                                this->register_file.setValidBit(dest, false);
                                if(alu_shared_rs.areOperandsReady(i)){
                                    this->instructions[i].setState("Reservation_ready");
                                }

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
                                this->instructions[i].stall("structural");
                                break;
                            }else{
                                this->instructions[i].setState("Reservation_Ex");
                                size_t src1 = this->instructions[i].getR2();
                                size_t src2 = this->instructions[i].getR3();
                                size_t dest = this->instructions[i].getR1();
                                size_t dest_tag = multiplier_divider.addInstruction(i, this->register_file.getRegister(src1), this->register_file.getRegister(src2));
                                this->register_file.setTag(dest, 10 + dest_tag);
                                this->register_file.setValidBit(dest, false);


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
                                this->instructions[i].stall("structural");
                                break;
                            }else{
                                this->instructions[i].setState("Reservation");
                                if(this->instructions[i].getOpcode() == "LOAD"){
                                    size_t src1 = this->instructions[i].getR2();
                                    size_t dest = this->instructions[i].getR1();
                                    size_t dest_tag = memory.addInstruction(i, this->register_file.getRegister(src1),NULL,"LOAD");
                                    this->register_file.setTag(dest, 20 + dest_tag);
                                    this->register_file.setValidBit(dest, false);
                                }else{
                                    size_t src1 = this->instructions[i].getR1();
                                    size_t src2 = this->instructions[i].getR2();
                                    size_t dest_tag = memory.addInstruction(i, this->register_file.getRegister(src1), this->register_file.getRegister(src2));
                                }
                                
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
                        // if not in issue stage check if the instruction is in reservation stage
                        if(this->instructions[i].getState() == "Reservation_Ex"){
                            this->instructions[i].stall("data");
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
                                if(mem_out == i){
                                    if(this->instructions[i].getOpcode() == "LOAD"){
                                        common_data_bus.push(i);
                                        this->instructions[i].setState("CDB");
                                    }else{
                                        this->instructions[i].setState("Done");
                                        this->instructions[i].setEnd(this->cycle);
                                        this->instructions[i].print_summary();
                                        --this->remaining_instructions;
                                    }
                                }
                            }
                            
                        }else if (this->instructions[i].getState() == "Reservation_ready"){
                            this->instructions[i].stall("structural_res");
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
                        this->instructions[k].stall("structural");                        
                    }
                }
            }
            alu_shared_rs.occupancy_avg();
            mul_shared_rs.occupancy_avg();
            load_store_shared_rs.occupancy_avg();

        }
        this-> avg_occupancy = alu_shared_rs.getAvgOccupancy() + mul_shared_rs.getAvgOccupancy() + load_store_shared_rs.getAvgOccupancy();
        this-> avg_occupancy /= 3;
        this-> avg_occupancy /= this->cycle;
                
    }


    void print(){
        std::cout << "Num of instructions " << this->num_instructions << std::endl;        
        
        for(size_t i = 0; i < this->num_instructions; ++i){
            this->instructions[i].print();            
        }
        
        
    }

    void print_report(){
        float avg_stall_rate = 0;
        float avg_structural_stall_rate = 0;
        float avg_data_stall_rate = 0;
        for(size_t i = 0; i < this->num_instructions; ++i){
            
            avg_stall_rate += this->instructions[i].getStallRate();
            avg_structural_stall_rate += this->instructions[i].getStructuralStallRate();
            avg_data_stall_rate += this->instructions[i].getDataStallRate();
        }
        avg_stall_rate /= this->num_instructions;
        avg_structural_stall_rate /= this->num_instructions;
        avg_data_stall_rate /= this->num_instructions;
        std::cout << "Report: "<< std::endl;
        std::cout << "Num of instructions " << this->num_instructions << std::endl;
        std::cout << "Total execution time = " << this-> cycle <<" clock cycles." << std::endl;  
        std::cout << "Average number of instructions per cycle = " << this->num_instructions / this->cycle  << " (IPC)." << std::endl;
        std::cout << "Average reservation station occupancy = " << this->avg_occupancy *100 << "%" <<std::endl;
        
    }
};




int main(int argc, char *argv[]){


    return 0;
}

