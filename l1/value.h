#ifndef SPL_L1_VALUE_H
#define SPL_L1_VALUE_H

#include "spl/l1/expression.h"

namespace spl {
namespace l1 {

class value : public expression {
public:
    typedef std::shared_ptr<const value> ptr;
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
    [[nodiscard]] pointer reduce(storage& s) const final {
        return std::make_shared<const unit>();
    }
    static ptr dyn_cast(pointer x){return std::dynamic_pointer_cast<const unit>(x);}
    [[nodiscard]] virtual std::string to_string() const final { return "()"; }

private:
};

expression::ptr make_unit(){
    return std::make_shared<const unit>();
}

}
}

#endif //SPL_L1_VALUE_H
