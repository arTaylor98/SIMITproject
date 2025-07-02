#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

inline std::vector<uint8_t> createMockShmData() {
    std::vector<uint8_t> shm;

    auto append = [&](auto val) {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
        shm.insert(shm.end(), ptr, ptr + sizeof(val));
        };

    uint32_t total_size = 1024;
    uint32_t header_size = 50;
    uint16_t version = 0;
    uint32_t sampling_time = 100;
    uint16_t cycle_counter = 0;
    uint8_t mutex_len = 5;

    append(total_size);
    append(header_size);
    append(version);
    append(sampling_time);
    append(cycle_counter);
    append(mutex_len);

    const char* mutex_name = "Mutex";
    shm.insert(shm.end(), mutex_name, mutex_name + mutex_len);

    uint8_t sig1_name_len = 6;
    const char* sig1_name = "StartC";
    uint16_t sig1_addr = 0;
    uint8_t sig1_io = 0;
    uint8_t sig1_type = 0;

    shm.push_back(sig1_name_len);
    shm.insert(shm.end(), sig1_name, sig1_name + sig1_name_len);
    append(sig1_addr);
    shm.push_back(sig1_io);
    shm.push_back(sig1_type);

    uint8_t sig2_name_len = 5;
    const char* sig2_name = "Speed";
    uint16_t sig2_addr = 2;
    uint8_t sig2_io = 0;
    uint8_t sig2_type = 13;

    shm.push_back(sig2_name_len);
    shm.insert(shm.end(), sig2_name, sig2_name + sig2_name_len);
    append(sig2_addr);
    shm.push_back(sig2_io);
    shm.push_back(sig2_type);

    shm.push_back(0);

    while (shm.size() < header_size) {
        shm.push_back(0);
    }

    return shm;
}
