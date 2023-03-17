#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>

void LRU(int number_of_frames, const char *trace_file, bool debug) {
  // Check if trace file exists
  std::ifstream tracefile(trace_file);
  if (!tracefile) {
    std::cerr << "Error: failed to open tracefile.\n";
    return;
  }

  // Initialize variables
  std::vector<int> lruValues;
  std::vector<char> lruOperation;
  lruValues.reserve(number_of_frames);
  lruOperation.reserve(number_of_frames);
  char operation, tempOperation;
  unsigned addressHEX;
  int address = 0;
  int total_reads = 0, total_writes = 0;
  int ignore = 0, tempAddress = 0, tempLocation = 0, counter = 0;

  // Read trace file line by line and parse
  while (tracefile >> std::hex >> addressHEX >> operation) {
    counter++;
    address = addressHEX / 4096;

    if (debug == true) {
      printf("Address is: %d\nOperation is '%c'\n", address, operation);
    }
    // If vector is empty
    if (lruValues.size() == 0) {
      lruValues.push_back(address);
      lruOperation.push_back(operation);
      total_reads++;
      continue;
    }
    // If vector is not full
    if (int(lruValues.size()) < number_of_frames) {
      ignore = 0;
      for (int i = 0; i < int(lruValues.size()); i++) {
        if (lruValues.at(i) == address) {
          ignore = 1;
          tempLocation = i;
          break;
        }
      }
      // If found, do following logic
      if (ignore == 1) {
        tempAddress = lruValues.at(tempLocation);
        tempOperation = lruOperation.at(tempLocation);
        lruValues.erase(lruValues.begin() + tempLocation);
        lruOperation.erase(lruOperation.begin() + tempLocation);
        lruValues.insert(lruValues.begin(), tempAddress);
        if (tempOperation == 'W') {
          lruOperation.insert(lruOperation.begin(), 'W');
        } else {
          lruOperation.insert(lruOperation.begin(), 'R');
        }
      } else { // If not found, add
        lruValues.insert(lruValues.begin(), address);
        lruOperation.insert(lruOperation.begin(), operation);
        total_reads++;
      }

    }
    // WHEN VECTOR IS FULL / PAGE TABLE
    else {
      ignore = 0;
      for (int i = 0; i < int(lruValues.size()); i++) {
        if (lruValues.at(i) == address) {
          // IF FOUND.
          if (operation == 'W') { // If page is found inside, check if operaiton
            lruOperation[i] = 'W'; // is W and apply
          }
          tempLocation = i;
          ignore = 1;
          break;
        }
      }
      // If found, do following logic
      if (ignore == 1) {
        // Found value.
        tempAddress = lruValues.at(tempLocation);
        tempOperation = lruOperation.at(tempLocation);
        lruValues.erase(lruValues.begin() + tempLocation);
        lruOperation.erase(lruOperation.begin() + tempLocation);
        lruValues.insert(lruValues.begin(), tempAddress);
        // Insert correct operation
        if (tempOperation == 'W') {
          lruOperation.insert(lruOperation.begin(), 'W');
        } else {
          lruOperation.insert(lruOperation.begin(), 'R');
        }

      } else {
        // If not found, add.
        lruValues.pop_back();
        if (lruOperation.back() == 'W') {
          total_writes++;
        }
        lruOperation.pop_back();

        lruValues.insert(lruValues.begin(), address);
        lruOperation.insert(lruOperation.begin(), operation);
        total_reads++;
      }
    }
  }
  std::cout << "Memory Frames: " << number_of_frames << '\n';
  std::cout << "Trace Count: " << counter << '\n';
  std::cout << "Total disk reads: " << total_reads << '\n';
  std::cout << "Total disk writes: " << total_writes << '\n';
  return;
}
