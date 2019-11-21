#ifndef SPL_L1_OPERATION_H
#define SPL_L1_OPERATION_H

#include "spl/l1/expression.h"

namespace spl {
namespace l1 {

class operation : public expression {
public:
    [[nodiscard]] bool is_operation() const final { return true; }
    [[nodiscard]] virtual bool is_plus() const { return false; }
    [[nodiscard]] virtual bool is_less() const { return false; }
    [[nodiscard]] virtual bool is_dereference() const { return false; }
    [[nodiscard]] virtual bool is_assignment() const { return false; }
};

class binary_operation : public operation {
public:
    binary_operation(ptr expr1, ptr expr2) : e1(expr1), e2(expr2) {}
    ptr lhs() { return e1; }
    ptr rhs() { return e2; }

private:
protected:
    const ptr e1;
    const ptr e2;
};

class plus : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_plus() const final { return true; }
    [[nodiscard]] ptr reduce(storage &s) const final {
        ptr r1 = e1->reduce(s);
        if (std::shared_ptr<const natural> n1 = std::dynamic_pointer_cast<const natural>(r1)) {
            ptr r2 = e2->reduce(s);
            if (std::shared_ptr<const natural> n2 = std::dynamic_pointer_cast<const natural>(r2)) {
                return make_natural(n1->tp_cpp_type() + n2->tp_cpp_type());
            } else return std::make_shared<plus>(r1, r2);
        } else return std::make_shared<plus>(r1, e2);
    }
};

inline expression::ptr make_plus(expression::ptr expr1, expression::ptr expr2) {
    return std::make_shared<const plus>(expr1, expr2);
}

class less : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_less() const final { return true; }
    [[nodiscard]] ptr reduce(storage &s) const final {
        ptr r1 = e1->reduce(s);
        if (std::shared_ptr<const natural> n1 = std::dynamic_pointer_cast<const natural>(r1)) {
            ptr r2 = e2->reduce(s);
            if (std::shared_ptr<const natural> n2 = std::dynamic_pointer_cast<const natural>(r2)) {
                return make_boolean(n1->tp_cpp_type() < n2->tp_cpp_type());
            } else return std::make_shared<plus>(r1, r2);
        } else return std::make_shared<plus>(r1, e2);
    }
};

inline expression::ptr make_less(expression::ptr expr1, expression::ptr expr2) {
    return std::make_shared<const less>(expr1, expr2);
}

class assignment : public binary_operation {
public:
    [[nodiscard]] bool is_assignment() const final { return true; }
};

class unary_operation : public operation {
public:
    unary_operation(ptr expr) : e(expr) {}
    ptr arg() { return e; }

private:
    const ptr e;
};

class dereference : public unary_operation {
public:
    [[nodiscard]] bool is_dereference() const final { return true; }
};

}
}

#endif //SPL_L1_OPERATION_H
