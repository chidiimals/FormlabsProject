#include "Layer.hpp"
#include <iostream>
#include "FileExporter.hpp"
#include "CSVReader.hpp"

#ifndef USER_INTERACTION_HPP_
#define USER_INTERACTION_HPP_

class UserMode{
public:


    virtual ~UserMode() = default;

    virtual void Start(std::string filename) = 0;
    virtual void GetUserInput() = 0;
    virtual void EncounteredError() = 0;
    virtual void ContinuePrint() = 0;
    
    bool StopPrinting(){
        return stop_printing_;
    }

    void UpdatePrintStatus(bool status){
        stop_printing_ = status;
    }


    
    
private:
    bool stop_printing_{false}; //! stop printing

};


class SupervisedMode : public UserMode {
    public:
        void ContinuePrint() override {
            std::cout << "Inspect layer + Hit Enter to continue" << std::endl;
            std::string input;
    
            while (true) {
                std::getline(std::cin, input);
                if (input.empty()) {
                    std::cout << "Printing next layer" << std::endl;
                    break;
                } else {
                    std::cout << "Invalid input. Please hit enter to continue" << std::endl;
                }
            }
        }
    
        void EncounteredError() override {
            std::string input;
            bool valid_input = false;
    
            while (!valid_input) {
                std::cout << "Error Encountered: Do you want to stop printing? [y/n]" << std::endl;
                std::getline(std::cin, input);
    
                if (!input.empty()) {
                    char choice = std::tolower(input[0]);
                    if (choice == 'y') {
                        std::cout << "You chose Yes" << std::endl;
                        UpdatePrintStatus(true);
                        valid_input = true;
                    } else if (choice == 'n') {
                        std::cout << "You chose No" << std::endl;
                        UpdatePrintStatus(false);
                        valid_input = true;
                    } else {
                        std::cout << "Invalid input. Please enter [y/n]" << std::endl;
                    }
                } else {
                    std::cout << "Invalid input. Please enter [y/n]" << std::endl;
                }
            }
        }
    
        void Start(std::string filename) override {
            std::cout << "Supervised Start: Printing file " << filename << std::endl;
        }
    
        void GetUserInput() override {
            std::cout << "Supervised GetUserInput" << std::endl;
        }
    };
    

class AutomaticMode : public UserMode {
    public:
        void EncounteredError() override {
            std::cout << "Error Encountered: Stopping printing immediately" << std::endl;
            UpdatePrintStatus(false);
        }
    
        void ContinuePrint() override {
            std::cout << "Auto Mode: Continuing without user input" << std::endl;
        }
    
        void Start(std::string filename) override {
            std::cout << "Automatic Start: Printing file " << filename << std::endl;
        }
    
        void GetUserInput() override {
            std::cout << "Automatic GetUserInput" << std::endl;
        }
    };

#endif
    