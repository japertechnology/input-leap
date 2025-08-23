#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define private public
#include "client/ServerProxy.h"
#include "test/global/TestEventQueue.h"
#undef private

#include "client/Client.h"
#include "inputleap/ClientArgs.h"
#include "net/NetworkAddress.h"
#include "net/ISocketFactory.h"
#include "test/mock/io/MockStream.h"
#include "test/mock/inputleap/MockScreen.h"
#include "inputleap/protocol_types.h"
#include <cstring>

using namespace testing;

namespace inputleap {

class DummySocketFactory : public ISocketFactory {
public:
    std::unique_ptr<IDataSocket> create(IArchNetwork::EAddressFamily, ConnectionSecurityLevel) const override { return {}; }
    std::unique_ptr<IListenSocket> create_listen(IArchNetwork::EAddressFamily, ConnectionSecurityLevel) const override { return {}; }
};

class ServerProxyTest : public ::testing::Test {
protected:
    TestEventQueue events_;
    MockStream stream_;
    MockScreen screen_;
    DummySocketFactory* socketFactory_ = nullptr;
    Client* client_ = nullptr;
    ServerProxy* proxy_ = nullptr;

    void SetUp() override {
        events_.is_ready_ = true;
        EXPECT_CALL(stream_, get_event_target()).WillRepeatedly(Return(&stream_));
        socketFactory_ = new DummySocketFactory();
        ClientArgs args;
        client_ = new Client(&events_, "test", NetworkAddress(), socketFactory_, &screen_, args);
        client_->m_mock = true;
        proxy_ = new ServerProxy(client_, &stream_, &events_);
        proxy_->m_parser = &ServerProxy::parseMessage;
    }

    void TearDown() override {
        delete proxy_;
        delete client_;
        delete socketFactory_;
    }
};

TEST_F(ServerProxyTest, InvalidMessageDisconnects) {
    uint8_t code[4] = {'X', 'X', 'X', 'X'};
    EXPECT_CALL(stream_, read(_, 4))
        .WillOnce(DoAll(SetArrayArgument<0>(code, code + 4), Return(4)));

    std::string reason;
    events_.add_handler(EventType::CLIENT_CONNECTION_FAILED, client_->get_event_target(),
                         [&reason](const auto& e) { reason = e.get_data_as<Client::FailInfo>().m_what; });

    proxy_->handle_data();

    Event ev;
    ASSERT_TRUE(events_.getEvent(ev, 0.0));
    events_.dispatchEvent(ev);

    EXPECT_EQ("invalid message from server", proxy_->m_disconnectReason);
    EXPECT_EQ("invalid message from server", reason);
}

TEST_F(ServerProxyTest, ProtocolErrorMessageDisconnects) {
    uint8_t code[4];
    memcpy(code, kMsgEBad, 4);
    EXPECT_CALL(stream_, read(_, 4))
        .WillOnce(DoAll(SetArrayArgument<0>(code, code + 4), Return(4)));

    std::string reason;
    events_.add_handler(EventType::CLIENT_CONNECTION_FAILED, client_->get_event_target(),
                         [&reason](const auto& e) { reason = e.get_data_as<Client::FailInfo>().m_what; });

    proxy_->handle_data();

    Event ev;
    ASSERT_TRUE(events_.getEvent(ev, 0.0));
    events_.dispatchEvent(ev);

    EXPECT_EQ("server reported a protocol error", proxy_->m_disconnectReason);
    EXPECT_EQ("server reported a protocol error", reason);
}

} // namespace inputleap

