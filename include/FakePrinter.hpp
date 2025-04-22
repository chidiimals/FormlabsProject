#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>


#include "FileDownloader.hpp"
#include "UserInteraction.hpp"
#include "FileExporter.hpp"
#include "CSVReader.hpp"

#ifndef FAKE_PRINTER_HPP_
#define FAKE_PRINTER_HPP_

struct Summary{
    int total_jobs;
    int failed_jobs;
    int printed_jobs;

};

void to_json(json& j, const Summary& summary) {
    j = json{
        {"expected layer height",summary.total_jobs},
        {"failed layers", summary.failed_jobs},
        {"total printed layers ", static_cast<int>(summary.printed_jobs)}
    };
}class FakePrinter{
public:

FakePrinter(
    const std::string& name,
    std::unique_ptr<PluginConverter> plugin,
    const std::string& csv_file_name,
    const std::string& directory_name,
    std::unique_ptr<UserMode> mode
)
    : name_(std::move(name)),
      id_(next_id_++),
      plugin_(std::move(plugin)),
      csv_file_name_(std::move(csv_file_name)),
      directory_name_(std::move(directory_name)),
      base_path_(std::filesystem::current_path()),
      mode_(std::move(mode)),jobs_(0)
{
    std::filesystem::create_directories(base_path_/directory_name_);
    base_path_str_ = base_path_.string();
    std::cout << base_path_str_ << std::endl;
}

    void PrintJob(const std::string& filename){
        
        std::string full_path = (base_path_/csv_file_name_).string();
        reader_.ProcessCSV(full_path);
        std::cout << reader_.GetCompositeLayer().GetNoLayers() << std::endl;
        for (const auto layer: reader_.GetCompositeLayer().GetLayers()){

            
            std::string file_name = "layer_" + std::to_string(layer.layer_number);
            std::string directory_path = (base_path_ / directory_name_/file_name);
            
            std::filesystem::create_directories(directory_path);
            // Create a fresh plugin
            auto plugin_copy = plugin_->Clone();
            FileWriter writer(directory_path, file_name, std::move(plugin_copy));
            
            mode_->ContinuePrint();
            
            if(layer.error != LayerError::kSuccess){
                mode_->EncounteredError();
                ++failed_jobs_;
            }
            else{
                ++jobs_;

            }
            
            if(mode_->StopPrinting()){
                break;
            }
            
            downloader_.start_download(layer.url_image,directory_path,layer.filename);
            writer.Write(layer);
                

        }

        Summary summary = {.total_jobs = static_cast<int>(reader_.GetCompositeLayer().GetNoLayers()),.printed_jobs = jobs_,.failed_jobs = failed_jobs_};
        std::string summary_file{base_path_/ "summary.json"};
        write_to_json(summary,summary_file);


    }

    const int GetID() const{
        return id_;
    }

    const std::string GetName() const{
        return name_;
    }

    void write_to_json(const Summary& summary,std::string summary_file) {
        json j;
        to_json(j, summary);
        
        std::ofstream file(summary_file);  
        
        if (file.is_open()) {
            file << j.dump(4); 
        } else {
            std::cerr << "Could not open file for writing!" << std::endl;
        }
    }
    
private:
    const std::string name_; //! printer name
    const int id_; //! printer id

    CSVReader reader_; //! CSV reader
    std::unique_ptr<UserMode> mode_; //! User mode
    std::unique_ptr<PluginConverter> plugin_;  //! Plugin
    FileDownloader downloader_;
    std::string csv_file_name_; //! csv file name
    std::string directory_name_; //! name of directory
    std::string base_path_str_; //! base path
    std::filesystem::path base_path_; //! file system base path
    int jobs_; //! no of jobs
    int failed_jobs_; //! no of failed jobs


static int next_id_; //! static variable for unique ids of printers 
};

int FakePrinter::next_id_ = 0;

#endif
