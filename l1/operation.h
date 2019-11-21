#ifndef SPL_L1_OPERATION_H
#define SPL_L1_OPERATION_H

#include "spl/l1/expression.h"
#include "spl/l1/location.h"

namespace spl {
namespace l1 {

class operation : public expression {
public:
    [[nodiscard]] bool is_operation() const final { return true; }
    [[nodiscard]] virtual bool is_plus() const { return false; }
    [[nodiscard]] virtual bool is_less() const { return false; }
    [[nodiscard]] virtual bool is_dereference() const { return false; }
    [[nodiscard]] virtual bool is_assignment() const { return false; }
    [[nodiscard]] virtual bool is_sequence() const { return false; }
};

class binary_operation : public operation {
public:
    binary_operation(pointer expr1, pointer expr2) : e1(expr1), e2(expr2) {}
    pointer lhs() { return e1; }
    pointer rhs() { return e2; }

private:
protected:
    const pointer e1;
    const pointer e2;
};

class plus : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_plus() const final { return true; }
    [[nodiscard]] pointer reduce(storage &s) const final {
        pointer r1 = e1->reduce(s);
        if (std::shared_ptr<const natural> n1 = std::dynamic_pointer_cast<const natural>(r1)) {
            pointer r2 = e2->reduce(s);
            if (std::shared_ptr<const natural> n2 = std::dynamic_pointer_cast<const natural>(r2)) {
                return make_natural(n1->to_cpp_type() + n2->to_cpp_type());
            } else return std::make_shared<const plus>(r1, r2);
        } else return std::make_shared<const plus>(r1, e2);
    }

    [[nodiscard]] virtual std::string to_string() const final { return e1->to_string()+" + "+e2->to_string(); }
};

inline expression::ptr make_plus(expression::ptr expr1, expression::ptr expr2) {
    return std::make_shared<const plus>(expr1, expr2);
}

class less : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_less() const final { return true; }
    [[nodiscard]] pointer reduce(storage &s) const final {
        pointer r1 = e1->reduce(s);
        if (std::shared_ptr<const natural> n1 = std::dynamic_pointer_cast<const natural>(r1)) {
            pointer r2 = e2->reduce(s);
            if (std::shared_ptr<const natural> n2 = std::dynamic_pointer_cast<const natural>(r2)) {
                return make_boolean(n1->to_cpp_type() < n2->to_cpp_type());
            } else return std::make_shared<plus>(r1, r2);
        } else return std::make_shared<plus>(r1, e2);
    }

    [[nodiscard]] virtual std::string to_string() const final { return e1->to_string()+" < "+e2->to_string(); }
};

inline expression::ptr make_less(expression::ptr expr1, expression::ptr expr2) {
    return std::make_shared<const less>(expr1, expr2);
}

class sequence : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_sequence() const final { return true; }
    [[nodiscard]] ptr reduce(storage &s) const final {
        pointer r1 = e1->reduce(s);
        if (unit::ptr u1 = unit::dyn_cast(r1)) {
            return e2->reduce(s);
        } else return std::make_shared<const sequence>(r1, e2);
    }

    [[nodiscard]] virtual std::string to_string() const final { return e1->to_string()+"; "+e2->to_string(); }
};

inline expression::ptr make_sequence(expression::ptr expr1, expression::ptr expr2) {
    return std::make_shared<const sequence>(expr1, expr2);
}

class assignment : public binary_operation {
public:
    using binary_operation::binary_operation;
    [[nodiscard]] bool is_assignment() const final { return true; }
    [[nodiscard]] ptr reduce(storage &s) const final {
        pointer r1 = e1->reduce(s);
        if(location::ptr l = location::dyn_cast(r1)){
            pointer r2 = e2->reduce(s);
            if(natural::ptr n = natural::dyn_cast(r2)){
                s.set(l->to_cpp_type(),n->to_cpp_type());
                return make_unit();
            } else return std::make_shared<const assignment>(r1, r2);
        } else return std::make_shared<const assignment>(r1, e2);
    }

    [[nodiscard]] virtual std::string to_string() const final { return e1->to_string()+" := "+e2->to_string(); }
};

inline expression::ptr make_assignment(expression::ptr expr1, expression::ptr expr2) {
    return std::make_shared<const assignment>(expr1, expr2);
}

class unary_operation : public operation {
public:
    unary_operation(pointer expr) : e(expr) {}
    pointer arg() { return e; }

protected:
    const pointer e;
};

class dereference : public unary_operation {
public:
    using unary_operation::unary_operation;
    [[nodiscard]] bool is_dereference() const final { return true; }
    [[nodiscard]] ptr reduce(storage &s) const final {
        pointer r = e->reduce(s);
        if (location::ptr l = location::dyn_cast(r)) {
            return make_natural(s.get(l->to_cpp_type()));
        } else return r;
    }

    [[nodiscard]] virtual std::string to_string() const final { return std::string("!")+e->to_string();}
};

inline expression::ptr make_dereference(expression::ptr expr) {
    return std::make_shared<const dereference>(expr);
}

}
}

#endif //SPL_L1_OPERATION_H
