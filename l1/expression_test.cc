#include <memory>
#include "gtest/gtest.h"
#include "expression.h"
#include "spl/l1/boolean.h"
#include "spl/l1/natural.h"
#include "spl/l1/operation.h"

namespace spl {
namespace l1 {
namespace {

TEST(SharedPtr, Casting) {
    expression::nome_ptr_pubblico e = std::make_shared<natural>(42);
    EXPECT_TRUE(e->is_value());
    std::shared_ptr<const value> v = std::dynamic_pointer_cast<const value>(e);
    EXPECT_FALSE(v->is_boolean());
    EXPECT_TRUE(v->is_natural());
    std::shared_ptr<const natural> n = std::dynamic_pointer_cast<const natural>(v);
    EXPECT_EQ(n->tp_cpp_type(), 42);
}

TEST(Expression,Reduce){
    expression::nome_ptr_pubblico e = make_plus(make_natural(100), make_natural(1));
    storage s;
    expression::nome_ptr_pubblico e_star = e->reduce(s);
    EXPECT_TRUE(e_star->is_value());

}

}
}
}
