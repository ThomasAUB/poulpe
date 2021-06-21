#pragma once


#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>

#include "poulpe_instance.h"

bool PingReceiver::sRX = false;

class PingTest : public CppUnit::TestFixture { 
    
    CPPUNIT_TEST_SUITE(PingTest);
    CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();

    public:
    
        void setUp(){}

        void tearDown(){}

    protected: 

        void run(){
            TX<Emitter> tx;
        }
        

        template<typename emitter_t>
        struct TX {

            TX(){

                bool ping = false;

                PingReceiver::sRX = false;

                PingSignal<eTestSigType::eRef> s1(ping);
                emitter_t::pEmit(s1);

                CPPUNIT_ASSERT(ping);
                CPPUNIT_ASSERT(PingReceiver::sRX);

                PingReceiver::sRX = false;

                emitter_t::template pEmit<const PingSignal<eTestSigType::eConstRef>&>(PingSignal<eTestSigType::eConstRef>(ping));

                CPPUNIT_ASSERT(ping);
                CPPUNIT_ASSERT(PingReceiver::sRX);

                PingReceiver::sRX = false;

                PingSignal<eTestSigType::eCopy> s3(ping);
                emitter_t::pEmit(s3);

                CPPUNIT_ASSERT(ping);
                CPPUNIT_ASSERT(PingReceiver::sRX);

                PingReceiver::sRX = false;

                emitter_t::template pEmit<const PingSignal<eTestSigType::eConstCopy>>(PingSignal<eTestSigType::eConstCopy>(ping));

                CPPUNIT_ASSERT(ping);
                CPPUNIT_ASSERT(PingReceiver::sRX);

            }
        };
        
};
