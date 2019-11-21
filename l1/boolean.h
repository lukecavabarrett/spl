#ifndef SPL_L1_BOOLEAN_H
#define SPL_L1_BOOLEAN_H

#include <stdint-gcc.h>
#include "spl/l1/value.h"

namespace spl {
namespace l1 {

class boolean : public value {
public:
    typedef std::shared_ptr<const boolean> ptr;
    [[nodiscard]] bool is_boolean() const final {return true;}
    explicit boolean(const bool d) : data(d) {}
    [[nodiscard]] bool tp_cpp_type() const {return data;}

    [[nodiscard]] nome_ptr_protetto reduce(storage& s) const final {
        return std::make_shared<const boolean>(data);
    }
    static ptr dyn_cast(nome_ptr_protetto x){return std::dynamic_pointer_cast<const boolean>(x);}
private:
    const bool data;
};


inline expression::nome_ptr_pubblico make_boolean(const bool d){
    return std::make_shared<const boolean>(d);
}


}
}

#endif //SPL_L1_BOOLEAN_H
