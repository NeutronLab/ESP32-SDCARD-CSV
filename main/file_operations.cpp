#include "file_operations.hpp"
#include "esp_log.h"
#include "string.h"
#include <iostream>

static const char *TAG = "File Operations";

#ifndef MOUNT_POINT
#define MOUNT_POINT "/sdcard"
#endif

extern std::string read_text(std::string file_path)
{
    if (file_path.empty())
    {
        return "";
    }
    // std::string mount_point = MOUNT_POINT;
    // std::string mount_point = MOUNT_POINT "/";
    // std::string file_path = mount_point + "HelloWorld231.txt";
    // std::string file_path = MOUNT_POINT "/" + file_name;

    FILE *f;
    f = fopen(file_path.c_str(), "r");
    ESP_LOGI(TAG, "Reading file %s\n", file_path.c_str());
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return "";
    }
    int c_int;
    std::string data;
    while ((c_int = fgetc(f)) != EOF)
    {
        data.push_back((char)c_int);
    }
    // char data_clist[100];
    // fgets(data_clist, 100, f);
    fclose(f);
    return data;
}

extern void write_text(std::string data, std::string file_path)
{
    if (data.empty() || file_path.empty())
    {
        return;
    }
    // std::string file_path = MOUNT_POINT "/" + file_name;
    if (file_path.empty())
    {
        ESP_LOGE(TAG, "Couldn't concat file name with mounting point.");
        return;
    }
    FILE *f;
    f = fopen(file_path.c_str(), "w");
    ESP_LOGI(TAG, "writing to file %s\n", file_path.c_str());
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fputs(data.c_str(), f);
    fclose(f);
    ESP_LOGI(TAG, "File written");
}
