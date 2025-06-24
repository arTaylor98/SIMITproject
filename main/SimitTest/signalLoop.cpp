#include "signalLoop.h"

// Getters
uint32_t HeaderArea::getMemoryAreaSize() const { return memoryAreaSize; }
uint32_t HeaderArea::getHeaderAreaSize() const { return headerAreaSize; }
uint16_t HeaderArea::getVersion() const { return version; }
uint32_t HeaderArea::getScanningCycleMs() const { return scanningCycleMs; }
uint16_t HeaderArea::getCycleCounter() const { return cycleCounter; }
uint8_t HeaderArea::getMutexNameLength() const { return mutexNameLength; }
const std::string& HeaderArea::getMutexName() const { return mutexName; }

// Setters
void HeaderArea::setMemoryAreaSize(uint32_t size) { memoryAreaSize = size; }
void HeaderArea::setHeaderAreaSize(uint32_t size) { headerAreaSize = size; }
void HeaderArea::setVersion(uint16_t ver) { version = ver; }
void HeaderArea::setScanningCycleMs(uint32_t ms) { scanningCycleMs = ms; }
void HeaderArea::setCycleCounter(uint16_t counter) { cycleCounter = counter; }
void HeaderArea::setMutexNameLength(uint8_t length) { mutexNameLength = length; }
void HeaderArea::setMutexName(const std::string& name) { mutexName = name; }