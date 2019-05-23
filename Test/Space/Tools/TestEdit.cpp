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
/**
 * -------------------------------------------------------------------------------------------------
 * echo Spliter
 * -------------------------------------------------------------------------------------------------
 */
TEST(SModule, EchoSpliter)
{
    STask::Enable();
    // define types -----------------------------------------------------------
    using Spliter  = Module::SSpliterModule<
        Decoded::IOConnector, Decoded::IConnector, Decoded::OConnector
    >;
    using Monitor = SResourceMonitor<
    >;
    // settings ---------------------------------------------------------------
    auto size   = 100;
    auto addr   = "127.0.0.1";
    auto port1  = 9999;
    auto port2  = 8888;

    // build a spliter -------------------------------------------------------- 
    Spliter s ({
        {Spliter::Command::MODULE,   {{
            {Module::TIMEOUT, "200"},
            {Module::IO::VERBOSE, "0"}
        }}},
        {Spliter::Command::FUNCTION, {{
            {Module::Function::TYPE, Module::Function::Type::MESSAGE},
            {Module::IO::VERBOSE, "0"}
        }}},
        {Spliter::Command::INOUT,    {{
            {Module::IO::MINIMUM, "0"}
        }, {
            {Module::IO::TYPE, Module::IO::Type::MESSAGE_REMOTE},
            {Module::IO::URI, SText(addr, ":", port1)},
            {Module::IO::VERBOSE, "0"}
        }}},
        {Spliter::Command::INPUT,    {{
            {Module::IO::MINIMUM, "1"}
        }, {
            {Module::IO::TYPE, Module::IO::Type::STREAM_REMOTE},
            {Module::IO::URI, SText(addr, ":", port2)},
            {Module::IO::VERBOSE, "0"}
        }}},
        {Spliter::Command::OUTPUT,   {{
            {Module::IO::MINIMUM, "0"}
        }, {
            {Module::IO::TYPE, Module::IO::Type::STREAM_REMOTE},
            {Module::IO::URI, SText(addr, ":", port2)},
            {Module::IO::VERBOSE, "0"}
        }}}
    });
    s.start();
    // build a test frames ---------------------------------------------------- 
    auto const in  = SRandom::Frame(size);
    auto       out = IOFrame(size);

    // interface resource -----------------------------------------------------
    auto interface = Message::SRemoteResource()
        .link(addr, port1)
    .detach();

    // wait -------------------------------------------------------------------
    EXPECT_EQ(s.state_wait(Spliter::Time(3000), Spliter::PLAY), true);

    // send ------------------------------------------------------------------- 
    EXPECT_EQ(interface.write(in).good(), true);

    // wait ------------------------------------------------------------------- 
    Monitor(Monitor::Time(3000), &interface).wait();

    // receive ----------------------------------------------------------------
    EXPECT_EQ(interface.read(out).good(), true);

    // test data --------------------------------------------------------------
    EXPECT_EQ(in, out);

    // end module -------------------------------------------------------------
    s.stop();
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */

