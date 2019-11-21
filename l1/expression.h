#ifndef SPL_L1_EXPRESSION_H
#define SPL_L1_EXPRESSION_H

#include "spl/l1/storage.h"

namespace spl {
namespace l1 {

class expression {
public:
    typedef const std::shared_ptr<const expression> nome_ptr_pubblico;
    [[nodiscard]] virtual bool is_value() const { return false; }
    [[nodiscard]] virtual bool is_operation() const { return false; }
    [[nodiscard]] virtual bool is_location() const { return false; }
    [[nodiscard]] virtual nome_ptr_pubblico reduce(storage& s) const = 0;
protected:
    typedef const std::shared_ptr<const expression> nome_ptr_protetto;
};

}
}

#endif //SPL_L1_EXPRESSION_H
