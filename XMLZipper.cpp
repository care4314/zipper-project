#include "XMLZipper.h"
#include <iostream>
#include <memory>
#include <sstream>

using namespace std;

XMLElement::XMLElement(const string& tagName, const string& textContent)
    : tag(tagName), text(textContent) {}

XMLContext::XMLContext(shared_ptr<XMLElement> p, size_t index) 
    : parent(p), childIndex(index) {}

XMLZipper::XMLZipper(shared_ptr<XMLElement> root) : currentElement(root) {}

void XMLZipper::parseXML(istream& inputStream) {
    currentElement = parseXMLElement(inputStream);
    if (!currentElement) {
        cerr << "Failed to parse XML." << endl;
    } else {
        cout << "XML Parsing Successful. Root tag: " << currentElement->tag << endl;
    }
}

shared_ptr<XMLElement> XMLZipper::parseXMLElement(istream& stream) {
    skipWhitespace(stream);
    string tag = readTag(stream);
    if (tag.empty()) {
        cerr << "Error: Failed to read tag or tag name is empty." << endl;
        return nullptr;
    }

    auto element = make_shared<XMLElement>(tag, "");
    element->attributes = readAttributes(stream);

    char nextChar;
    while (stream.get(nextChar)) {
        if (nextChar == '<') {
            if (stream.peek() == '/') {
                while (stream.get(nextChar) && nextChar != '>') {}
                break;
            } else if (stream.peek() == '?') {
                while (stream.get(nextChar) && nextChar != '>') {}
            } else if (stream.peek() == '!') {
                while (stream.get(nextChar) && nextChar != '>') {}
            } else {
                stream.putback('<');
                element->children.push_back(parseXMLElement(stream));
            }
        } else if (nextChar == '>') {
            element->text = readText(stream);
        }
    }
    return element;
}

string XMLZipper::readTag(istream& stream) {
    string tag;
    char ch;
    bool inTag = false;
    while (stream.get(ch)) {
        if (ch == '>') {
            break;
        }
        if (ch == '<') {
            if (inTag) {
                cerr << "Error: Unexpected '<' character in tag reading." << endl;
                return "";
            }
            inTag = true;
            continue;
        }
        if (inTag) {
            tag += ch;
        }
    }

    size_t pos = tag.find_first_of(" \t\n/>");
    if (pos != string::npos) {
        tag = tag.substr(0, pos);
    }

    cout << "Parsed tag: " << tag << endl;
    return tag;
}

vector<pair<string, string>> XMLZipper::readAttributes(istream& stream) {
    vector<pair<string, string>> attributes;
    string name, value;
    char ch;
    bool inValue = false;

    while (stream.get(ch)) {
        if (ch == '=') {
            if (name.empty()) {
                cerr << "Error: Unexpected '=' character." << endl;
                break;
            }
            stream.get(ch);
            if (ch != '"') {
                cerr << "Error: Expected opening quote for attribute value." << endl;
                break;
            }
            inValue = true;
            continue;
        } else if (ch == '"') {
            if (inValue) {
                attributes.emplace_back(name, value);
                name.clear();
                value.clear();
                inValue = false;
                continue;
            } else {
                cerr << "Error: Unexpected closing quote for attribute value." << endl;
                break;
            }
        } else if (ch == '>') {
            break;
        } else if (ch != ' ' && ch != '\n' && ch != '\t' && ch != '/') {
            if (inValue) {
                value += ch;
            } else {
                name += ch;
            }
        } else if (ch == ' ' && !inValue && !name.empty()) {
            attributes.emplace_back(name, ""); 
            name.clear();
        }
    }

    if (inValue) {
        cerr << "Error: Attribute value not closed properly." << endl;
    }

    if (!name.empty()) {
        attributes.emplace_back(name, ""); 
    }

    if (attributes.empty()) {
        cerr << "Error: No attributes found or failed to read attributes." << endl;
    } else {
        for (const auto& attr : attributes) {
            cout << "Attribute: " << attr.first << "=\"" << attr.second << "\"" << endl; 
        }
    }

    return attributes;
}

string XMLZipper::readText(istream& stream) {
    string text;
    char ch;
    while (stream.get(ch)) {
        if (ch == '<') {
            stream.putback(ch); 
            break;
        }
        text += ch;
    }
    return text;
}

void XMLZipper::skipWhitespace(istream& stream) {
    char ch;
    while (stream.get(ch)) {
        if (ch != ' ' && ch != '\n' && ch != '\t') {
            stream.putback(ch);
            break;
        }
    }
}

void XMLZipper::goToChild(size_t index) {
    if (currentElement && index < currentElement->children.size()) {
        context.emplace_back(currentElement, index);
        currentElement = currentElement->children[index];
    } else {
        cerr << "Invalid child index." << endl;
    }
}

void XMLZipper::goUp() {
    if (!context.empty()) {
        XMLContext ctx = context.back();
        context.pop_back();
        currentElement = ctx.parent;
    } else {
        cerr << "No parent to navigate to." << endl;
    }
}

string XMLZipper::getCurrentText() {
    return currentElement ? currentElement->text : "";
}

void XMLZipper::setCurrentText(const string& text) {
    if (currentElement) {
        currentElement->text = text;
    } else {
        cerr << "No current element to update." << endl;
    }
}

shared_ptr<XMLElement> XMLZipper::getRoot() {
    while (!context.empty()) {
        goUp();
    }
    return currentElement;
}

void XMLZipper::displayDocument(const shared_ptr<XMLElement>& element, int depth) {
    if (!element) return;

    cout << string(depth * 4, ' ') << "<" << element->tag;

    for (const auto& attr : element->attributes) {
        cout << " " << attr.first << "=\"" << attr.second << "\"";
    }

    if (element->children.empty() && element->text.empty()) {
        cout << "/>" << endl;
    } else {
        cout << ">";
        if (!element->text.empty()) {
            cout << element->text;
        }
        cout << endl;
        for (const auto& child : element->children) {
            displayDocument(child, depth + 1);
        }
        cout << string(depth * 4, ' ') << "</" << element->tag << ">" << endl;
    }
}

void XMLZipper::displayCurrentState() {
    printCurrentState();
}

void XMLZipper::printCurrentState() {
    displayDocument(getRoot());
}
