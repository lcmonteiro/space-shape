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
/**
 * ------------------------------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------------------------------
 */
const String ATTR_KEY = "#";
const String TEXT_KEY = "'";
const String NODE_KEY = "$";
/**
 * ----------------------------------------------------------------------------------------------------------
 * ToXML - Private
 * ----------------------------------------------------------------------------------------------------------
 */
namespace Private {
/**
 * ------------------------------------------------------------------------------------------------
 * Fill XML elements 
 * ------------------------------------------------------------------------------------------------
 */    
template<typename S>
void Fill(pugi::xml_node node, const Key& key, Var data, S schema) {
    switch (Var::Type(data)) {
        case Obj::Type::MAP: {
            auto child = node.append_child(key.data());
            Parse(key, Var::Map(data), schema, 
                [&child](auto key, String value) {
                    child.append_attribute(key.data()).set_value(value.data());
                }, 
                [&child](auto value) {
                    child.append_child(pugi::node_pcdata).set_value(value.data());
                },
                [&child, &schema](auto key, auto value) {
                    Fill(child, key, value, schema);
                }
            );
            break;
        } 
        case Obj::Type::LIST: {
            Tools::Basic::Foreach(Var::List(data), 
                [&node, &key, &schema](Var value) {
                    Fill(node, key, value, schema);
                }
            );
            break;
        } 
        case Obj::Type::LINK: {
            Fill(node, key, data, schema);
            break;
        } 
        default: {
            auto child = node.append_child(key.data());
            child.append_child(pugi::node_pcdata).set_value(Var::ToString(data).data());  
        }
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * process schema
 * ------------------------------------------------------------------------------------------------
 */
template<typename F, typename H>
void Parse(Map data, F filter, H handler) {
    Tools::Basic::Foreach(filter, [&data, &handler](auto rule) {
        auto found = data.find(rule);
        if(found != data.end()) {
            handler(found->first, Var(found->second));
            data.erase(found);
        }
    });
    Tools::Basic::Foreach(data, [&handler](auto pair) {
        handler(pair.first, Var(pair.second));
    });
}
/**
 * ------------------------------------------------------------------------------------------------
 * parse element
 * ------------------------------------------------------------------------------------------------
 */
struct Plus {	
    KeyList operator()(KeyList& left, const Link& right) const {	
        left.emplace_back(Var::ToString(right));
        return std::move(left);
    }
};
template<typename S, typename A, typename C, typename E>
void Parse(const Key& key, Map data, S schema, A attributes, C content, E elements) {
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
        auto found = data.find(ATTR_KEY);
        if(found != data.end()) {
            if(Var::IsMap(found->second)) {
                Parse(Var::Map(found->second), filter.second, attributes);
                data.erase(found);
            }        
        }
    }
    {
        auto found = data.find(TEXT_KEY);
        if(found != data.end()) {
            if(Var::IsString(found->second)) {
                content(Var::String(found->second));
                data.erase(found);
            }        
        }
    }
    Parse(data, filter.first, elements);
}}
/**
 * ----------------------------------------------------------------------------------------------------------
 * ToXML - Public
 * ----------------------------------------------------------------------------------------------------------
 */
std::ostream& Convert::ToXML(std::ostream& os, Var data, Var schema, FORMAT format) {
    pugi::xml_document doc;
    /**
     * Fill element
     */
    Tools::Basic::ForEach(Var::ToMap(data), [&schema, &doc](auto key, auto val) {
        Private::Fill(doc, key, val, Var::ToList(schema));
    });    
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
 * ----------------------------------------------------------------------------------------------------------
 *  From XML - Private
 * ----------------------------------------------------------------------------------------------------------
 */
namespace Private {
    /**
     * ------------------------------------------------------------------------------------------------
     * unload attributes 
     * ------------------------------------------------------------------------------------------------
     */
    Map UnloadAttr(pugi::xml_node& node) {
        //the attribute name must be unique
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
     * ------------------------------------------------------------------------------------------------
     * unload node 
     * ------------------------------------------------------------------------------------------------
     */
    Var UnloadNode(pugi::xml_node& node) {
        auto nodes = Map();
        auto text  = String();
        auto attr  = Map();
        // get attributes
        attr = UnloadAttr(node);
        // get children
        for (pugi::xml_node child : node.children()) {
            switch (child.type()) {
                case pugi::xml_node_type::node_element: {
                    Map::iterator it = nodes.find(child.name());
                    if (it == nodes.end()) {
                        // new child
                        nodes[child.name()] = UnloadNode(child);
                    } else if (Var::IsList(it->second)) {
                        // append child
                        Var::List(it->second).emplace_back(UnloadNode(child));
                    } else {
                        // update with list
                        nodes[child.name()] = Obj{it->second, UnloadNode(child)};
                    }
                    break;
                }
                case pugi::xml_node_type::node_pcdata:
                case pugi::xml_node_type::node_cdata: {
                    text.append(child.value());
                    break;
                }
                default:;
            }
        }
        if (!attr.empty()) {
            // insert attributes
            nodes.emplace(ATTR_KEY, Obj(std::move(attr)));
            // insert text
            if (!text.empty()) {
                nodes.emplace(TEXT_KEY, Obj(std::move(text)));
            }
            return Obj(std::move(nodes));
        }
        if (!nodes.empty()) {
            // insert text 
            if (!text.empty()) {
                nodes.emplace(TEXT_KEY, Obj(std::move(text)));
            }
            return Obj(std::move(nodes));
        }
        if (!text.empty()) {
            return Obj(std::move(text));
        }
        return Obj();
    }
}
/**
 * ----------------------------------------------------------------------------------------------------------
 *  From XML - Public
 * ----------------------------------------------------------------------------------------------------------
 */
Var Convert::FromXML(std::istream& is) {
    pugi::xml_document doc;
    return doc.load(is) ? Private::UnloadNode(doc) : Var(nullptr);
}
Var Convert::FromXML(std::istream&& is) {
    return FromXML(is);
}
/**
 * ----------------------------------------------------------------------------------------------------------
 * End
 * ----------------------------------------------------------------------------------------------------------
 */