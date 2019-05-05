/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   XmlConverter.h
 * Author: Luís Monteiro
 *
 * Created on September 14, 2015, 4:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H
/**
 * extern
 */
#include <pugixml.hpp>
/**
 * std
 */
#include <sstream>
/**
 * Space
 */
#include "SVariable.h"
/**
 * definitions
 */
const String __ATTR_KEY__ = "#";
const String __TEXT_KEY__ = "'";
const String __DATA_KEY__ = "$";
/**
 * --------------------------------------------------------------------------------------------------------------------
 * XmlConvert
 * --------------------------------------------------------------------------------------------------------------------
 */
typedef class __xmlconvert__ {
public:
    /**
     * --------------------------------------------------------------------------------------------
     * format types
     * --------------------------------------------------------------------------------------------
     */
    typedef enum {
        RAW      = pugi::format_raw,
        FILE     = pugi::format_indent,
        RAW_NODE = pugi::format_raw | pugi::format_no_declaration
    } FORMAT;
    /**
     * --------------------------------------------------------------------------------------------
     *  to XML
     * --------------------------------------------------------------------------------------------
     */
    static std::ostream& toXML(std::ostream& os, Var data, FORMAT format = FORMAT::FILE) {
        pugi::xml_document doc;
        /**
         * load
         */
        if (Var::IsMap(data)) {
            __load_node(doc, Map(Var::Map(data)));
        } else {
            __load_node(doc, Map({
                {__DATA_KEY__, data}
            }));
        }
        /**
         * save on stream
         */
        doc.save(os, PUGIXML_TEXT("\t"), format);
        return os;
    }
    static inline std::ostream& toXML(std::ostream&& os, Var data, FORMAT format = FORMAT::FILE) {
        return toXML(os, data, format);
    }
    static inline Var toXML(Var data, FORMAT format = FORMAT::RAW_NODE) {
        std::ostringstream os;
        toXML(os, data, format);
        return Obj::String(os.str());
    }
    /**
     * --------------------------------------------------------------------------------------------
     *  from XML
     * --------------------------------------------------------------------------------------------
     */
    static Var fromXML(std::istream& is) {
        pugi::xml_document doc;
        /**
         * unload
         */
        return doc.load(is) ? __unload_node(doc) : Var(nullptr);
    }
    static inline Var fromXML(std::istream&& is) {
        return fromXML(is);
    }
    static inline std::istream& fromXML(std::istream& is, Var& data) {
        data = fromXML(is);
        return is;
    }
    static inline std::istream& fromXML(std::istream&& is, Var& data) {
        data = fromXML(is);
        return is;
    }
private:
    /**
     * --------------------------------------------------------------------------------------------
     * unserialize
     * --------------------------------------------------------------------------------------------
     */
    static Var __unload_node(pugi::xml_node& node) {
        /**
         * get attributes
         */
        Map attr = __unload_attr(node);
        /**
         * get children
         */
        Map nodes;
        String text;
        for (pugi::xml_node child : node.children()) {
            switch (child.type()) {
                case pugi::xml_node_type::node_element:
                {
                    Map::iterator it = nodes.find(child.name());
                    if (it == nodes.end()) {
                        /**
                         * create
                         */
                        nodes[child.name()] = __unload_node(child);
                    } else if (Var::IsList(it->second)) {
                        /**
                         * append
                         */
                        Var::List(it->second).push_back(__unload_node(child));
                    } else {
                        /**
                         * update with list
                         */
                        nodes[child.name()] = Obj::List({it->second, __unload_node(child)});
                    }
                    break;
                }
                case pugi::xml_node_type::node_pcdata:
                case pugi::xml_node_type::node_cdata:
                {
                    text.append(child.value());
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        /**
         * create object
         */
        if (!attr.empty()) {
            /**
             * insert attributes
             */
            nodes.emplace(__ATTR_KEY__, Obj::Map(attr));
            /**
             * insert text
             */
            if (!text.empty()) {
                /**
                 * insert text 
                 */
                nodes.emplace(__TEXT_KEY__, Obj::String(text));
            }
            return Obj::Map(nodes);
        }
        if (!nodes.empty()) {
            if (!text.empty()) {
                /**
                 * insert text 
                 */
                nodes.emplace(__TEXT_KEY__, Obj::String(text));
            }
            return Obj::Map(nodes);
        }
        if (!text.empty()) {
            return Obj::String(text);
        }
        return Obj::Null();
    }

    static Map __unload_attr(pugi::xml_node& node) {
        /**
         * the attribute name must be unique
         */
        Map out;
        for (pugi::xml_attribute attr : node.attributes()) {
            auto res = out.insert(make_pair(attr.name(), Obj(attr.value())));
            if ( !res.second ) {
                String error="xml::tag=";
                error.append(attr.name());
                throw std::range_error(error);
            }
        }
        return out;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * serialize
     * --------------------------------------------------------------------------------------------
     */
    static inline void __load_node(pugi::xml_node& node, const Key& k, const Link& v) {
        if (Var::IsMap(v)) {
            /**
             * check if is attribute key
             */
            if (k == __ATTR_KEY__) {
                __load_attr(node, Map(Var::Map(v)));

            } else {
                __load_node(node.append_child(k.data()), Map(Var::Map(v)));
            }
        } else if (Var::IsList(v)) {
            /**
             * load a list of nodes with same type 
             */
            __load_node(node, k, List(Var::List(v)));
        } else if (Var::IsLink(v)) {
            /**
             * reload function 
             */
            __load_node(node, k, Var::Link(v));
        } else {
            /**
             * check if is text key
             */
            if (k == __TEXT_KEY__) {
                node.append_child(pugi::node_pcdata).set_value(Var::ToString(v).data());
            } else {
                node.append_child(k.data()).append_child(
                    pugi::node_pcdata).set_value(Var::ToString(v).data());
            }
        }
    }
    static void __load_node(pugi::xml_node node, const Key& k, List&& data) {
        for (auto& v : data) {
            __load_node(node, k, v);
        }
    }
    static void __load_node(pugi::xml_node node, Map&& data) {
        for (auto& v : data) {
            __load_node(node, v.first, v.second);
        }
    }
    static void __load_attr(pugi::xml_node node, Map&& data) {
        for (auto& v : data) {
            node.append_attribute(v.first.data()) = Var::ToString(v.second).data();
        }
    }
} XmlConvert;
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* XMLCONVERTER_H */

