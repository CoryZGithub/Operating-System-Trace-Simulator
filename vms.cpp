#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
void VMS(int number_of_frames, const char *trace_file, bool debug, int percentage) {

  std::ifstream tracefile(trace_file);
  if (!tracefile) {
    std::cerr << "Error: failed to open tracefile.\n";
    return;
  }

  int Secondary_size = (number_of_frames * percentage) / 100;
  int Primary_size = number_of_frames - Secondary_size;

  // Initialize variables, tables, etc.
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

  while (tracefile >> std::hex >> addressHEX >> operation) {
    counter++; // Counting number of traces.
    if (debug == true) {
      printf("Address is: %d\nOperation is %c\n", address, operation);
    }
    address = addressHEX / 4096;

    // Fifo first. If vector is empty
    if (fifo.size() == 0) {
      fifo.push_front(address); // add to stack
      fifoOperation.push_front(operation);
      total_reads++;
    }
    // Fifo while Fifo is not full
    else if (int(fifo.size()) < Primary_size) { // if FIFO < for ex: 64 aka empty
      ignore = 0;
      for (int i = 0; i < int(fifo.size()); i++) {
        if (fifo.at(i) == address) {
          ignore = 1;
          tempLocation = i;
          if (operation == 'W') { // If page is found inside, check if operaiton
            fifoOperation[i] = 'W'; // is W and apply.
          };
        }
      }
      if (ignore == 0) {
        total_reads++;
        fifo.push_front(address);
        fifoOperation.push_front(operation);
      } else { // Else, found in FIFO. Start again.
        continue;
      }
    } else {
      // Logic begins to integrate with LRU.
      ignore = 0;

      for (int i = 0; i < int(fifo.size());i++) { // Check FIFO first. Then go to LRU.
        if (fifo.at(i) == address) {
          ignore = 1;  // Found
          if (operation == 'W') { // If page is found inside, check if operaiton
            fifoOperation[i] = 'W'; // is W and apply.
          };
          break;
        }
      }
      if (ignore == 0) { // If FIFO is full but target not found --> SEARCH
        if (lruValues.size() == 0) {

          int tempFIFOAddress = fifo.back();
          char tempFIFOOperator = fifoOperation.back();
          fifo.pop_back();
          fifoOperation.pop_back();
          fifo.push_front(address);
          fifoOperation.push_front(operation);
          total_reads++;
          lruValues.push_back(tempFIFOAddress);
          lruOperation.push_back(tempFIFOOperator);

        } else if (
            int(lruValues.size()) < Secondary_size) 
        {
          // search to see if value inside.
          ignore = 0;
          for (int i = 0; i < int(lruValues.size()); i++) {
            // If Found.
            if (lruValues.at(i) == address) { 
              if (operation == 'W') { // If page is found inside, check if
                lruOperation[i] = 'W'; // operaiton is W and apply.
              }
              ignore = 1;
              tempLocation = i;
              break;
            }
          }
          // Found value.
          if (ignore == 1) {
        
            int tempFIFOAddress = fifo.back();
            char tempFIFOOperator = fifoOperation.back();
            // grab values of end of LRUvector.
            tempAddress = lruValues[tempLocation];
            tempOperation = lruOperation[tempLocation];

            // pop end of fifo && erase LRU Locatoin.
            fifo.pop_back();
            fifoOperation.pop_back();

            lruValues.erase(lruValues.begin() + tempLocation);
            lruOperation.erase(lruOperation.begin() + tempLocation);

            // add back_of_FIFO values to LRU Vector.
            lruValues.insert(lruValues.begin(), tempFIFOAddress);
            lruOperation.insert(lruOperation.begin(), tempFIFOOperator);

            // add LRU Values into FIFO.
            fifo.push_front(tempAddress);
            fifoOperation.push_front(tempOperation);
          } else { // If not found, add.
            int tempFIFOAddress = fifo.back();
            char tempFIFOOperator = fifoOperation.back();
            fifo.pop_back();
            fifoOperation.pop_back();
            fifo.push_front(address);
            fifoOperation.push_front(operation);
            total_reads++;

            lruValues.insert(lruValues.begin(), tempFIFOAddress);
            lruOperation.insert(lruOperation.begin(), tempFIFOOperator);

          }

        }
        // When vector is full and FIFO is full too.
        else {
          ignore = 0;
          for (int i = 0; i < int(lruValues.size()); i++) {
            // IF FOUND.
            if (lruValues.at(i) == address) { 
              if (operation == 'W') {
                lruOperation[i] = 'W';
              }
              ignore = 1;
              tempLocation = i;
              break;
            }
          }
          if (ignore == 1) {
            int tempFIFOAddress = fifo.back();
            char tempFIFOOperator = fifoOperation.back();
            // grab values of end of LRUvector.
            tempAddress = lruValues[tempLocation];
            tempOperation = lruOperation[tempLocation];

            // pop end of fifo && erase LRU Locatoin.
            fifo.pop_back();
            fifoOperation.pop_back();
            lruValues.erase(lruValues.begin() + tempLocation);
            lruOperation.erase(lruOperation.begin() + tempLocation);

            // add back_of_FIFO values to LRU Vector.
            lruValues.insert(lruValues.begin(), tempFIFOAddress);
            lruOperation.insert(lruOperation.begin(), tempFIFOOperator);

            // add LRU Values into FIFO.
            fifo.push_front(tempAddress);;
            fifoOperation.push_front(tempOperation);
          } else { // IF NOT FOUND, ADD
            tempFIFOAddress = fifo.back();
            tempFIFOOperator = fifoOperation.back();
            fifo.pop_back();
            fifoOperation.pop_back();
            fifo.push_front(address);
            fifoOperation.push_front(operation);
            total_reads++;

            lruValues.pop_back();
            if (lruOperation.back() == 'W') {
              total_writes++;
            };
            lruOperation.pop_back();

            // add back_of_FIFO values to LRU Vector.
            lruValues.insert(lruValues.begin(), tempFIFOAddress);
            lruOperation.insert(lruOperation.begin(), tempFIFOOperator);

            // add LRU Values into FIFO.
          }
        }
      } else {
        ;
        continue;
      }
    }
  }
  std::cout << "Memory Frames: " << number_of_frames << '\n';
  std::cout << "Trace Count: " << counter << '\n';
  std::cout << "Total disk reads: " << total_reads << '\n';
  std::cout << "Total disk writes: " << total_writes << '\n';
  return;
}
