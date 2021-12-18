#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "file_operations.hpp"
#include "csv_file.hpp"

static const char *TAG = "main";

#ifndef MOUNT_POINT
#define MOUNT_POINT "/sdcard"
#endif

void unmount_sdcard(sdmmc_card_t *card, char *mount_point)
{
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");
}

esp_err_t init_sdcard(sdmmc_card_t **card, char *mount_point)
{
    esp_err_t ret = ESP_FAIL;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};

    ESP_LOGI(TAG, "Initializing SD card");

    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // To use 1-line SD mode, change this to 1:
    slot_config.width = 4;

    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, card);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
        }
        return ret;
    }
    ESP_LOGI(TAG, "Filesystem mounted");
    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, *card);
    return ret;
}

void print_csv_data(std::vector<std::vector<std::string>> csv_data)
{
    for (int i = 0; i < csv_data.size(); i++)
    {
        std::string csv_line_joined = CSVFile::join_vec_string_with_delimiter(csv_data[i], ',');
        ESP_LOGI(TAG, "%s\n", csv_line_joined.c_str());
    }
}

extern "C" void app_main(void)
{
    sdmmc_card_t *card = NULL;
    char mount_point[] = MOUNT_POINT;
    if (init_sdcard(&card, mount_point) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing SdCard failed.");
        return;
    }
    std::string file_name = "csvFile.csv";
    std::string file_path = MOUNT_POINT "/" + file_name;
    // write_text("Hello World from sdcard!.", file_path);
    // std::string data = read_text(file_path);
    // ESP_LOGI(TAG, "message from SDCard: %s\n", data.c_str());
    std::vector<std::string> headers;
    headers.push_back("ID");
    headers.push_back("Cost");
    CSVFile csvfile(file_path, ',', headers, true);
    std::vector<std::string> csv_data;
    csv_data.push_back("1");
    csv_data.push_back("20$");
    csvfile.write_csv_line(csv_data);
    csv_data.clear();
    csv_data.push_back("2");
    csv_data.push_back("70$");
    csvfile.write_csv_line(csv_data);
    std::vector<std::vector<std::string>> csv_out_data = csvfile.get_all_csv_lines();
    print_csv_data(csv_out_data);
    unmount_sdcard(card, mount_point);
}