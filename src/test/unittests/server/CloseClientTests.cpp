#define INPUTLEAP_TEST_ENV
#define NDEBUG

#include <gtest/gtest.h>

#include "server/Server.h"
#include "server/ClientProxy.h"
#include "server/IClientConnection.h"
#include "base/EventTarget.h"
#include "inputleap/Clipboard.h"
#include "inputleap/option_types.h"

namespace inputleap {

class DummyConnection : public IClientConnection {
public:
    const EventTarget* get_event_target() override { return nullptr; }
    IStream* get_stream() override { return nullptr; }
    void send_query_info_1_6() override {}
    void send_leave_1_6() override {}
    void send_enter_1_6(std::int32_t, std::int32_t, std::uint32_t, KeyModifierMask) override {}
    void send_key_down_1_6(KeyID, KeyModifierMask, KeyButton) override {}
    void send_key_up_1_6(KeyID, KeyModifierMask, KeyButton) override {}
    void send_key_repeat_1_6(KeyID, KeyModifierMask, std::int32_t, KeyButton) override {}
    void send_mouse_down_1_6(ButtonID) override {}
    void send_mouse_up_1_6(ButtonID) override {}
    void send_mouse_move_1_6(std::int32_t, std::int32_t) override {}
    void send_mouse_relative_move_1_6(std::int32_t, std::int32_t) override {}
    void send_mouse_wheel_1_6(std::int32_t, std::int32_t) override {}
    void send_drag_info_1_6(std::uint32_t, const std::string&) override {}
    void send_screensaver_1_6(bool) override {}
    void send_reset_options_1_6() override {}
    void send_set_options_1_6(const OptionsList&) override {}
    void send_info_ack_1_6() override {}
    void send_keep_alive_1_6() override {}
    void send_close_1_6(const char*) override {}
    void send_clipboard_chunk_1_6(const ClipboardChunk&) override {}
    void send_file_chunk_1_6(const FileChunk&) override {}
    void send_grab_clipboard(ClipboardID) override {}
    void flush() override {}
    void close() override {}
};

class DummyClientProxy : public ClientProxy {
public:
    DummyClientProxy(const std::string& name, std::unique_ptr<IClientConnection> backend)
        : ClientProxy(name, std::move(backend)) {}

    bool getClipboard(ClipboardID, IClipboard*) const override { return false; }
    void getShape(std::int32_t&, std::int32_t&, std::int32_t&, std::int32_t&) const override {}
    void getCursorPos(std::int32_t&, std::int32_t&) const override {}
    void enter(std::int32_t, std::int32_t, std::uint32_t, KeyModifierMask, bool) override {}
    bool leave() override { return true; }
    void setClipboard(ClipboardID, const IClipboard*) override {}
    void grabClipboard(ClipboardID) override {}
    void setClipboardDirty(ClipboardID, bool) override {}
    void keyDown(KeyID, KeyModifierMask, KeyButton) override {}
    void keyRepeat(KeyID, KeyModifierMask, std::int32_t, KeyButton) override {}
    void keyUp(KeyID, KeyModifierMask, KeyButton) override {}
    void mouseDown(ButtonID) override {}
    void mouseUp(ButtonID) override {}
    void mouseMove(std::int32_t, std::int32_t) override {}
    void mouseRelativeMove(std::int32_t, std::int32_t) override {}
    void mouseWheel(std::int32_t, std::int32_t) override {}
    void screensaver(bool) override {}
    void resetOptions() override {}
    void setOptions(const OptionsList&) override {}
    void sendDragInfo(std::uint32_t, const char*, size_t) override {}
    void file_chunk_sending(const FileChunk&) override {}
};

TEST(ServerCloseClientTests, handles_null_message)
{
    Server server;
    DummyClientProxy client("dummy", std::make_unique<DummyConnection>());
    EXPECT_NO_THROW(server.closeClient(&client, nullptr));
}

} // namespace inputleap

