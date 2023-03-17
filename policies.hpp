#ifndef POLICIES_HPP
#define POLICIES_HPP


// FIFO FUNCTION
void VMS(int number_of_frames, const char* trace_file, bool debug, int percentage);
void FIFO(int number_of_frames, const char* trace_file, bool debug);
void LRU(int number_of_frames, const char* trace_file, bool debug);
#endif // POLICIES_HPP