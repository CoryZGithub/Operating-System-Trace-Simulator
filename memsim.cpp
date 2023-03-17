#include "policies.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 4 || argc > 6) {
    cout << "Usage: ./memsim tracefile nFrames policy percentage quiet/debug" << endl;
    return 1;
  }

  // Initialize the parameters and variables
  const char *tracefile = argv[1];
  int nFrames = atoi(argv[2]);
  string policy = argv[3];
  int percentage = 0;
  bool debug = false;

  if (policy == "vms" && argc == 6) {
    percentage = atoi(argv[4]);
    debug = strcmp(argv[5], "debug") == 0;
  } else if (argc == 5) {
    debug = strcmp(argv[4], "debug") == 0; // compare if debug is active or not.
  }

  int Secondary_size = (nFrames * percentage) / 100;
  int Primary_size = nFrames - Secondary_size;
  // Run the specified policy on the trace file
  if (policy == "fifo") {
    FIFO(nFrames, tracefile, debug); // Declared in fifo.cpp
  } else if (policy == "lru") {
    LRU(nFrames, tracefile, debug); // Declared in lru.cpp
  } else if (policy == "vms") {
    if (percentage == 100 || Primary_size == 0) {
      LRU(nFrames, tracefile, debug);
    } else if (percentage == 0 || Secondary_size == 0) {
      FIFO(nFrames, tracefile, debug);
    } else {
      VMS(nFrames, tracefile, debug, percentage);
    }

  } else {
    cout << "Invalid replacement policy specified" << endl;
    return 1;
  }

  return 0;
}
