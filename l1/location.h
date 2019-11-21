#ifndef SPL_L1_LOCATION_H
#define SPL_L1_LOCATION_H


#include <cstdint>
#include <memory>
#include "spl/l1/expression.h"
#include "spl/l1/storage.h"

namespace spl {
namespace l1 {

class location : public expression {
public:
    typedef std::shared_ptr<const location> ptr;
    typedef storage::addr_t addr_t;
    location(const addr_t d) : data(d) {};
    [[nodiscard]] bool is_location() const final { return true; }
    static ptr dyn_cast(pointer x) { return std::dynamic_pointer_cast<const location>(x); }
    [[nodiscard]] addr_t to_cpp_type() const { return data; }
    [[nodiscard]] pointer reduce(storage &s) const final { return std::make_shared<const location>(data); }

    [[nodiscard]] virtual std::string to_string() const final { return "0x" + std::to_string(data); }//TODO: fix
private:
    const addr_t data;
};

expression::ptr make_location(location::addr_t d) {
    return std::make_shared<const location>(d);
}

}
}

#endif //SPL_L1_LOCATION_H
