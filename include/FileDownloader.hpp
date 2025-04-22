#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#ifndef FILE_DOWNLOADER_HPP_
#define FILE_DOWNLOADER_HPP_


class FileDownloader {
    public:
        FileDownloader()
            : curl_handle_(curl_easy_init()){
            if (!curl_handle_) {
                throw std::runtime_error("Download Failed: Failed to initialize curl");
            }

        }

        ~FileDownloader() {
            if (curl_handle_) {
                curl_easy_cleanup(curl_handle_);
            }
        }

        void start_download(std::string url, std::string folder_name, std::string filename) {
            std::filesystem::path file_path{folder_name + "/" +filename};
            url.pop_back();
            std::cout << "url: " << url << std::endl;
            std::cout << "Saving to: " << file_path << std::endl;
            std::ofstream output_file_;
            output_file_.open(file_path, std::ios::binary);
            if (!output_file_.is_open()) {
                throw std::runtime_error("Error opening file");
            }

            // Enable verbose logging for debugging
            curl_easy_setopt(curl_handle_, CURLOPT_VERBOSE, 1L);

            // Set the curl options
            curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, &FileDownloader::write_data);
            curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &output_file_);

            // Perform the download
            CURLcode res = curl_easy_perform(curl_handle_);
            if (res != CURLE_OK) {
                std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
            } else {
                long http_code = 0;
                curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &http_code);
                std::cout << "Download succeeded! HTTP Response Code: " << http_code << std::endl;
            }
        }

    private:
        CURL* curl_handle_;


        static size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
            if (size * nmemb > 0) {
                std::cout << "Received data chunk: size = " << size << ", nmemb = " << nmemb << std::endl;
                std::ofstream* output_file = static_cast<std::ofstream*>(userp);
                output_file->write(static_cast<char*>(buffer), size * nmemb);
            } else {
                std::cout << "No data received." << std::endl;
            }

            return size * nmemb;
        }
};

#endif // FILE_DOWNLOADER_HPP_


