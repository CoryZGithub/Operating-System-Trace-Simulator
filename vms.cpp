#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
// #include <bits/stdc++.h>
void VMS(int number_of_frames, const char* trace_file, bool debug, int percentage)
{

    std::ifstream tracefile(trace_file);
    if (!tracefile)
    {
        std::cerr << "Error: failed to open tracefile.\n";
        return;
    }


    int Secondary_size = (number_of_frames * percentage)/100; 
    int Primary_size = number_of_frames - Secondary_size;
    //printf("Primary size: %d\nSecondary Size: %d\n", Primary_size, Secondary_size);


    
    std::deque<int> fifo;
    std::deque<char> fifoOperation;
    std::vector<int> lruValues;
    std::vector<char> lruOperation;
    int counter = 0;
    lruValues.reserve(Secondary_size);
    lruOperation.reserve(Secondary_size);
    char operation, tempOperation, tempFIFOOperator;
    unsigned addressHEX;
    int address = 0;
    int total_reads = 0, total_writes = 0;
    int ignore = 0, tempAddress = 0, tempLocation = 0, tempFIFOAddress = 0;
    while (tracefile >> std::hex >> addressHEX >> operation)
    {
        counter++;
        if (debug == true){
            printf("Address is: %d\nOperation is %c\n", address, operation);
        }
        address = addressHEX / 4096;
        // FIFO FUNCTION FIRST If vector is empty
        if (fifo.size() == 0){
            fifo.push_front(address); // add t ostack
            fifoOperation.push_front(operation);
            total_reads++;
            }

        else if (int(fifo.size()) < Primary_size) { // if FIFO < for ex: 64 aka empty 
            // START
            ignore = 0;
            for (int i = 0; i < int(fifo.size()); i++) {
                if (fifo.at(i) == address) {
                    ignore = 1;
                    tempLocation = i;
                    if (operation == 'W'){ // If page is found inside, check if operaiton is W and apply.
                        fifoOperation[i] = 'W';
                    };
                    
                }}
                if (ignore == 0){
                total_reads++;
                fifo.push_front(address);
                fifoOperation.push_front(operation);}
                else { // ELSE FIFO IS NOT FULL BUT MATCH WAS FOUND. SO, MOVE MATCH TO BACK.
                // tempAddress = fifo[tempLocation];
                // tempOperation = fifoOperation[tempLocation];
                // fifo.erase(fifo.begin() + tempLocation);
                // fifoOperation.erase(fifoOperation.begin() + tempLocation);
                // fifo.push_front(tempAddress);
                // fifoOperation.push_front(tempOperation);
                continue;
                }
        } else {
                // LOGIC STARTS TO MATTER AND INTEGRATES WITH LRU. 

                ignore = 0;
               
                for (int i = 0; i < int(fifo.size()); i++) { // CHECKS FIFO FIRST, (FIFO FULL AND MB EMPTY LRU).
                   
                    if (fifo.at(i) == address) { //TODO CHECK THIS THEN CHECK LRU, THENNNN input. printf(
                        ignore = 1; // FOUND <===
                        //tempTemp = i;   
                        
                        if (operation == 'W'){ // If page is found inside, check if operaiton is W and apply.    
                            fifoOperation[i] = 'W';
                        };
                        break;
                    }}

                   // //original"chickets\n");
                    if (ignore == 0){ // If FIFO is full but target not found --> SEARCH DEQUE. ===========================
                    // ========================================================
                    // == SEARCHING DEQUE NOW.
                    //original"buckets?\n");
                        if (lruValues.size() == 0)
                            {
                                int tempFIFOAddress = fifo.back();
                                char tempFIFOOperator = fifoOperation.back();
                                fifo.pop_back();
                                fifoOperation.pop_back();
                                fifo.push_front(address);
                                fifoOperation.push_front(operation);
                                total_reads++;
                                // grab values of end of LRUvector.??????????
                                // add back_of_FIFO values to LRU Vector.
                                lruValues.push_back(tempFIFOAddress);
                                lruOperation.push_back(tempFIFOOperator);
                                
                            }
                            else if (int(lruValues.size()) < Secondary_size)  // ERROR IN HERE YOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
                            {
                                // search to see if value inside. 
                                ignore = 0;
                                for (int i = 0; i < int(lruValues.size()); i++)
                                {
                                    // IF FOUND.
                                    if (lruValues.at(i) == address){ //THIS MATTERS --> IF FOUND, NEED TO MOVE TO FRONT OF FIFO. AND MOVE BACK OF FIFO INTO FRONT OF VECTOR.
                                     if (operation == 'W') {           // If page is found inside, check if operaiton is W and apply.
                                        lruOperation[i] = 'W'; }
                                    ignore = 1;
                                    tempLocation = i;
                                    break; }
                                    
                                }
                                    if (ignore == 1){
                                        // FOUND VALUE, DELETE VALUE AND PUT AT FRONT of FIFOOOOOOOOO.
                                        // grab values of end of FIFOdeque.
                                        //original"Hiello\n");
                                        int tempFIFOAddress = fifo.back();
                                        char tempFIFOOperator = fifoOperation.back(); 
                                        // grab values of end of LRUvector.
                                        tempAddress = lruValues[tempLocation];
                                        tempOperation = lruOperation[tempLocation];
                                        //original"Temp Address: %d\nTemp Operation: %c\n", tempAddress, tempOperation);
                                        // pop end of fifo && erase LRU Locatoin.
                                        fifo.pop_back();
                                        fifoOperation.pop_back();
                                        //original"Size AFTER POP: %d\n", fifo.size());
                                        
                                        lruValues.erase(lruValues.begin() + tempLocation);
                                        lruOperation.erase(lruOperation.begin() + tempLocation);

                                        // add back_of_FIFO values to LRU Vector.
                                        lruValues.insert(lruValues.begin(), tempFIFOAddress);
                                        lruOperation.insert(lruOperation.begin(), tempFIFOOperator);
                                        
                                        // add LRU Values into FIFO.
                                        //original"Size BEFORE PUSH: %d\n", fifo.size());
                                        fifo.push_front(tempAddress);
                                        fifoOperation.push_front(tempOperation);
                                        //original"Reinserted Address: %d\nReinserted Operation: %c\n", fifo[0], lruOperation[0]);
                                        //original"Size AFTER PUSH: %d\n", fifo.size());
                                    }
                            else {  // IF NOT FOUND, ADD
                                        int tempFIFOAddress = fifo.back();
                                        char tempFIFOOperator = fifoOperation.back();
                                        fifo.pop_back();
                                        fifoOperation.pop_back();
                                        fifo.push_front(address);
                                        fifoOperation.push_front(operation);
                                        total_reads++;
                                        // grab values of end of LRUvector.??????????
                                        // add back_of_FIFO values to LRU Vector.
                                        lruValues.insert(lruValues.begin(), tempFIFOAddress);
                                        lruOperation.insert(lruOperation.begin(), tempFIFOOperator);

                                        // add LRU Values into FIFO.
                                       
                            }
                                
                            }
                            // WHEN VECTOR IS FULL / PAGE TABLE and FIFO IS FULL TOO.
                            else {
                                //original" IS FULL\n");
                                ignore = 0;
                                for (int i = 0; i < int(lruValues.size()); i++)
                                {
                                    // IF FOUND.
                                    if (lruValues.at(i) == address){ //THIS MATTERS --> IF FOUND, NEED TO MOVE TO FRONT OF FIFO. AND MOVE BACK OF FIFO INTO FRONT OF VECTOR.
                                     if (operation == 'W') {  
                                        //original"Chili\n");         // If page is found inside, check if operaiton is W and apply.
                                        lruOperation[i] = 'W'; }
                                        //original"Lily\n");
                                    ignore = 1;
                                    tempLocation = i;
                                    break; }
                                    
                                }
                                    if (ignore == 1){
                                        //original"gatto\n");
                                        // FOUND VALUE, DELETE VALUE AND PUT AT FRONT of FIFOOOOOOOOO.
                                        // grab values of end of FIFOdeque.
                                        int tempFIFOAddress = fifo.back();
                                        char tempFIFOOperator = fifoOperation.back(); 
                                        // grab values of end of LRUvector.
                                        tempAddress = lruValues[tempLocation];
                                        tempOperation = lruOperation[tempLocation];
                                        //original"doggo\n");
                                        // pop end of fifo && erase LRU Locatoin.
                                        fifo.pop_back();
                                        fifoOperation.pop_back();
                                        lruValues.erase(lruValues.begin() + tempLocation);
                                        lruOperation.erase(lruOperation.begin() + tempLocation);
                                        //original"LOL\n");
                                        // add back_of_FIFO values to LRU Vector.
                                        lruValues.insert(lruValues.begin(), tempFIFOAddress);
                                        lruOperation.insert(lruOperation.begin(), tempFIFOOperator);
                                       
                                        // add LRU Values into FIFO.
                                        //original"doublecheckc\n");
                                        //original"Size of FIFO: %d\n", fifo.size());
                                        fifo.push_front(tempAddress);
                                        //original"WHAT?\n");
                                        fifoOperation.push_front(tempOperation);
                                        //original"FULL and moved things around\n");
                                    }
                            else {  // IF NOT FOUND, ADD
                                         // grab values of end of FIFOdeque.
                                        tempFIFOAddress = fifo.back();
                                        tempFIFOOperator = fifoOperation.back();
                                        fifo.pop_back();
                                        fifoOperation.pop_back();
                                        fifo.push_front(address);
                                        fifoOperation.push_front(operation);
                                        total_reads++;
                                        
                                        

                                        lruValues.pop_back();
                                        if (lruOperation.back() == 'W'){
                                            total_writes++;
                                        };
                                        lruOperation.pop_back();

                                        // add back_of_FIFO values to LRU Vector.
                                        lruValues.insert(lruValues.begin(), tempFIFOAddress);
                                        lruOperation.insert(lruOperation.begin(), tempFIFOOperator);

                                        // add LRU Values into FIFO.
                                       
                                    
                                    }
                               
                            }
                            }
                            else{;
                                continue;
                    }   
                    }}
                    std::cout << "Memory Frames: " << number_of_frames << '\n';
                    std::cout << "Trace Count: " << counter << '\n';
                    std::cout << "Total disk reads: " << total_reads << '\n';
                    std::cout << "Total disk writes: " << total_writes << '\n';
                    return;    
}








































