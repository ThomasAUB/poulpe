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

        void run(){
            TX<Emitter> tx;
        }
        

        template<typename emitter_t>
        struct TX {

            void init() {
            // initialize receive states
            RX_var<eTestSigType::eRef>::value = false;
            RX_var<eTestSigType::eCopy>::value = false;
            RX_var<eTestSigType::eConstRef>::value = false;
            RX_var<eTestSigType::eConstCopy>::value = false;
            }

            template<typename signal_t, eTestSigType t>
            void checkSignal(bool inPingState) {
                init();
                bool ping = false;
                signal_t s(ping);
                emitter_t::pEmit(s);
                CPPUNIT_ASSERT(RX_var<t>::value);
                CPPUNIT_ASSERT(ping == inPingState);
            }

            TX(){

                checkSignal<PingSignal<eTestSigType::eRef>, eTestSigType::eRef>(true);
                checkSignal<PingSignal<eTestSigType::eCopy>, eTestSigType::eCopy>(true);

                checkSignal<PingSignal<eTestSigType::eConstRef>, eTestSigType::eConstRef>(false);
                checkSignal<PingSignal<eTestSigType::eConstCopy>, eTestSigType::eConstCopy>(false);

                checkSignal<const PingSignal<eTestSigType::eConstRef>, eTestSigType::eConstRef>(false);
                checkSignal<const PingSignal<eTestSigType::eConstCopy>, eTestSigType::eConstCopy>(false);
            }

        };

};
