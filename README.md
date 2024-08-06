# zipper-project
Zipper project finals


*Project Data Structure Implemented*

XML Zipper
For my project, I found the data structure, zippers, on the wikipedia list, and chose to base my work around using the zipper to solve another problem. At an internship I had this summer, we did brief work with XML files, which inspired me to implement an XML Zipper, a data structure inspired by the zipper concept in functional programming. The XML Zipper is based on the concept of the Zipper, a technique commonly used in functional programming to traverse and update data structures like lists and trees. The XML Zipper provides a convenient way to navigate and manipulate an XML document while maintaining a context stack to easily traverse up and down the XML hierarchy. The main idea is to represent the current position within the XML document and allow efficient navigation and modification of the XML elements and their attributes.




*Short Explanation of the Data Structure*

The XML Zipper data structure I created consists of several components:

An XML Zipper consists of two main components: the current element (or focus) and a context stack. The current element represents the XML node currently being examined or modified. The context stack keeps track of the path from the root of the XML document to the current element, allowing for easy navigation back to parent elements.

The XML Zipper supports operations such as moving to a child element, moving up to the parent element, reading or updating the text content of the current element, and displaying the entire XML document from the current element's perspective. This structure is particularly useful for applications that need to dynamically navigate and modify XML documents, such as XML editors or XML-based configuration tools.

XMLElement: Represents an element in the XML document. It includes the tag name, text content, attributes, and children elements. This class serves as the fundamental building block of the XML structure.

XMLContext: Maintains the context of the current position in the XML document. It stores a reference to the parent element and the index of the current child. This helps in navigating back up the XML hierarchy.

XMLZipper: This is the main class that manages the XML document. It provides methods for parsing XML, navigating the XML tree (both down to children and up to parents), reading and setting text content, and displaying the current state of the XML document. The zipper maintains a stack of contexts to ensure easy traversal.




*Building the project*

-To build my project, I chose to do it with this command line.

g++ -std=c++11 -o xml_zipper main.cpp XMLZipper.cpp

-Followed by

./xml_zipper

-To execute the code





*Project Overview*


XML Parsing and Manipulation
The program reads a string representation of an XML document, parses it, and constructs an internal representation using the XML Zipper data structure. The parsing process I created involves reading the tags, attributes, and text content while handling various edge cases such as nested elements, attributes with spaces, and empty elements.

Navigation and Modification
The XML Zipper allows navigation through the XML document using methods like goToChild(size_t index) and goUp(). These methods enable moving down to a child element or up to the parent element, respectively. The current position in the XML tree is maintained, allowing modifications at any level of the hierarchy. The methods getCurrentText() and setCurrentText(const string& text) provide access to the text content of the current element.

Displaying the XML Document
The displayDocument(const shared_ptr<XMLElement>& element, int depth) method recursively displays the XML document, showing the structure with proper indentation. This method is used by printCurrentState() to display the entire XML document from the root element, providing a clear view of the current state of the XML document.





*What the Project Does*

The XML Zipper project provides a way to parse, navigate, and manipulate XML documents. It reads an XML document, constructs an in-memory representation, and allows efficient traversal and modification. The program outputs the initial state of the XML document and demonstrates navigation and modification by updating specific elements and displaying the updated state.




*Conclusion*

The XML Zipper project provides a powerful and flexible way to navigate and manipulate XML documents. By leveraging the Zipper data structure, the project enables efficient and intuitive operations on XML data, making it suitable for various applications requiring XML document processing. The provided instructions and demonstration materials aim to clearly communicate the project's functionality and usage.