#include <iostream>
#include <string>
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

    auto root = make_shared<XMLElement>("root");
    XMLZipper zipper(root);

    // Directly pass the XML string to parseXML
    zipper.parseXML(xmlData);

    // Display the current state
    zipper.displayCurrentState();

    // Example of setting text (make sure to navigate to a proper element first)
    zipper.setCurrentText("Updated John Doe");

    // Display the updated XML
    zipper.displayCurrentState();

    return 0;
}
