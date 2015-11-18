/*!
 * \file XmlDocument.h
 *
 * A class to ease the I/O of XML files
 *
 * \date Dec 19, 2010
 * \author Arnaud Ramey
 */
#ifndef XMLDOCUMENT_H_
#define XMLDOCUMENT_H_

#define XML_IMPLEMENTATION_MXML       1
#define XML_IMPLEMENTATION_RAPID_XML  2
#define XML_USED_IMPLEMENTATION       XML_IMPLEMENTATION_RAPID_XML

/*
 * include the wanted implementation file
 */
#if XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_MXML
#include <mxml.h>
#elif XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_RAPID_XML
#include "xml/rapidxml-1.13/rapidxml.hpp"
#include "xml/rapidxml-1.13/rapidxml_print.hpp"
#endif // XML_USED_IMPLEMENTATION
//
// std
#include <string>
#include <vector>

class XmlDocument {
public:
#if XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_MXML
  typedef mxml_node_t Node;
#elif XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_RAPID_XML
  typedef rapidxml::xml_node<> Node;
#endif // XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_MXML

  /*!
     * constructor
     * @return an empty XmlDocument
     */
  XmlDocument();

  /*!
     * constructor
     * @param filename the absolute path to the file to parse
     * @return the XmlDocument obtained by parsing filename
     */
  XmlDocument(const std::string & filename);
  ~XmlDocument();
  bool load_from_string(const std::string & file_content);
  bool load_from_file(const std::string & filename);
  void write_to_file(const std::string & filename) const;
  Node* root() const;

  // ///////////////////////////////////////////////////////////////
  //  functions for extracting from tags
  /*!
     *
     * @param start
     * @param path
     * @return
     * EX :
     \code
     * node=<foo>
     *         <bar>1</bar>
     *         <bar>2</bar>
     * </foo>
     \endcode
     * get_value()
     */
  std::string get_value(const Node* start, const std::string & path) const;

  /*!
     *
     * @param start
     * @param path
     * @param default_value
     * @return
     */
  template<class _T>
  _T get_value(const Node* start, const std::string & path, _T default_value) const;

  // ///////////////////////////////////////////////////////////////
  // functions for extracting attributes
  /*!
     *
     * @param start where to start looking for
     * @param path the path where to search the child
     * @return the node at the given direction if it exists,
     * or NULL otherwise
     * EX :
     \code
     * node=<foo>
     *         <bar>1</bar>
     *         <bar>2</bar>
     * </foo>
     \endcode
     * get_node_at_direction(node, "foo.bar") will return the bar 1 node

     */
  Node* get_node_at_direction(const Node* start, const std::string & path) const;


  /*!
     * @param start
     * @param path
     * @param attribute_name a wanted attribute name
     * @param attribute_value a wanted attribute value
     * @return the node at the given direction if it exists,
     * or NULL otherwise
    */
  Node* get_node_at_direction(const Node* start,
                              const std::string & path,
                              const std::string & attribute_name,
                              const std::string & attribute_value) const;

  /*!
     *
     * @param node the node whose name we want to obtain
     * @return the name of this node
     * EX :
     * node="<foo param=3></foo>"
     * get_node_name(node) = "foo"
     */
  std::string get_node_name(const Node* node) const;

  /*!
     * @param node the node which has attributes
     * @param attribute the name of the attribute to extract
     * @return the attribute of node if found, otherwise ""
     *
     * EX :
     * node="<foo param=3></foo>"
     * get_node_attribute(node, "param") = "3"
     * get_node_attribute(node, "non_existing") = ""
     */
  std::string get_node_attribute(const Node* node, const std::string & attribute) const;

  /*!
     * A templated version of the string equivalent
     * @param node the node which has attributes
     * @param attribute the name of the attribute to extract
     * @param default_value what to return if we don't find
     * the given attribute
     * @return the attribute of node if found, otherwise the default value
     *
     * EX :
     * node="<foo param=3></foo>"
     * get_node_attribute(node, "param", 1) = 3
     * get_node_attribute(node, "non_existing", 1) = 1
     */
  template<class _T>
  _T get_node_attribute(const Node* node,
                        const std::string & attribute,
                        _T default_value) const;

  /*!
     *
     * @param start where to start looking for
     * @param path the path where are the children
     * @param ans the vector that will be populated with all the children
     * of "start" and which have a given path
     * EX :
     * \code
     * node=<foo>
     *         <bar>1</bar>
     *         <bar>2</bar>
     * </foo>
     * \endcode
     * If start=the "foo" Node and path="bar",
     * ans will contain both "bar" nodes
     */
  void get_all_nodes_at_direction(const Node* start, const std::string & path,
                                  std::vector<Node*>& ans) const;

