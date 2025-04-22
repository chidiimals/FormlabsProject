#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <functional>

#include "Layer.hpp"


#ifndef CSV_READER_HPP_
#define CSV_READER_HPP_


std::vector<std::string> SplitCsvLine(const std::string& line) {
    std::stringstream ss(line);
    std::string cell;
    std::vector<std::string> result;
    while (std::getline(ss, cell, ',')) {
        result.push_back(cell);
    }
    return result;
}



std::optional<Layer> RowToLayer(std::vector<std::string> headers, std::vector<std::string> values){

    Layer result;
    if(headers.size() != values.size()) return std::nullopt;
    std::unordered_map<std::string, std::function<void(const std::string&)>> handlers = {
        {"Layer Number", [&](const std::string& v){ result.layer_number = std::stoi(v); }},
        {"Layer Height", [&](const std::string& v){ result.layer_height = std::stod(v); }},
        {"Material Type", [&](const std::string& v){ result.material_type = v; }},
        {"Extrusion Temperature", [&](const std::string& v){ result.extrusion_temp = std::stoi(v); }},
        {"Print Speed", [&](const std::string& v){ result.print_speed = std::stoi(v); }},
        {"Layer Adhesion Quality", [&](const std::string& v){ result.adhesion_quality = v; }},
        {"Infill Density", [&](const std::string& v){ result.density = std::stod(v); }},
        {"Infill Pattern", [&](const std::string& v){ result.infill_pattern = v; }},
        {"Shell Thickness", [&](const std::string& v){ result.shell_thickness = std::stof(v); }},
        {"Overhang Angle", [&](const std::string& v){ result.overhang_angle = std::stoi(v); }},
        {"Retraction Settings", [&](const std::string& v){ result.retraction_settings = std::stoi(v); }},
        {"Cooling Fan Speed", [&](const std::string& v){ result.cooling_fan_speed = std::stoi(v); }},
        {"Z-Offset Adjustment", [&](const std::string& v){ result.z_offset_adjustment = std::stof(v); }},
        {"Print Bed Temperature", [&](const std::string& v){ result.print_bed_temp = std::stoi(v); }},
        {"Layer Time", [&](const std::string& v){ result.layer_time = v; }},
        {"file_name", [&](const std::string& v){ result.filename = v; }},
        {"image url\r", [&](const std::string& v){ result.url_image = v; }},
        {"Layer Error", [&](const std::string& v){
            if (v == "SUCCESS") result.error = LayerError::kSuccess;
            else if (v == "TEMP_OUT_OF_RANGE") result.error = LayerError::kTimedOut;
            else if (v == "TIMED_OUT") result.error = LayerError::kTempOutRange;
        }},
    };
    
    try {
        for (size_t i = 0; i < headers.size(); ++i) {
            auto it = handlers.find(headers[i]);
            if (it != handlers.end()) {
                it->second(values[i]);
            }
        }
    } catch (const std::exception& e) {
        return std::nullopt;  // Bad data (non-numeric where numeric expected, etc.)
    }

    return result;
}



class CSVReader{

public:

    
    CSVReader() = default;
    /**
     * @brief Construct a new CSVReader object
     * 
     * @param filename 
     */
    CSVReader(std::string filename){
        ProcessCSV(filename);

    }

    /**
     * @brief Open and process csv file
     * 
     * @param filename 
     */
    void ProcessCSV(std::string filename){
        std::string line;
        input_file_.open(filename);
        std::cout << filename << std::endl;
        if(!input_file_.is_open())
            throw std::runtime_error("Could not open file " + filename);

        std::vector<std::string> headers;
      

        std::getline(input_file_,line);
        headers = SplitCsvLine(line);


        while(std::getline(input_file_,line)){
            std::vector<std::string> values = SplitCsvLine(line);
            const auto maybe_layer = RowToLayer(headers,values);
            if(maybe_layer.has_value()){
                composite_layer_.AddLayer(maybe_layer.value());
            }
        }
        
        input_file_.close();
    }

    /**
     * @brief Get the Commposite Layer object
     * 
     * @return const CompositeLayers 
     */
    const CompositeLayers GetCompositeLayer(){
        return composite_layer_;
    }
    
    
    
    private:
        CompositeLayers composite_layer_; //! Print layers
        std::ifstream input_file_; //! name of input file


};

#endif

