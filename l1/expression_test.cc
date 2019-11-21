#include <memory>
#include "gtest/gtest.h"
#include "expression.h"
#include "spl/l1/boolean.h"
#include "spl/l1/natural.h"
#include "spl/l1/operation.h"
#include "spl/l1/location.h"
#include "spl/l1/construct.h"

namespace spl {
namespace l1 {
namespace {

TEST(SharedPtr, Casting) {
    expression::ptr e = std::make_shared<natural>(42);
    EXPECT_TRUE(e->is_value());
    value::ptr v = std::dynamic_pointer_cast<const value>(e);
    EXPECT_FALSE(v->is_boolean());
    EXPECT_TRUE(v->is_natural());
    natural::ptr n = natural::dyn_cast(v);
    EXPECT_EQ(n->to_cpp_type(), 42);
}

TEST(Expression, Reduce) {
    expression::ptr e = make_plus(make_natural(100), make_natural(1));
    storage s;
    expression::ptr e_star = e->reduce(s);
    EXPECT_TRUE(e_star->is_value());
    natural::ptr n = natural::dyn_cast(e_star);
    EXPECT_TRUE(n);
    EXPECT_FALSE(boolean::dyn_cast(e_star));
    EXPECT_EQ(n->to_cpp_type(), 101);
}

TEST(Expression, ReduceMismatch) {
    expression::ptr e = make_plus(make_boolean(false), make_natural(1));
    storage s;
    expression::ptr e_star = e->reduce(s);
    EXPECT_FALSE(e_star->is_value());
    natural::ptr n = natural::dyn_cast(e_star);
    EXPECT_FALSE(n);
    EXPECT_FALSE(boolean::dyn_cast(e_star));
}

TEST(Expression, Assignment) {
    expression::ptr e = make_sequence(make_assignment(make_location(0xa), make_natural(42)),
                                      make_assignment(make_location(0xb),
                                                      make_plus(make_dereference(make_location(0xa)),
                                                                make_natural(7))));
    std::cout << e->to_string() << std::endl;
    storage s;
    expression::ptr e_star = e->reduce(s);
    std::cout << e_star->to_string() << std::endl;
    EXPECT_TRUE(unit::dyn_cast(e_star));
    EXPECT_EQ(s.get(0xa), 42);
    EXPECT_EQ(s.get(0xb), 49);
}

TEST(Selection, Simple) {
    expression::ptr e = make_selection(make_less(make_natural(3), make_natural(5)), make_natural(10),
                                       make_natural(100));
    std::cout << e->to_string() << std::endl;
    storage s;
    expression::ptr e_star = e->reduce(s);
    std::cout << e_star->to_string() << std::endl;
}

TEST(While, Simple) {
    expression::ptr e = make_loop(make_less(make_dereference(make_location(0x1)), make_natural(1)),
                                  make_sequence(make_assignment(make_location(0x0), make_plus(
                                          make_dereference(make_location(0x0)), make_dereference(make_location(0x0))
                                                )),
                                                make_assignment(make_location(0x1),
                                                                make_plus(make_dereference(make_location(0x1)),
                                                                          make_natural(1)))));
    std::cout << e->to_string() << std::endl;
    storage s;
    s.set(0x0, 1);
    s.set(0x1, 0);
    expression::ptr e_star = e->reduce(s);
    std::cout << e_star->to_string() << std::endl;
}


TEST(While, Fibonacci) {
    expression::ptr n = make_location(0x0), i = make_location(0x1), a = make_location(0x2), b = make_location(
            0x3), tmp = make_location(0x4);
    expression::ptr fib = make_sequence(
            make_sequence(
                    make_sequence(
                            make_assignment(i, make_natural(0)),
                            make_sequence(
                                    make_assignment(a, make_natural(0)),
                                    make_assignment(b, make_natural(1)))),
                    make_loop(make_less(make_dereference(i), make_dereference(n)),
                              make_sequence(make_sequence(make_assignment(tmp, make_dereference(a)),
                                                          make_assignment(a, make_dereference(b))), make_sequence(
                                      make_assignment(b, make_plus(make_dereference(b), make_dereference(tmp))),
                                      make_assignment(i, make_plus(make_dereference(i), make_natural(1))))))),
            make_sequence(make_assignment(n, make_dereference(a)), make_dereference(a)));
    std::cout << fib->to_string() << std::endl;
    storage s;
    s.set(0x0, 8);
    expression::ptr fib_star = fib->reduce(s);
    std::cout << std::endl << std::endl;
    std::cout << fib_star->to_string() << std::endl;
}

}
}
}