  /*!
     *
     * @param start
     * @param path
     * @param ans
     */
  template<class _T>
  void get_all_values_at_direction(const Node* start,
                                   const std::string & path,
                                   std::vector<_T>& ans) const;

  // ///////////////////////////////////////////////////////////////
  /*!
     * extract all children from a given node
     * @param father where to extract the childrem
     * @param ans the vector that will be populated
     * with the children of "father"
     */
  void get_all_children(Node* father, std::vector<Node*>& ans) const;

  /*!
      \return a string version of the whole document
      */
  std::string to_string_node(Node* node) const;

  /*!
      \return a string version of the whole document
      */
  std::string to_string() const;

  // ///////////////////////////////////////////////////////////////
  // functions for writing
  Node* add_node(Node* father, Node* node_to_add, const std::string & path);
  Node* add_node(Node* father, const std::string & node_name, const std::string & path);
  // values
  //    inline void set_node_value(Node* node, const std::string & value);
  //    template<class _T>
  //    void set_node_value(Node* node, const _T & value);
  // attributes
  void set_node_attribute(Node* node, const std::string & attr_name, const std::string & value);
  template<class _T>
  void set_node_attribute(Node* node, const std::string & attr_name, const _T & value);

  /*! \return the path leading to this document, or "" if undefined
      This path contains the trailing "/"
    */
  std::string get_path() const
  { return _path; }

  //! \return the folder containing this document, or "" if undefined
  std::string get_folder() const
  { return _folder; }

private:
#if XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_MXML
  Node* _root;
#elif XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_RAPID_XML
  rapidxml::xml_document<> doc;
  std::vector<char> xml_copy;
#endif // XML_USED_IMPLEMENTATION

  //! the full path to the XML file that was parsed
  std::string _path;

  //! the folder where the XML file was parsed
  std::string _folder;
};

////////////////////////////////////////////////////////////////////////////////
/*** template implementations */
#include "string/StringUtils.h"

template<class _T>
_T XmlDocument::get_value(const Node* start,
                          const std::string & path,
                          _T default_value) const {
  std::string ans_string = get_value(start, path);
  bool success;
  _T ans = StringUtils::cast_from_string<_T>(ans_string, success);
  return (success ? ans : default_value);
}

////////////////////////////////////////////////////////////////////////////////

template<class _T>
_T XmlDocument::get_node_attribute(const Node* node,
                                   const std::string & attribute,
                                   _T default_value) const {
  std::string ans_string = get_node_attribute(node, attribute);
  bool success;
  _T ans = StringUtils::cast_from_string<_T>(ans_string, success);
  return (success ? ans : default_value);
}

////////////////////////////////////////////////////////////////////////////////

template<class _T>
void XmlDocument::get_all_values_at_direction(const Node* start,
                                              const std::string & path,
                                              std::vector<_T>& ans) const {
  //maggieDebug2("get_all_values_at_direction('%s')", path.c_str());
  std::vector<Node*> nodes;
  get_all_nodes_at_direction(start, path, nodes);

  ans.clear();
  for (std::vector<Node*>::const_iterator node = nodes.begin(); node
       != nodes.end(); ++node) {
    //#if XML_USED_IMPLEMENTATION == XML_IMPLEMENTATION_MXML
    //        Node* son = (*node)->child;
    //        if (son == NULL) {
    //            printf("The node ''%s'' does not have any child\n", path.c_str());
    //            continue;
    //        }
    //        // get the value
    //        bool success;
    //        _T value =
    //                StringUtils::cast_from_string<_T>(son->value.text.string, success);
    //        if (success)
    //            ans.push_back(value);

    //#else
    //        maggieError("Not implemented");
    //#endif
    std::string value_str = get_value(*node, "");
    if (value_str != "")
      ans.push_back(StringUtils::cast_from_string<_T>(value_str));
  } // end loop node
}

////////////////////////////////////////////////////////////////////////////////

//template<class _T>
//void XmlDocument::set_node_value(Node* node, const _T & value) {
//    std::string value_str = StringUtils::cast_to_string<_T>(value);
//    set_node_value(node, value_str);
//}

////////////////////////////////////////////////////////////////////////////////

template<class _T>
void XmlDocument::set_node_attribute(Node* node, const std::string & attr_name, const _T & value) {
  std::string value_str = StringUtils::cast_to_string<_T>(value);
  set_node_attribute(node, attr_name, value_str);
}

////////////////////////////////////////////////////////////////////////////////


#endif /* XMLDOCUMENT_H_ */

