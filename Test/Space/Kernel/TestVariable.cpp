/**
 * ------------------------------------------------------------------------------------------------
 * File:   TestSpliter.cpp
 * Author: Luis Monteiro
 *
 * Created on February  2, 2019, 10:21 AM
 * ------------------------------------------------------------------------------------------------
 */
#include <gtest/gtest.h>
/**
 * space
 */
#include "SEdit.h"
#include "SLog.h"
/**
 * -------------------------------------------------------------------------------------------------
 * Variable - Throw
 * -------------------------------------------------------------------------------------------------
 */
TEST(SVariable, Throw)
{ 
    // settings ---------------------------------------------------------------
    Var var = Obj{
        {Key("a"), Obj{
            {Key("a"), Obj("a")},
            {Key("b"), Obj(1)},
            {Key("c"), Obj(3.5)}
        }},
        {Key("b"), Obj{
            Obj(1),
            Obj(2),
            Obj(3)
        }},
    };  
    // test 1 -----------------------------------------------------------------
    EXPECT_EQ(Var::Size(var),      2);
    EXPECT_EQ(Var::Size(var["b"]), 3);
    
    // test 2 -----------------------------------------------------------------
    EXPECT_EQ(Var::IsEmpty(var),      false);
    EXPECT_EQ(Var::IsEmpty(var["b"]), false);
    
    // test 3 -----------------------------------------------------------------
    try {
        Var::Throw(var);
    } catch(const Map& m) {
        EXPECT_TRUE(Edit::Match(Obj(m), var));
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
