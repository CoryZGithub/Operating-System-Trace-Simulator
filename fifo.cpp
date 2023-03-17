#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <queue>

void FIFO(int number_of_frames, const char *trace_file, bool debug) {
  const int nframe = number_of_frames;
  std::ifstream tracefile(trace_file);
  if (!tracefile) {
    std::cerr << "Error: failed to open tracefile.\n";
    return;
  }

  std::deque<int> fifo;
  std::deque<char> fifoOperation;
  char operation;
  unsigned addressHEX;
  int counter = 0;
  int address = 0;
  int total_reads = 0, total_writes = 0;
  int ignore = 0;

  while (tracefile >> std::hex >> addressHEX >> operation) {
    counter++;
    if (debug == true) {
      printf("Address is: %d\nOperation is %c\n", address, operation);
    }
    address = addressHEX / 4096;
    // printf("NEXT %u, %c\n", address, operation);
    if (fifo.size() == 0) {
      fifo.push_front(address); // add t ostack
      fifoOperation.push_front(operation);
      total_reads++;
    }

    if (int(fifo.size()) < nframe) { // if FIFO < for ex: 64 aka empty
      // START
      ignore = 0;
      for (int i = 0; i < int(fifo.size()); i++) {

        if (fifo.at(i) == address) {
          ignore = 1;
          if (operation == 'W') { // If page is found inside, check if operaiton
                                  // is W and apply.
            fifoOperation.at(i) = 'W';
            break;
          };
        }
      }
      if (ignore == 0) {
        total_reads++;
        fifo.push_front(address);
        fifoOperation.push_front(operation);
      }
    } else {
      ignore = 0;
      for (int i = 0; i < int(fifo.size()); i++) {

        if (fifo.at(i) == address) {
          ignore = 1;
          if (operation == 'W') { // If page is found inside, check if operaiton
                                  // is W and apply.
            fifoOperation.at(i) = 'W';
            break;
          };
        }
      }
      if (ignore == 0) {

        if (fifoOperation.back() ==
            'W') { // READ END OF DEQUE, if W, total_writes++
          total_writes++;
        };
        fifo.pop_back();
        fifoOperation.pop_back();
        total_reads++;
        fifo.push_front(address);
        fifoOperation.push_front(operation);
      }
    }
  }
  std::cout << "Memory Frames: " << number_of_frames << '\n';
  std::cout << "Trace Count: " << counter << '\n';
  std::cout << "Total disk reads: " << total_reads << '\n';
  std::cout << "Total disk writes: " << total_writes << '\n';

  return;
}