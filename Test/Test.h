#include <iostream>
#include <vector>
#include <functional>

#ifndef TEST_API
#define TEST_API

class TestAPI {

    public: /********* Public members *********/
    
    /**
     * Default constructor
     */
    TestAPI();
    
    ~TestAPI();

    /**
     * Test printing function
     */
    static void TEST(const std::string&);

    /**
     * Passes if condition is true
     */
    static void ASSERT(const bool&);

};

#endif