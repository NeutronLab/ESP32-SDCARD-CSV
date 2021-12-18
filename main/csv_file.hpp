#include <string>
#include <vector>
#include <esp_log.h>

class CSVFile
{
private:
    /* data */
    std::vector<std::string> _csv_headers;
    char _delimiter;
    std::string _file_path;

public:
    CSVFile(std::string file_path, char delimiter, std::vector<std::string> csv_headers, bool delete_file_if_exist)
    {

        _delimiter = delimiter;
        _file_path = file_path;
        if (delete_file_if_exist)
        {
            if (remove(file_path.c_str()) != 0)
            {
                ESP_LOGE("csv_file.hpp", "Unable to delete %s file.\n", file_path.c_str());
            }
        }
        if (csv_headers.size() != 0)
        {
            _csv_headers = csv_headers;
            this->write_csv_line(_csv_headers);
        }
    }
    ~CSVFile()
    {
    }
    int write_csv_line(std::vector<std::string> csv_line);
    std::vector<std::vector<std::string>> get_all_csv_lines();
    std::vector<std::string> get_header();

    static std::vector<std::string> split_string_by_delimiter(std::string csv_formatted_data, char delimiter);
    static std::string join_vec_string_with_delimiter(std::vector<std::string> csv_data, char delimiter);
};

// CSVFile::CSVFile(std::string file_path, std::vector<std::string> csv_headers, char delimiter)
// {

// }

// CSVFile::~CSVFile()
// {
// }
