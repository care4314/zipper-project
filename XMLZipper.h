#ifndef XMLZIPPER_H
#define XMLZIPPER_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Class to represent an XML element
class XMLElement {
public:
    string tag;
    string text;
    vector<pair<string, string>> attributes;
    vector<shared_ptr<XMLElement>> children;

    XMLElement(const string& tagName, const string& textContent = "");
};

struct XMLContext {
    shared_ptr<XMLElement> parent;
    size_t childIndex;

    XMLContext(shared_ptr<XMLElement> p, size_t index);
};

// Class definition for the XML Zipper
class XMLZipper {
private:
    shared_ptr<XMLElement> currentElement;
    vector<XMLContext> context;

public:
    XMLZipper(shared_ptr<XMLElement> root);

    void goToChild(size_t index);
    void goUp();
    string getCurrentText();
    void setCurrentText(const string& text);
    shared_ptr<XMLElement> getRoot();
    void displayDocument(const shared_ptr<XMLElement>& element, int depth = 0);
    void displayCurrentState();
    void parseXML(istream& inputStream);

private:
    shared_ptr<XMLElement> parseXMLElement(istream& stream);
    string readTag(istream& stream);
    vector<pair<string, string>> readAttributes(istream& stream);
    string readText(istream& stream);
    void skipWhitespace(istream& stream);
    void printCurrentState();
};

#endif // XMLZIPPER_H
