#ifndef SPL_L1_STORAGE_H
#define SPL_L1_STORAGE_H

#include <stdint-gcc.h>
#include <unordered_map>

namespace spl {
namespace l1 {

class storage {
public:
    typedef uint64_t addr_t;
    typedef uint64_t data_t;
    uint64_t get(const addr_t l) { return store[l]; }
    bool set(const addr_t l,const data_t n) {
        return store.insert(std::pair<addr_t, data_t>(l,n)).second;
    }
private:
    std::unordered_map<addr_t, data_t> store;
};

}
}

#endif //SPL_L1_STORAGE_H
