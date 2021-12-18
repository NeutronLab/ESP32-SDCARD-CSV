#include <csv_file.hpp>
#include <file_operations.hpp>
#include <esp_log.h>

int CSVFile::write_csv_line(std::vector<std::string> csv_line)
{
    FILE *f;
    f = fopen(_file_path.c_str(), "a");
    std::string csv_line_joined = CSVFile::join_vec_string_with_delimiter(csv_line, _delimiter);
    fputs(csv_line_joined.c_str(), f);
    fclose(f);
    return 1;
}

std::vector<std::vector<std::string>> CSVFile::get_all_csv_lines()
{
    std::vector<std::vector<std::string>> all_csv_data;
    std::vector<std::string> csv_line;
    std::string csv_entity = "";
    std::string file_content = read_text(_file_path);
    ESP_LOGI("FILECONTENT", "%s\n", file_content.c_str());
    for (int i = 0; i < file_content.length(); i++)
    {
        if (file_content[i] == _delimiter)
        {
            if (csv_entity.length() != 0)
            {
                csv_line.push_back(csv_entity);
                csv_entity.clear();
            }
        }
        else if (file_content[i] == '\n')
        {
            if (csv_entity.length() != 0)
            {
                csv_line.push_back(csv_entity);
                csv_entity.clear();
            }
            if (csv_line.size() != 0)
            {
                all_csv_data.push_back(csv_line);
                csv_line.clear();
            }
            csv_entity.clear();
        }
        else
        {
            csv_entity += file_content[i];
        }
    }
    return all_csv_data;
}

std::vector<std::string> CSVFile::split_string_by_delimiter(std::string csv_formatted_data, char delimiter)
{
    std::string tmp_str;
    std::vector<std::string> csv_line;
    for (int i = 0; i < csv_formatted_data.length(); i++)
    {
        if (csv_formatted_data[i] == delimiter)
        {
            csv_line.push_back(tmp_str);
            tmp_str = "";
        }
        tmp_str += csv_formatted_data[i];
    }
    if (!tmp_str.empty())
    {
        csv_line.push_back(tmp_str);
    }
    return csv_line;
}

std::string CSVFile::join_vec_string_with_delimiter(std::vector<std::string> csv_data, char delimiter)
{
    std::string csv_data_joined;
    for (int i = 0; i < csv_data.size(); i++)
    {
        csv_data_joined += csv_data[i] + delimiter;
    }
    uint8_t last_delimiter_index = csv_data_joined.find_last_of(delimiter);
    csv_data_joined[last_delimiter_index] = '\n';
    return csv_data_joined;
}
