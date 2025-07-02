#include "ShmHeaderParser.h"
#include "MockShmData.h"
#include <iostream>

int main() {
    auto shm_data = createMockShmData();
    std::cout << "Mock SHM buffer size: " << shm_data.size() << "\n";
    
    //Logging start
    if (!shm_data.data()) {
        std::cout << "Error: SHM data pointer is null!\n";
    }
    
    std::cout << "Mock SHM buffer address: " << static_cast<void*>(shm_data.data()) << "\n";
    std::cout << "sizeof(ShmHeaderFixed): " << sizeof(ShmHeaderFixed) << "\n";
    //loggging end

    ShmHeaderParser parser(shm_data.data(), shm_data.size());

    try {
        parser.parse();

        const auto* hdr = parser.header();
        std::cout << "Total SHM size: " << hdr->total_size << "\n";
        std::cout << "Mutex: " << parser.mutex_name() << "\n";

        for (const auto& sig : parser.signals()) {
            std::cout << "Signal: " << sig.name << ", addr=" << sig.address
                << ", dir=" << (sig.is_input ? "Input" : "Output")
                << ", type=" << static_cast<int>(sig.type_id) << "\n";
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error parsing mock SHM: " << ex.what() << "\n";
    }
    return 0;
}
