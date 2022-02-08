#pragma once


#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>

class CounterTest : public CppUnit::TestFixture { 
    
    CPPUNIT_TEST_SUITE(PingTest);
    CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();

    struct DummySignal {};
    struct RX { void pReceive(const DummySignal& s) {} };

    DEFINE_RECEIVERS(RX);

    RX mRX;

    CREATE_POULPE(mRX);

    public:
    
        void setUp(){}

        void tearDown(){}

        void run(){
            CPPUNIT_ASSERT(Emitter::getReceiverCount<DummySignal>() == 1);
        }
  
};
