#include "esp_log.h"
#include "esp_check.h"
#include "esp_ldo_regulator.h"

#include "dev_display_lcd.h"


#if __has_include(<esp_lcd_ek79007.h>)
#define HAS_EK79007 1
#include "esp_lcd_ek79007.h"
#endif

#if __has_include(<esp_lcd_touch_gt911.h>)
#define HAS_GT911 1
#include "esp_lcd_touch_gt911.h"
#endif

static const char *TAG = "CROWPANEL_SETUP";

/*
 * The working CrowPanel firmware also enables LDO4 at 3.3 V.
 *
 * LDO3 = MIPI PHY 2.5 V
 * LDO4 = 3.3 V peripheral/touch rail
 */
static esp_ldo_channel_handle_t s_ldo_peripheral = NULL;

static esp_err_t crowpanel_power_init(void)
{
    if (s_ldo_peripheral != NULL) {
        return ESP_OK;
    }

    const esp_ldo_channel_config_t config = {
        .chan_id = 4,
        .voltage_mv = 3300,
    };

    return esp_ldo_acquire_channel(
        &config,
        &s_ldo_peripheral
    );
}


#if defined(HAS_EK79007)

__attribute__((weak))
esp_err_t lcd_dsi_panel_factory_entry_t(
    esp_lcd_dsi_bus_handle_t dsi_handle,
    dev_display_lcd_config_t *lcd_cfg,
    dev_display_lcd_handles_t *lcd_handles)
{
    /*
     * The CrowPanel requires its 3.3 V peripheral rail as well as
     * the 2.5 V MIPI PHY rail.
     */
    ESP_RETURN_ON_ERROR(
        crowpanel_power_init(),
        TAG,
        "failed to initialize LDO4"
    );

    ek79007_vendor_config_t vendor_config = {
        .mipi_config = {
            .dsi_bus = dsi_handle,
            .dpi_config = &lcd_cfg->sub_cfg.dsi.dpi_config,
        },
    };

    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num =
            lcd_cfg->sub_cfg.dsi.reset_gpio_num,

        .rgb_ele_order =
            lcd_cfg->rgb_ele_order,

        .data_endian =
            lcd_cfg->data_endian,

        .bits_per_pixel =
            lcd_cfg->bits_per_pixel,

        .flags = {
            .reset_active_high =
                lcd_cfg->sub_cfg.dsi.reset_active_high,
        },

        .vendor_config = &vendor_config,
    };

    ESP_LOGI(
        TAG,
        "Installing CrowPanel EK79007 LCD driver"
    );

    return esp_lcd_new_panel_ek79007(
        lcd_handles->io_handle,
        &panel_config,
        &lcd_handles->panel_handle
    );
}

#endif


#if defined(HAS_GT911)

__attribute__((weak))
esp_err_t lcd_touch_factory_entry_t(
    esp_lcd_panel_io_handle_t io,
    const esp_lcd_touch_config_t *touch_dev_config,
    esp_lcd_touch_handle_t *ret_touch)
{
    return esp_lcd_touch_new_i2c_gt911(
        io,
        touch_dev_config,
        ret_touch
    );
}

#endif
