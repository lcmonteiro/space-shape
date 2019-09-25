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
    pugi::xml_node& node, const Key& key, Link data, Map schema);
static inline void __LoadNode(
    pugi::xml_node node,  const Key& key, List data, Map schema);
static inline void __LoadNode(
    pugi::xml_node node,  Map data, Map schema);
static inline void __LoadAttr(
    pugi::xml_node node,  Map data, List schema);

List schema = {
    // rule 1
    Obj{ 
        Obj("*"), 
        Obj{ 
            {Key("#"), Obj{ 
                Obj("attr")
            }}, 
            {Key("$"), Obj{ 
                Obj("")
            }} 
        }
    }
};
template<typename Data, typename Schema>
void forward(pugi::xml_node& node, Key key, Data data, Schema schema){
    
}

template<typename HandlerAttributes>
void foreach(const Map& data, const List& rules, HandlerAttributes hattr) {
    std::for_each(data.begin(), std::remove_if(data.begin(), data.end(), [](){

    }), [](){

    });
}

template<typename HandlerAttributes, typename HandlerData>
void foreach(const Map& data, const List& schema, const Key& key, HandlerAttributes hattr, HandlerData hdata) {
    /**
     * extract filter
     */
    List filter;
    auto filter = std::accumulate(schema.begin(), schema.end(), std::pair<List> [](){

    });
    std::copy_if(schema.begin(), schema.end(), std::back_inserter(filter), [](){

    });
    /**
     * process filter
     */ 
    for(Var rule : Logic::ToList(filter, [](auto data) { 
        return List(data[__DATA_KEY__]);
    })) {
       auto found = data.find(rule);
       if(found != data.end())) {
           hdata(found->first, found->second);
           data.erase(found);
       } 
    }
    /**
     * process remaining 
     */
    std::for_each(data.begin(), data.end(), [](auto pair) {
            if(__ATTR_KEY__ == pair.first) {
                foreach(Var::ToMap(link), );
            } else {
                hdata(found->first, found->second);
            }
        }
    );
}


/**
 * --------------------------------------------------------------------------------------
 * implementation
 * --------------------------------------------------------------------------------------
 */
void __LoadNode(
    pugi::xml_node& node, const Key& key, Link data, Link schema) {
    
    switch (Var::Type(data)) {
        /**
         * ------------------------------------------------------------------------
         * if map add attributs or elements
         * ------------------------------------------------------------------------
         */
        case Obj::Type::MAP: {
            foreach(Var::Map(data), Var::ToList(schema), key, 
                [&node](auto key, auto value) {
                    node.append_attribute(key.data()).set_value(value.data());
                }, 
                [&node](auto key, auto value) {
                    forward(node.append_child(key.data()), value);
                }
            );
        } break;
        /**
         * ------------------------------------------------------------------------
         * if list add elements
         * ------------------------------------------------------------------------
         */

    

    
    // if (Var::IsMap(data)) {
    //     if (__ATTR_KEY__ == key) {
    //         __LoadAttr(node, Var::Map(data)); 
    //         return;
    //     }
    //     __LoadNode(node.append_child(key.data()), Var::Map(data));
    //     return;
    // }
    
    // if (Var::IsList(data)) {
    //     __LoadNode(node, key, Var::List(data));
    //     return;
    // }
    // /**
    //  * ------------------------------------------------------------------------
    //  * if link bypass
    //  * ------------------------------------------------------------------------
    //  */
    // if (Var::IsLink(data)) {
    //     __LoadNode(node, key, Var::Link(data));
    //     return;
    // } 
    // /**
    //  * ------------------------------------------------------------------------
    //  * is a text elememt
    //  * ------------------------------------------------------------------------
    //  */
    // if (__TEXT_KEY__ == key) {
    //     node.append_child(
    //         pugi::node_pcdata).set_value(Var::ToString(v).data());
    // } else {
    //     node.append_child(k.data()).append_child(
    //         pugi::node_pcdata).set_value(Var::ToString(v).data());
    // }
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
        node.append_attribute(v.first.data()).set_value(Var::ToString(v.second).data());
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

