#include <gtest/gtest.h>
#include "include/Layer.hpp"

TEST(LayerTest, SimpleTests){

    Layer layer = {.adhesion_quality = "Good",.cooling_fan_speed = 10,.density = 20,
                   .error = LayerError::kTempOutRange};
    
    CompositeLayers comp_layer;
    EXPECT_EQ(comp_layer.CheckValidLayer(layer),false);
    
    EXPECT_EQ(comp_layer.IsEmpty(),true);
    
    comp_layer.AddLayer(layer);
    
    EXPECT_EQ(comp_layer.IsEmpty(),false);
    EXPECT_EQ(comp_layer.GetNoLayers(),1);
    std::vector<Layer> layers {layer};

    for(int i = 0; i < comp_layer.GetNoLayers(); i++){
        EXPECT_EQ(comp_layer.GetLayers()[i],layers[i]);
    }

}
