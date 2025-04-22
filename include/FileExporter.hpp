#include <memory>
#include <iostream>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Layer.hpp"

#ifndef FILE_EXPORTER_HPP_
#define FILE_EXPORTER_HPP_


using json = nlohmann::json;


class PluginConverter{
    public:
        virtual std::string Convert(const Layer& layer) const = 0;
        virtual ~PluginConverter() = default;
        virtual std::unique_ptr<PluginConverter> Clone() const = 0;
    
};
    
void to_json(json& j, const Layer& layer) {
    j = json{
        {"error", static_cast<int>(layer.error)},
        {"layer_number", layer.layer_number},
        {"layer_height", layer.layer_height},
        {"material_type", layer.material_type},
        {"extrusion_temp", layer.extrusion_temp},
        {"print_speed", layer.print_speed},
        {"adhesion_quality", layer.adhesion_quality},
        {"density", layer.density},
        {"infill_pattern", layer.infill_pattern},
        {"shell_thickness", layer.shell_thickness},
        {"overhang_angle", layer.overhang_angle},
        {"cooling_fan_speed", layer.cooling_fan_speed},
        {"retraction_settings", layer.retraction_settings},
        {"z_offset_adjustment", layer.z_offset_adjustment},
        {"print_bed_temp", layer.print_bed_temp},
        {"layer_time", layer.layer_time},
        {"filename", layer.filename},
        {"url_image", layer.url_image}
    };
}

class JsonPlugin : public PluginConverter {
    public:
        std::string Convert(const Layer& layer) const override {
            json output = layer;
            return output.dump(4); // Pretty print with 4 spaces
        }

           
        std::unique_ptr<PluginConverter> Clone() const override {
            return std::make_unique<JsonPlugin>(*this); // Use copy constructor
        }
    };
    
    
    class YamlPlugin : public PluginConverter{
        std::string Convert(const Layer& layer) const override{
        std::ostringstream oss;
        oss << "- layer_number: " << layer.layer_number << "\n"
                << "  layer_height: " << layer.layer_height << "\n"
                << "  material_type: " << layer.material_type << "\n"
                << "  extrusion_temp: " << layer.extrusion_temp << "\n"
                << "  print_speed: " << layer.print_speed << "\n"
                << "  adhesion_quality: " << layer.adhesion_quality << "\n"
                << "  density: " << layer.density << "\n"
                << "  infill_pattern: " << layer.infill_pattern << "\n"
                << "  shell_thickness: " << layer.shell_thickness << "\n"
                << "  overhang_angle: " << layer.overhang_angle << "\n"
                << "  cooling_fan_speed: " << layer.cooling_fan_speed << "\n"
                << "  retraction_settings: " << layer.retraction_settings << "\n"
                << "  z_offset_adjustment: " << layer.z_offset_adjustment << "\n"
                << "  print_bed_temp: " << layer.print_bed_temp << "\n"
                << "  layer_time: " << layer.layer_time << "\n"
                << "  filename: " << layer.filename << "\n"
                << "  url_image: " << layer.url_image << "\n";
            
                return oss.str();   
        }

            
        std::unique_ptr<PluginConverter> Clone() const override {
            return std::make_unique<YamlPlugin>(*this); // Use copy constructor
        }

    
    };

class Exporter {
    public:
        
        /**
         * @brief Construct a new Exporter object
         * 
         * @param plugin 
         */
        Exporter(std::unique_ptr<PluginConverter> plugin):plugin_(std::move(plugin)){}


        /**
         * @brief 
         * 
         * @param layer
         * @return true 
         * @return false 
         */
        const std::string Export(const Layer& layer)
        {
                  
            return plugin_->Convert(layer);

        }

    
    private:
        std::unique_ptr<PluginConverter> plugin_; //! unique pointer member

    };

class FileWriter{
public:
    FileWriter(const std::string& directory,std::string filename, std::unique_ptr<PluginConverter> plugin, const std::string& file_extension = ".json",
        std::ios_base::openmode mode = std::ios::out): directory_(directory),filename_(filename),
        exporter_(std::move(plugin)),mode(mode),file_extension(file_extension){
        
        OpenFile();

        if(!filestream.is_open()){
            const std::string full_file_name = directory_ + "/" + filename + file_extension;
            throw std::runtime_error("Failed to open file " + full_file_name);
        }
    }

    void Write(const std::string& data) {
        filestream << data;
    }

    void Write(const Layer& layer){

        filestream << exporter_.Export(layer);
    }

    ~FileWriter(){
        filestream.close();
    }


    /**
     * @brief 
     *  Open file
     */
    void OpenFile(){
        std::string full_file_name;
        std::string directory;
        full_file_name =  directory_ + "/" + filename_ + file_extension;
        filestream.open(full_file_name,mode);
    }

    /**
     * @brief 
     *  Close file
     */
    void Close(){
        filestream.close();
    }

private:
    std::fstream filestream; //! file stream
    std::string directory_; //! file path
    std::string filename_;
    std::string file_extension; //! file extension
    std::ios_base::openmode mode; //! file mode
    Exporter exporter_; //! exporter 
};


#endif