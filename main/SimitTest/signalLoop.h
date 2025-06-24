#pragma once

#include <string>
#include <cstdint>

class HeaderArea {
private:
    uint32_t memoryAreaSize;
    uint32_t headerAreaSize;
    uint16_t version;
    uint32_t scanningCycleMs;
    uint16_t cycleCounter;
    uint8_t mutexNameLength;
    std::string mutexName;

public:
    // Getters
    uint32_t getMemoryAreaSize() const;
    uint32_t getHeaderAreaSize() const;
    uint16_t getVersion() const;
    uint32_t getScanningCycleMs() const;
    uint16_t getCycleCounter() const;
    uint8_t getMutexNameLength() const;
    const std::string& getMutexName() const;

    // Setters
    void setMemoryAreaSize(uint32_t size);
    void setHeaderAreaSize(uint32_t size);
    void setVersion(uint16_t ver);
    void setScanningCycleMs(uint32_t ms);
    void setCycleCounter(uint16_t counter);
    void setMutexNameLength(uint8_t length);
    void setMutexName(const std::string& name);
};
