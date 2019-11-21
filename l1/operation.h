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
    binary_operation(nome_ptr_protetto expr1, nome_ptr_protetto expr2) : e1(expr1), e2(expr2) {}
    nome_ptr_protetto lhs() { return e1; }
    nome_ptr_protetto rhs() { return e2; }

private:
protected:
    const nome_ptr_protetto e1;
    const nome_ptr_protetto e2;
};

class plus : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_plus() const final { return true; }
    [[nodiscard]] nome_ptr_protetto reduce(storage &s) const final {
        nome_ptr_protetto r1 = e1->reduce(s);
        if (std::shared_ptr<const natural> n1 = std::dynamic_pointer_cast<const natural>(r1)) {
            nome_ptr_protetto r2 = e2->reduce(s);
            if (std::shared_ptr<const natural> n2 = std::dynamic_pointer_cast<const natural>(r2)) {
                return make_natural(n1->tp_cpp_type() + n2->tp_cpp_type());
            } else return std::make_shared<plus>(r1, r2);
        } else return std::make_shared<plus>(r1, e2);
    }
};

inline expression::nome_ptr_pubblico make_plus(expression::nome_ptr_pubblico expr1, expression::nome_ptr_pubblico expr2) {
    return std::make_shared<const plus>(expr1, expr2);
}

class less : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_less() const final { return true; }
    [[nodiscard]] nome_ptr_protetto reduce(storage &s) const final {
        nome_ptr_protetto r1 = e1->reduce(s);
        if (std::shared_ptr<const natural> n1 = std::dynamic_pointer_cast<const natural>(r1)) {
            nome_ptr_protetto r2 = e2->reduce(s);
            if (std::shared_ptr<const natural> n2 = std::dynamic_pointer_cast<const natural>(r2)) {
                return make_boolean(n1->tp_cpp_type() < n2->tp_cpp_type());
            } else return std::make_shared<plus>(r1, r2);
        } else return std::make_shared<plus>(r1, e2);
    }
};

inline expression::nome_ptr_pubblico make_less(expression::nome_ptr_pubblico expr1, expression::nome_ptr_pubblico expr2) {
    return std::make_shared<const less>(expr1, expr2);
}

class assignment : public binary_operation {
public:
    [[nodiscard]] bool is_assignment() const final { return true; }
};

class unary_operation : public operation {
public:
    unary_operation(nome_ptr_protetto expr) : e(expr) {}
    nome_ptr_protetto arg() { return e; }

private:
    const nome_ptr_protetto e;
};

class dereference : public unary_operation {
public:
    [[nodiscard]] bool is_dereference() const final { return true; }
};

}
}

#endif //SPL_L1_OPERATION_H
