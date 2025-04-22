#include <gtest/gtest.h>
#include <vector>

#include "include/Layer.hpp"
#include "include/CSVReader.hpp"




// std::vector<std::string> headers{"Material Type"}
// Layer layer_object = {.adhesion_quality="Good",
//                     .cooling_fan_speed = 10,
//                     .density=10,
//                     .error=LayerError::kSuccess,
//                     .extrusion_temp = 40,
//                 .layer_time="1min30sec",
//                 .overhang_angle=50,
//                 .filename="image.jpg",
//                 .material_type="PLA",
//             .url_image="https://drive.google.com/uc?export=download&id=18dB2HuwpFoyW0AyFDIilJ_PqbYkgeUkH",
//         .infill_pattern="Grid",
//         .print_bed_temp=30,
//     .layer_height=40,
// .layer_number=1,
// .shell_thickness=15,
// .s};


TEST(CSVReaderTests, SplitCsvLineTest) {
    const std::string test_string = "life,today,tomorrow";
    std::vector<std::string> expected = {"life", "today", "tomorrow"};
    
    auto result = SplitCsvLine(test_string);

    ASSERT_EQ(expected.size(), result.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(expected[i], result[i]);
    }
}

TEST(CSVReaderTests, RowToLayerTests){

    Layer layer;
    std::vector<std::string> header{"Material Type","Layer Error","Layer Number","Layer Adhesion Quality","Extrusion Temperature",
        "Infill Pattern","Infill Density"};
    std::vector<std::string> values{"PLA","SUCCESS","5","Good","210","Grid","20"};
    layer = RowToLayer(header,values).value();

    EXPECT_EQ(layer.material_type,"PLA");
    EXPECT_EQ(layer.error,LayerError::kSuccess);
    EXPECT_EQ(layer.layer_number,5);
    EXPECT_EQ(layer.adhesion_quality,"Good");
    EXPECT_EQ(layer.extrusion_temp,210);
    EXPECT_EQ(layer.infill_pattern,"Grid");
    EXPECT_EQ(layer.density,20);

    
    header = std::vector<std::string>{"Material Type","Layer Error","Layer Number","Layer Adhesion Quality","Extrusion Temperature",
        "Infill Pattern","Infill Density"};
    values = std::vector<std::string>{"PLA","SUCCESS","5","Good","210","Grid"};
    
    auto maybe_layer = RowToLayer(header,values);
    EXPECT_EQ(maybe_layer.has_value(),false);

}