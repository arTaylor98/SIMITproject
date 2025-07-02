#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstring>

// Structure representing a shared memory header
#pragma pack(push, 1) // Ensure no padding is added
struct ShmHeaderFixed {
    uint32_t total_size;       // Size of the memory area
    uint32_t header_size;      // Size of the header area
    uint16_t version;          // Version of the header
    uint32_t sampling_time_ms; // Scanning/Sampling cycle in milliseconds
    uint16_t cycleCounter;     // Cycle counter
    uint8_t mutex_name_length; // Length of the mutex name
    // mutex_name (m bytes) and signals follow outside this struct
};
#pragma pack(pop) // Restore previous packing alignment

// Structure representing a signal in the shared memory
struct SignalEntry {
    std::string name;     // Signal name
    uint16_t address;           // Address of the signal
    bool is_input;       // I/O identifier of the signal
    uint8_t type_id;     // Type identifier of the signal
};
