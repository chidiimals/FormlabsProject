
#include "include/FileDownloader.hpp"

int test_download() {
    try {
        FileDownloader downloader;
        std::string url = "https://drive.google.com/uc?export=download&id=18dB2HuwpFoyW0AyFDIilJ_PqbYkgeUkH";
        downloader.start_download(url,"/Users/chidi/Documents/Courses/Code/Formlabs","test_image");
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}