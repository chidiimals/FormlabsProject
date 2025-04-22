#include <iostream>
#include "include/FakePrinter.hpp"
#include "include/FileExporter.hpp"
#include "include/UserInteraction.hpp"


int main(){

    
    FakePrinter printer(
        std::string("Formlabs Printer"),
        std::make_unique<JsonPlugin>(),
        std::string("fl_coding_challenge_v1.csv"),
        std::string("Printing"),
        std::make_unique<AutomaticMode>()
    );

    printer.PrintJob("Job1");

}