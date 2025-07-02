#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <iostream>

// Structure representing a shared memory header
#pragma pack(push, 1) // Ensure no padding is added
struct ShmHeaderFixed {
    uint32_t total_size;    // Size of the memory area
    uint32_t header_size;    // Size of the header area
    uint16_t version;           // Version of the header
    uint32_t sampling_time_ms;   // Scanning/Sampling cycle in milliseconds
    uint16_t cycleCounter;      // Cycle counter
    uint8_t mutex_name_length;    // Length of the mutex name
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

class ShmHeaderParser {
public:
    ShmHeaderParser(uint8_t* shm_ptr, size_t shm_size)
        : shm_ptr_(shm_ptr), shm_size_(shm_size) {
    }

    void parse() {
        if (!shm_ptr_ || shm_size_ < sizeof(ShmHeaderFixed)) {
            throw std::runtime_error("Invalid SHM pointer or too small for header");
        }

        // Read fixed header part
        header_ = reinterpret_cast<ShmHeaderFixed*>(shm_ptr_);
        print_fixed_header();

        if (header_->header_size > header_->total_size || header_->header_size > shm_size_) {
            throw std::runtime_error("Invalid header size exceeds SHM size");
        }

        // Read mutex name
        size_t offset = sizeof(ShmHeaderFixed);
        if (offset + header_->mutex_name_length > header_->header_size) {
            throw std::runtime_error("Mutex name overruns header boundary");
        }
        mutex_name_ = read_string(offset, header_->mutex_name_length);
        offset += header_->mutex_name_length;
        std::cout << "Mutex name: " << mutex_name_ << "\n";

        // Read signal list entries dynamically
        while (offset < header_->header_size) {
            uint8_t name_len = read_byte(offset);

            if (name_len == 0) { // End identifier found
                std::cout << "End of signal list reached.\n";
                offset += 1;
                break;
            }

            offset += 1;
            if (offset + name_len + 4 > header_->header_size) {
                throw std::runtime_error("Signal entry overruns header boundary");
            }

            std::string sig_name = read_string(offset, name_len);
            offset += name_len;

            uint16_t address = read_uint16(offset);
            offset += sizeof(uint16_t);

            uint8_t io_id = read_byte(offset++);
            uint8_t type_id = read_byte(offset++);

            signals_.push_back(SignalEntry{ sig_name, address, io_id == 0, type_id });

            std::cout << "Parsed Signal: name=\"" << sig_name
                << "\", address=" << address
                << ", direction=" << (io_id == 0 ? "Input" : "Output")
                << ", type_id=" << static_cast<int>(type_id) << "\n";
        }

        // Sanity check
        if (offset < header_->header_size) {
            std::cout << "Warning: Header not fully parsed; possible padding.\n";
        }
        else if (offset > header_->header_size) {
            std::cout << "Warning: Parsed past declared header size; check header consistency!\n";
        }
    }

    const ShmHeaderFixed* header() const { return header_; }
    const std::string& mutex_name() const { return mutex_name_; }
    const std::vector<SignalEntry>& signals() const { return signals_; }

private:
    uint8_t* shm_ptr_;
    size_t shm_size_;
    ShmHeaderFixed* header_;
    std::string mutex_name_;
    std::vector<SignalEntry> signals_;

    uint8_t read_byte(size_t offset) const {
        if (offset >= shm_size_) throw std::out_of_range("read_byte out of bounds");
        return *(shm_ptr_ + offset);
    }

    uint16_t read_uint16(size_t offset) const {
        if (offset + 2 > shm_size_) throw std::out_of_range("read_uint16 out of bounds");
        uint16_t val;
        std::memcpy(&val, shm_ptr_ + offset, sizeof(val));
        return val;
    }

    std::string read_string(size_t offset, size_t len) const {
        if (offset + len > shm_size_) throw std::out_of_range("read_string out of bounds");
        return std::string(reinterpret_cast<char*>(shm_ptr_ + offset), len);
    }

    void print_fixed_header() const {
        std::cout << "Total SHM size: " << header_->total_size << "\n";
        std::cout << "Header size: " << header_->header_size << "\n";
        std::cout << "Version: " << header_->version << "\n";
        std::cout << "Sampling time: " << header_->sampling_time_ms << " ms\n";
    }
};
#pragma once
