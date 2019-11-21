#ifndef SPL_L1_LOCATION_H
#define SPL_L1_LOCATION_H


#include <cstdint>
#include "spl/l1/expression.h"
#include "spl/l1/storage.h"

namespace spl {
namespace l1 {

class location : public expression {
public:
    typedef storage::addr_t addr_t;
    location(const addr_t d) : data(d) {};
    [[nodiscard]] bool is_location() const final {return true;}
private:
    const addr_t data;
};

}
}

#endif //SPL_L1_LOCATION_H
