#ifndef SPL_L1_NATURAL_H
#define SPL_L1_NATURAL_H

#include <stdint-gcc.h>
#include "spl/l1/value.h"

namespace spl {
namespace l1 {
class natural : public value {
public:
    typedef std::shared_ptr<const natural> ptr;
    typedef uint64_t data_t;
    [[nodiscard]] bool is_natural() const final { return true; }
    explicit natural(const data_t d) : data(d) {}
    [[nodiscard]] data_t tp_cpp_type() const { return data; }
    [[nodiscard]] nome_ptr_protetto reduce(storage& s) const final {
        return std::make_shared<const natural>(data);
    }
    static ptr dyn_cast(nome_ptr_protetto x){return std::dynamic_pointer_cast<const natural>(x);}
private:
    const data_t data;
};


expression::nome_ptr_pubblico make_natural(natural::data_t d){
    return std::make_shared<const natural>(d);
}

}
}

#endif //SPL_L1_NATURAL_H
