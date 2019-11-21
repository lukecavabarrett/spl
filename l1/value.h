#ifndef SPL_L1_VALUE_H
#define SPL_L1_VALUE_H

#include "spl/l1/expression.h"

namespace spl {
namespace l1 {

class value : public expression {
public:
    [[nodiscard]] bool is_value() const final {return true;}
    [[nodiscard]] virtual bool is_natural() const {return false;}
    [[nodiscard]] virtual bool is_boolean() const {return false;}
    [[nodiscard]] virtual bool is_unit() const {return false;}
};

class unit : public value {
public:
    typedef std::shared_ptr<const unit> ptr;
    [[nodiscard]] bool is_unit() const final {return true;}
    explicit unit() {}
    [[nodiscard]] nome_ptr_protetto reduce(storage& s) const final {
        return std::make_shared<const unit>();
    }
    static ptr dyn_cast(nome_ptr_protetto x){return std::dynamic_pointer_cast<const unit>(x);}
private:
};

expression::nome_ptr_pubblico make_unit(){
    return std::make_shared<const unit>();
}

}
}

#endif //SPL_L1_VALUE_H
