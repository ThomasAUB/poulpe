#pragma once


#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>

#include "poulpe_instance.h"

class CounterTest : public CppUnit::TestFixture { 
    
    CPPUNIT_TEST_SUITE(PingTest);
    CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();

    public:
    
        void setUp(){}

        void tearDown(){}

    protected: 

        struct DummySignal {};
        struct RX { void pReceive(const DummySignal& s) {} };

        DEFINE_RECEIVERS(RX);

        RX mRX;

        CREATE_POULPE(mRX);

        void run(){
            CPPUNIT_ASSERT(Emitter::getReceiverCount<DummySignal>() == 1);
        }


};
