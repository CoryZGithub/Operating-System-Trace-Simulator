#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
// #include <bits/stdc++.h>
void LRU(int number_of_frames, const char* trace_file, bool debug)
{
    //printf("RUNNING LRU\n");
    std::ifstream tracefile(trace_file);
    if (!tracefile)
    {
        std::cerr << "Error: failed to open tracefile.\n";
        return;
    }

    std::vector<int> lruValues;
    std::vector<char> lruOperation;
    lruValues.reserve(number_of_frames);
    lruOperation.reserve(number_of_frames);
    char operation, tempOperation;
    unsigned addressHEX;
    int address = 0;
    int total_reads = 0, total_writes = 0;
    int ignore = 0, tempAddress = 0, tempLocation = 0, counter = 0;
    while (tracefile >> std::hex >> addressHEX >> operation)
    {
        counter++;
        address = addressHEX / 4096;

        if (debug == true){
            printf("Address is: %d\nOperation is '%c'\n", address, operation);
        }
        // If vector is empty
        if (lruValues.size() == 0)
        {
            //lruValues.insert(lruValues.begin(), address);
            //lruValues.insert(lruValues.begin(), operation);
            lruValues.push_back(address);
            lruOperation.push_back(operation);
            //printf("Hello once\n");
            total_reads++;
            continue;
        }
        //printf("%d SIZE\n", lruValues.size());
        if (int(lruValues.size()) < number_of_frames)
        {
            // search to see if value inside. 
            ignore = 0;
            for (int i = 0; i < int(lruValues.size()); i++)
            {
                // IF FOUND.
                if (lruValues.at(i) == address){
                ignore = 1;
                tempLocation = i;
                break; }
                
            }
           // printf("Jeloo");
           // printf("Templocation: %d\n", tempLocation);
                if (ignore == 1){
                    //printf("\nCHECKIT\n");
                    // FOUND VALUE, DELETE VALUE AND PUT AT FRONT.
                    tempAddress = lruValues.at(tempLocation);
                    tempOperation = lruOperation.at(tempLocation);
                    lruValues.erase(lruValues.begin() + tempLocation);
                    lruOperation.erase(lruOperation.begin() + tempLocation);
                    lruValues.insert(lruValues.begin(), tempAddress);

                    // INSERT CORRECT OPERATION
                    if (tempOperation == 'W')
                    {
                        lruOperation.insert(lruOperation.begin(), 'W');
                    }
                    else
                    {
                        lruOperation.insert(lruOperation.begin(), 'R');
                    }
                }
        else {  // IF NOT FOUND, ADD
            lruValues.insert(lruValues.begin(), address);
            lruOperation.insert(lruOperation.begin(), operation);
            total_reads++;
         }
            
        }
        // WHEN VECTOR IS FULL / PAGE TABLE.
        else {
            ignore = 0;
            for (int i = 0; i < int(lruValues.size()); i++)
            {
                if (lruValues.at(i) == address){
                // IF FOUND.
                if (operation == 'W') {           // If page is found inside, check if operaiton is W and apply.
                    lruOperation[i] = 'W'; }
                tempLocation = i;
                ignore = 1;
                break; }
            }
                if (ignore == 1){
                    // FOUND VALUE, DELETE VALUE AND PUT AT FRONT.
                    tempAddress = lruValues.at(tempLocation);
                    tempOperation = lruOperation.at(tempLocation);
                    lruValues.erase(lruValues.begin() + tempLocation);
                    lruOperation.erase(lruOperation.begin() + tempLocation);
                    lruValues.insert(lruValues.begin(), tempAddress);
                    // INSERT CORRECT OPERATION
                    if (tempOperation == 'W')
                    {
                        lruOperation.insert(lruOperation.begin(), 'W');
                    }
                    else
                    {
                        lruOperation.insert(lruOperation.begin(), 'R');
                    }
                } else {
                    // IF NOT FOUND, ADD

                    lruValues.pop_back();
                    if (lruOperation.back() == 'W'){
                        total_writes++;
                    }
                    lruOperation.pop_back();

                    lruValues.insert(lruValues.begin(), address);
                    lruOperation.insert(lruOperation.begin(), operation);
                    total_reads++;
                }
        }
        //printf("testererein.");
    }
   // printf("testss");
std::cout << "Memory Frames: " << number_of_frames << '\n';
std::cout << "Trace Count: " << counter << '\n';
std::cout << "Total disk reads: " << total_reads << '\n';
std::cout << "Total disk writes: " << total_writes << '\n';
return;    
    
}
