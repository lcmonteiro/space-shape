/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   XmlConverter.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ---------------------------------------------------------------------------------------------------------------------
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
#include "SPattern.h"
#include "SBasic.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------------------------------
 */
const String __ATTR_KEY__ = "#";
const String __TEXT_KEY__ = "'";
const String __DATA_KEY__ = "$";

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
/**
 * ----------------------------------------------------------------------------------------------------------
 * ToXML - Private
 * ----------------------------------------------------------------------------------------------------------
 */
namespace Private {
/**
 * ------------------------------------------------------------------------------------------------
 * process 
 * ------------------------------------------------------------------------------------------------
 */    
template<typename Schema>
void Forward(pugi::xml_node node, const Key& key, Var data, Schema schema) {
    switch (Var::Type(data)) {
        /**
         * ------------------------------------------------------------------------
         * if map add attributs end elements
         * ------------------------------------------------------------------------
         */
        case Obj::Type::MAP: {
            Foreach(key, Var::Map(data), schema, 
                [&node](String key, String value) {
                    node.append_attribute(key.data()).set_value(value.data());
                }, 
                [&node, &schema](auto key, auto value) {
                    Forward(node.append_child(key.data()), key, value, schema);
                }
            );
        } break;
        /**
         * ------------------------------------------------------------------------
         * if list fill element
         * ------------------------------------------------------------------------
         */
        case Obj::Type::LIST: {
            Tools::Basic::Foreach(Var::List(data), 
                [&node, &key, &schema](Var value) {
                    Forward(node, key, value, schema);
                }
            );
        } break;
        /**
         * ------------------------------------------------------------------------
         * if link forward
         * ------------------------------------------------------------------------
         */
        case Obj::Type::LINK: {
            Forward(node, key, data, schema);
        } break;
        /**
         * ------------------------------------------------------------------------
         * default element data  
         * ------------------------------------------------------------------------
         */
        default: {
            if (key == __TEXT_KEY__) {
                node.append_child(
                    pugi::node_pcdata).set_value(Var::ToString(data).data());
            } else {
                node.append_child(key.data()).append_child(
                    pugi::node_pcdata).set_value(Var::ToString(data).data());
            }
        }
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * process schema
 * ------------------------------------------------------------------------------------------------
 */
template<typename Filter, typename Handler>
void Foreach(Map data, Filter filter, Handler handler) {
    /**
     * process filter
     */ 
    std::for_each(filter.begin(), filter.end(), 
        [&data, &handler](auto rule) {
            auto found = data.find(rule);
            if(found != data.end()) {
                handler(found->first, Var(found->second));
                data.erase(found);
            }
        }
    );
    /**
     * process remaining 
     */
    std::for_each(data.begin(), data.end(), 
        [&handler](auto pair) {
            handler(pair.first, Var(pair.second));
        }
    );
}
/**
 * ------------------------------------------------------------------------------------------------
 * process element
 * ------------------------------------------------------------------------------------------------
 */
struct Plus {	
    KeyList operator()(KeyList& left, const Link& right) const {	
        left.emplace_back(Var::ToString(right));
        return std::move(left);
    }
};
template<typename Schema, typename HandlerA, typename HandlerE>
void Foreach(const Key& key, Map data, Schema schema, HandlerA attributes, HandlerE elements) {
    /**
     * extract filters (nodes, attributes)
     */
    auto filter = Tools::Basic::Accumulate(schema, std::pair<KeyList, KeyList>(), 
        [&key](auto acc, auto rule) -> std::pair<KeyList, KeyList> {
            Var k = Var::List(rule).at(0);
            Var v = Var::List(rule).at(1);
            if(Tools::Pattern::Match(key, Var::ToString(k))) {
                if(Var::IsMap(v)) {
                    return {
                        Tools::Basic::Accumulate(List(v[__DATA_KEY__]), acc.first,  Plus()),
                        Tools::Basic::Accumulate(List(v[__ATTR_KEY__]), acc.second, Plus())
                    };
                }
                return {
                    Tools::Basic::Accumulate(List(v), acc.first,  Plus()), 
                    Tools::Basic::Accumulate(List() , acc.second, Plus())
                };
            }
            return std::move(acc);
        }
    );
    /**
     * process attributes 
     */
    auto found = data.find(__ATTR_KEY__);
    if(found != data.end()) {
        if(Var::IsMap(found->second)) {
            Foreach(Var::Map(found->second), filter.second, attributes);
            data.erase(found);
        }        
    }
    /**
     * process elements
     */ 
    Foreach(data, filter.first, elements);
}
}
/**
 * ----------------------------------------------------------------------------------------------------------
 * ToXML - Public
 * ----------------------------------------------------------------------------------------------------------
 */
std::ostream& Convert::ToXML(std::ostream& os, Var data, Var schema, FORMAT format) {
    pugi::xml_document doc;
    /**
     * load
     */
    Private::Forward(doc, "", data, Var::ToList(schema));
    /**
     * save on stream
     */
    doc.save(os, PUGIXML_TEXT("  "), std::map<FORMAT, unsigned int>({
        {RAW,      pugi::format_raw },
        {FILE,     pugi::format_indent },
        {RAW_NODE, pugi::format_raw | pugi::format_no_declaration }
    }).at(format));
    return os;
}
std::ostream& Convert::ToXML(std::ostream&& os, Var data, Var schema, FORMAT format) {
    return ToXML(os, data, schema, format);
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