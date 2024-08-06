#include <iostream>
#include <sstream>
#include "XMLZipper.h"

using namespace std;

int main() {
    std::string xmlData = R"(
    <company name="Engineering">
        <employee name="Jane Smith" />
        <department name="IT">
            <employee name="Michael Brown" />
        </department>
    </company>
    )";

    
    std::istringstream xmlStream(xmlData);

    
    auto root = make_shared<XMLElement>("root");
    XMLZipper zipper(root);

    
    zipper.parseXML(xmlStream);

    // Display the current state
    zipper.displayCurrentState();

    // Example of setting text (make sure to navigate to a proper element first)
    zipper.setCurrentText("Updated John Doe");

    // Display the updated XML
    zipper.displayCurrentState();

    return 0;
}
