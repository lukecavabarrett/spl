#ifndef SPL_L1_EXPRESSION_H
#define SPL_L1_EXPRESSION_H

#include "spl/l1/storage.h"

namespace spl {
namespace l1 {

class expression {
public:
    typedef const std::shared_ptr<const expression> pointer;
    typedef pointer ptr;
    [[nodiscard]] virtual bool is_value() const { return false; }
    [[nodiscard]] virtual bool is_operation() const { return false; }
    [[nodiscard]] virtual bool is_location() const { return false; }
    [[nodiscard]] virtual pointer reduce(storage& s) const = 0;
};

}
}

#endif //SPL_L1_EXPRESSION_H
