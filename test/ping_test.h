#pragma once


#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>

#include "poulpe_instance.h"


class PingTest : public CppUnit::TestFixture { 
    
    CPPUNIT_TEST_SUITE(PingTest);
    CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();

    public:
    
        void setUp(){}

        void tearDown(){}

    protected: 

        void run(){
            bool ping = false;
            TX<Emitter> tx(ping);
            CPPUNIT_ASSERT(ping);
        }
        

        template<typename emitter_t>
        struct TX {
            TX(bool& r){
                PingSignal s(r);
                emitter_t::pEmit(s);
            }
        };
        
};
