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
            TX<Emitter> tx;
        }


        

        template<typename emitter_t>
        struct TX {

            TX(){

                checkSignal<eTestSigType::eRef>(true);

                checkSignal<eTestSigType::eCopy>(false);

                checkSignal<eTestSigType::eConstRef>(false);

                checkSignal<eTestSigType::eConstCopy>(false);

            }

            template<eTestSigType val>
            void checkSignal(const bool inPingState) {

                // initialize receive states
                RX_var<eTestSigType::eRef>::value = false;
                RX_var<eTestSigType::eCopy>::value = false;
                RX_var<eTestSigType::eConstRef>::value = false;
                RX_var<eTestSigType::eConstCopy>::value = false;

                bool ping = false;

                // instatiate signal
                PingSignal<val> s(ping);

                // send signal
                emitter_t::pEmit(s);

                // check states
                CPPUNIT_ASSERT(ping == inPingState);

                CPPUNIT_ASSERT((val == eTestSigType::eRef       && RX_var<val>::value) || !RX_var<val>::value);

                CPPUNIT_ASSERT((val == eTestSigType::eCopy      && RX_var<val>::value) || !RX_var<val>::value);

                CPPUNIT_ASSERT((val == eTestSigType::eConstRef  && RX_var<val>::value) || !RX_var<val>::value);

                CPPUNIT_ASSERT((val == eTestSigType::eConstCopy && RX_var<val>::value) || !RX_var<val>::value);
            }

        };
        
};
