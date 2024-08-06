#include "XMLZipper.h"

XMLElement::XMLElement(const string& tagName, const string& textContent)
    : tag(tagName), text(textContent) {}

XMLContext::XMLContext(shared_ptr<XMLElement> p, size_t index) 
    : parent(p), childIndex(index) {}

XMLZipper::XMLZipper(shared_ptr<XMLElement> root) : currentElement(root) {}

void XMLZipper::parseXML(const string& input) {
    size_t pos = 0;
    currentElement = parseXMLElement(input, pos);
    if (!currentElement) {
        cerr << "Failed to parse XML." << endl;
    } else {
        cout << "XML Parsing Successful. Root tag: " << currentElement->tag << endl;
    }
}

shared_ptr<XMLElement> XMLZipper::parseXMLElement(const string& xml, size_t& pos) {
    skipWhitespace(xml, pos);
    string tag = readTag(xml, pos);
    if (tag.empty()) {
        cerr << "Error: Failed to read tag or tag name is empty." << endl;
        return nullptr;
    }

    auto element = make_shared<XMLElement>(tag, "");
    element->attributes = readAttributes(xml, pos);

    while (pos < xml.size()) {
        if (xml[pos] == '<') {
            if (xml[pos + 1] == '/') {
                pos = xml.find('>', pos);
                if (pos == string::npos) break;
                ++pos;
                break;
            } else if (xml[pos + 1] == '?' || xml[pos + 1] == '!') {
                pos = xml.find('>', pos);
                if (pos == string::npos) break;
                ++pos;
            } else {
                element->children.push_back(parseXMLElement(xml, pos));
            }
        } else {
            element->text = readText(xml, pos);
        }
    }
    return element;
}

string XMLZipper::readTag(const string& xml, size_t& pos) {
    string tag;
    bool inTag = false;
    while (pos < xml.size()) {
        char ch = xml[pos++];
        if (ch == '>') break;
        if (ch == '<') {
            inTag = true;
            continue;
        }
        if (inTag) tag += ch;
    }

    size_t tagEnd = tag.find_first_of(" \t\n/>");
    if (tagEnd != string::npos) tag = tag.substr(0, tagEnd);

    return tag;
}

vector<pair<string, string>> XMLZipper::readAttributes(const string& xml, size_t& pos) {
    vector<pair<string, string>> attributes;
    string name, value;
    bool inValue = false;

    while (pos < xml.size()) {
        char ch = xml[pos++];
        if (ch == '=') {
            pos++;
            inValue = true;
            continue;
        } else if (ch == '"') {
            if (inValue) {
                attributes.emplace_back(name, value);
                name.clear();
                value.clear();
                inValue = false;
                continue;
            }
        } else if (ch == '>') {
            break;
        } else if (ch != ' ' && ch != '\n' && ch != '\t' && ch != '/') {
            if (inValue) value += ch;
            else name += ch;
        } else if (ch == ' ' && !inValue && !name.empty()) {
            attributes.emplace_back(name, ""); 
            name.clear();
        }
    }

    if (inValue) cerr << "Error: Attribute value not closed properly." << endl;
    if (!name.empty()) attributes.emplace_back(name, ""); 

    return attributes;
}

string XMLZipper::readText(const string& xml, size_t& pos) {
    string text;
    while (pos < xml.size()) {
        char ch = xml[pos++];
        if (ch == '<') {
            --pos;
            break;
        }
        text += ch;
    }
    return text;
}

void XMLZipper::skipWhitespace(const string& xml, size_t& pos) {
    while (pos < xml.size()) {
        char ch = xml[pos];
        if (ch != ' ' && ch != '\n' && ch != '\t') break;
        ++pos;
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
    displayDocument(getRoot());
}
