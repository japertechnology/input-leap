#include "config.h"
#if HAVE_LIBPORTAL_INPUTCAPTURE
#include "platform/PortalInputCapture.h"
#include <gtest/gtest.h>
#include <glib.h>
#include <string>

namespace inputleap {

TEST(PortalInputCaptureTests, NextBackoffIncreases)
{
    unsigned int delay = PortalInputCapture::next_backoff(1000);
    EXPECT_EQ(2000u, delay);
    delay = PortalInputCapture::next_backoff(30000);
    EXPECT_EQ(60000u, delay);
}

TEST(PortalInputCaptureTests, TransientDisableParsing)
{
    GVariantBuilder builder;
    g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);
    g_variant_builder_add(&builder, "{sv}", "reason", g_variant_new_string("timeout"));
    GVariant* opts = g_variant_builder_end(&builder);
    std::string reason;
    EXPECT_TRUE(PortalInputCapture::is_transient_disable(opts, reason));
    EXPECT_EQ("timeout", reason);
    g_variant_unref(opts);

    g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);
    g_variant_builder_add(&builder, "{sv}", "reason", g_variant_new_string("user"));
    opts = g_variant_builder_end(&builder);
    reason.clear();
    EXPECT_FALSE(PortalInputCapture::is_transient_disable(opts, reason));
    EXPECT_EQ("user", reason);
    g_variant_unref(opts);
}

} // namespace inputleap
#endif // HAVE_LIBPORTAL_INPUTCAPTURE
