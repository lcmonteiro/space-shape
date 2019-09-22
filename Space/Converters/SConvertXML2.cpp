/**
 * ------------------------------------------------------------------------------------------------
 * File:   XmlConverter.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#include <pugixml.hpp>
/**
 * std
 */
#include <map>
/**
 * space
 */
#include "SConvertXML.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------------------------------
 */
const String __ATTR_KEY__ = "#";
const String __TEXT_KEY__ = "'";
const String __DATA_KEY__ = "$";
/**
 * ------------------------------------------------------------------------------------------------
 * To XML - Private
 * ------------------------------------------------------------------------------------------------
 * definitions
 * --------------------------------------------------------------------------------------
 */
static inline void __LoadNode(
    pugi::xml_node& node, const Key& key, const Link& data, const Map& schema);
static inline void __LoadNode(
    pugi::xml_node node,  const Key& key, const List& data, const Map& schema);
static inline void __LoadNode(
    pugi::xml_node node,  const Map& data, const Map& schema);
static inline void __LoadAttr(
    pugi::xml_node node,  const Map& data, const List& schema);
/**
 * --------------------------------------------------------------------------------------
 * implementation
 * --------------------------------------------------------------------------------------
 */
void __LoadNode(
    pugi::xml_node& node, const Key& key, const Link& data, const Map& schema) {
    /**
     * ------------------------------------------------------------------------
     * if map add attributs or elements
     * ------------------------------------------------------------------------
     */
    if (Var::IsMap(data)) {
        if (__ATTR_KEY__ == key) {
            __LoadAttr(node, Var::Map(data)); 
            return;
        }
        __LoadNode(node.append_child(key.data()), Var::Map(data));
        return;
    }
    /**
     * ------------------------------------------------------------------------
     * if list add elements
     * ------------------------------------------------------------------------
     */
    if (Var::IsList(data)) {
        __LoadNode(node, key, Var::List(data));
        return;
    }
    /**
     * ------------------------------------------------------------------------
     * if link bypass
     * ------------------------------------------------------------------------
     */
    if (Var::IsLink(data)) {
        __LoadNode(node, key, Var::Link(data));
        return;
    } 
    /**
     * ------------------------------------------------------------------------
     * is a text elememt
     * ------------------------------------------------------------------------
     */
    if (__TEXT_KEY__ == key) {
        node.append_child(
            pugi::node_pcdata).set_value(Var::ToString(v).data());
    } else {
        node.append_child(k.data()).append_child(
            pugi::node_pcdata).set_value(Var::ToString(v).data());
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------------------------------------
 */
void __LoadNode(pugi::xml_node node, const Key& k, const List& data) {
    for (auto& v : data) {
        __LoadNode(node, k, v);
    }
}
void __LoadNode(pugi::xml_node node, const Map& data) {
    for (auto& v : data) {
        __LoadNode(node, v.first, v.second);
    }
}
void __LoadAttr(pugi::xml_node node, const Map& data) {
    for (auto& v : data) {
        node.append_attribute(v.first.data()) = Var::ToString(v.second).data();
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * To XML - Public
 * ------------------------------------------------------------------------------------------------
 */
std::ostream& Convert::ToXML(std::ostream& os, Var data, Var schema, FORMAT format) {
    pugi::xml_document doc;
    /**
     * ------------------------------------------------------------------------
     * load
     * ------------------------------------------------------------------------
     */
    if (Var::IsMap(data)) {
        __LoadNode(doc, Map(Var::Map(data)));
    } else {
        __LoadNode(doc, Map({{__DATA_KEY__, data}}));
    }
    /**
     * ------------------------------------------------------------------------
     * save on stream
     * ------------------------------------------------------------------------
     */
    doc.save(os, PUGIXML_TEXT("  "), std::map<FORMAT, unsigned int>({
        {RAW,      pugi::format_raw },
        {FILE,     pugi::format_indent },
        {RAW_NODE, pugi::format_raw | pugi::format_no_declaration }
    }).at(format));
    return os;
}
std::ostream& Convert::ToXML(std::ostream&& os, Var data, FORMAT format) {
    return ToXML(os, data, format);
}
/**
 * ------------------------------------------------------------------------------------------------
 *  From XML
 * ------------------------------------------------------------------------------------------------
 *  Private
 * ----------------------------------------------------------------------------
 * definitions
 */
static inline Var __UnLoadNode(pugi::xml_node& node);
static inline Map __UnLoadAttr(pugi::xml_node& node);
/**
 * implementation
 */
Var __UnLoadNode(pugi::xml_node& node) {
    /**
     * get attributes
     */
    Map attr = __UnLoadAttr(node);
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
                     * new
                     */
                    nodes[child.name()] = __UnLoadNode(child);
                } else if (Var::IsList(it->second)) {
                    /**
                     * append
                     */
                    Var::List(it->second).emplace_back(__UnLoadNode(child));
                } else {
                    /**
                     * update with list
                     */
                    nodes[child.name()] = Obj{it->second, __UnLoadNode(child)};
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
        nodes.emplace(__ATTR_KEY__, Obj(std::move(attr)));
        /**
         * insert text
         */
        if (!text.empty()) {
            /**
             * insert text 
             */
            nodes.emplace(__TEXT_KEY__, Obj(std::move(text)));
        }
        return Obj(std::move(nodes));
    }
    if (!nodes.empty()) {
        if (!text.empty()) {
            /**
             * insert text 
             */
            nodes.emplace(__TEXT_KEY__, Obj(std::move(text)));
        }
        return Obj(std::move(nodes));
    }
    if (!text.empty()) {
        return Obj(std::move(text));
    }
    return Obj();
}
Map __UnLoadAttr(pugi::xml_node& node) {
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
 * ----------------------------------------------------------------------------
 * Public
 * ----------------------------------------------------------------------------
 */
Var Convert::FromXML(std::istream& is) {
    pugi::xml_document doc;
    return doc.load(is) ? __UnLoadNode(doc) : Var(nullptr);
}
Var Convert::FromXML(std::istream&& is) {
    return FromXML(is);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */

