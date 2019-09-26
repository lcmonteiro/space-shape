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
#include "SVariant.h"
#include "SBasic.h"
#include "SLog.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------------------------------
 */
const String ATTR_KEY = "#";
const String TEXT_KEY = "'";
const String NODE_KEY = "$";

// List schema_model = {
//     // rule 1
//     Obj{ 
//         Obj("*"), 
//         Obj{ 
//             {Key("#"), Obj{ 
//                 Obj("attr")
//             }}, 
//             {Key("$"), Obj{ 
//                 Obj("SHORT-NAME")
//             }} 
//         }
//     }
// };
List schema_model = {
    // rule 1
    Obj{ 
        Obj("*"), 
        Obj{ 
            Obj("SHORT-NAME")
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
template<typename S>
void Forward(pugi::xml_node node, const Key& key, Var data, S schema) {
    switch (Var::Type(data)) {
        case Obj::Type::MAP: {
            DEBUG("map", key);
            /**
             * add attributs end elements
             */
            Foreach(key, Var::Map(data), schema, 
                [&node](auto key, String value) {
                    node.append_attribute(key.data()).set_value(value.data());
                }, 
                [&node](auto value) {
                    node.append_child(pugi::node_pcdata).set_value(value.data());
                },
                [&node, &schema](auto key, auto value) {
                    Forward(node.append_child(key.data()), key, value, schema);
                }
            );
            break;
        } 
        case Obj::Type::LIST: {
            DEBUG("list", key);
            /**
             * fill element
             */
            Tools::Variant::Foreach(Var::List(data), 
                [&node, &key, &schema](Var value) {
                    Forward(node, key, value, schema);
                },
                [&node, &key, &schema](Var value) {
                    Forward(node.append_child(key.data()), key, value, schema);
                }
            );
            break;
        } 
        case Obj::Type::LINK: {
            /**
             * link forward
             */
            Forward(node, key, data, schema);
            break;
        } 
        default: {
            /**
             * data element  
             */
            node.append_child(
                pugi::node_pcdata).set_value(Var::ToString(data).data());   
        }
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * process schema
 * ------------------------------------------------------------------------------------------------
 */
template<typename F, typename H>
void Foreach(Map data, F filter, H handler) {
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
template<typename S, typename A, typename C, typename E>
void Foreach(const Key& key, Map data, S schema, A attributes, C content, E elements) {
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
                        Tools::Basic::Accumulate(List(v[NODE_KEY]), acc.first,  Plus()),
                        Tools::Basic::Accumulate(List(v[ATTR_KEY]), acc.second, Plus())
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
    { 
        /**
         * process attributes 
         */  
        auto found = data.find(ATTR_KEY);
        if(found != data.end()) {
            if(Var::IsMap(found->second)) {
                Foreach(Var::Map(found->second), filter.second, attributes);
                data.erase(found);
            }        
        }
    }
    {
        /**
         * process content 
         */
        auto found = data.find(TEXT_KEY);
        if(found != data.end()) {
            if(Var::IsString(found->second)) {
                content(Var::String(found->second));
                data.erase(found);
            }        
        }
    }
    /**
     * process elements
     */ 
    Foreach(data, filter.first, elements);
}}
/**
 * ----------------------------------------------------------------------------------------------------------
 * ToXML - Public
 * ----------------------------------------------------------------------------------------------------------
 */
std::ostream& Convert::ToXML(std::ostream& os, Var data, Var schema, FORMAT format) {
    pugi::xml_document doc;

    //schema = Obj(schema_model);
    /**
     * load
     */
    Private::Forward(doc, {}, data, Var::ToList(schema));
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
        nodes.emplace(ATTR_KEY, Obj(std::move(attr)));
        /**
         * insert text
         */
        if (!text.empty()) {
            /**
             * insert text 
             */
            nodes.emplace(TEXT_KEY, Obj(std::move(text)));
        }
        return Obj(std::move(nodes));
    }
    if (!nodes.empty()) {
        if (!text.empty()) {
            /**
             * insert text 
             */
            nodes.emplace(TEXT_KEY, Obj(std::move(text)));
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
            throw std::range_error(String::Build("xml::attr=", attr.name()));
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