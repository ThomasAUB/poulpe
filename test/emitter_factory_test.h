#pragma once


#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>


#include "poulpe_instance.h"


class EmitterFactoryTest : public CppUnit::TestFixture { 
    
    CPPUNIT_TEST_SUITE(EmitterFactoryTest);
    CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();

    public:
    
        void setUp(){}

        void tearDown(){}

    protected: 

        void run(){

            EmitterFactory<
                Emitter, 
                TX, 
                SignalTypes<PingSignal>
            > tx;

            bool ping = false;

            tx.transmit(ping);

            CPPUNIT_ASSERT(ping);

        }
        

        struct TX{
            void transmit(bool& r){
                PingSignal s(r);
                pEmit(s);
            }
            virtual void pEmit(PingSignal&) = 0;
        };
        
};
