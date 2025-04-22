#include <iostream>
#include <string>
#include <vector>


#ifndef LAYER_HPP_
#define LAYER_HPP_

enum class LayerError{kSuccess,kTimedOut,kTempOutRange};
struct Layer{

    LayerError error; //! layer error
    size_t layer_number; //! layer number
    double layer_height; //! height of layer
    std::string material_type; //! type of material
    int extrusion_temp; //! temperature
    int print_speed; //! Speed
    std::string adhesion_quality; //! Quality
    double density; //! Density
    std::string infill_pattern; //! Pattern
    int shell_thickness; //! Thickness
    int overhang_angle; //! Angle
    int cooling_fan_speed; //! Fan speed
    int retraction_settings; //! retraction speed in mm
    float z_offset_adjustment; //! Offset adjustment
    int print_bed_temp; //! Print bed temperature
    std::string layer_time; //! time taken to print layer
    std::string filename; //! name of file to save layer
    std::string url_image; //! url image

    bool operator==(const Layer& rhs) const {
        return error == rhs.error &&
               layer_number == rhs.layer_number &&
               layer_height == rhs.layer_height &&
               material_type == rhs.material_type &&
               extrusion_temp == rhs.extrusion_temp &&
               print_speed == rhs.print_speed &&
               adhesion_quality == rhs.adhesion_quality &&
               density == rhs.density &&
               infill_pattern == rhs.infill_pattern &&
               shell_thickness == rhs.shell_thickness &&
               overhang_angle == rhs.overhang_angle &&
               cooling_fan_speed == rhs.cooling_fan_speed &&
               retraction_settings == rhs.retraction_settings &&
               z_offset_adjustment == rhs.z_offset_adjustment &&
               print_bed_temp == rhs.print_bed_temp &&
               layer_time == rhs.layer_time &&
               filename == rhs.filename &&
               url_image == rhs.url_image;
    }


};




class CompositeLayers{
public:
    CompositeLayers() = default;

    bool CheckValidLayer(Layer layer) const{
        if(layer.error != LayerError::kSuccess)
            return false;
        return true;

    }

    void AddLayer(Layer layer){
        layers_.push_back(layer);
    }

    bool IsEmpty(){
        return layers_.empty();
    }

    const std::vector<Layer> GetLayers() const{
        return layers_;
    }

    size_t GetNoLayers() const{
        return layers_.size();
    }


private:
    std::vector<Layer> layers_; //! layers


};


#endif
