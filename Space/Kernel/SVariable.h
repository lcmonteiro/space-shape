/**
 * ------------------------------------------------------------------------------------------------
 * File:   Variable.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef VARIABLE_H
#define VARIABLE_H
/**
 * std
 */
#include <initializer_list>
#include <unordered_map>
#include <stdexcept>
#include <memory>
/**
 * space
 */
#include "STypes.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Pointers
 * ------------------------------------------------------------------------------------------------
 * declare
 */
class __interface__;
/**
 * define
 */
typedef class __link__: public std::shared_ptr<__interface__> {
    public:
    using __super__ = std::shared_ptr<__interface__>;
    /**
     * __super__
     */ 
    using __super__::__super__;
    using __super__::operator=;
    /**
     * constructors
     */  
    __link__() = default;
    __link__(const __super__&  a) : __super__(a) {}
    __link__(      __super__&& a) : __super__(std::move(a)) {}
} Link;
/**
 * ------------------------------------------------------------------------------------------------
 * Containers
 * ------------------------------------------------------------------------------------------------
 * List
 * ----------------------------------------------------------------------------
 **/
typedef class __list__ : public std::vector<__link__> {
public:
    using __super__ = std::vector<__link__>;
    /**
     * super
     */ 
    using __super__::__super__;
    using __super__::operator=;
    /**
     * operators
     */
    inline __list__ operator + (__link__ val) {
        auto res = __list__(*this);
        res.emplace_back(val);
        return std::move(res);
    }
    inline __list__ operator + (__super__ list) {
        for(auto& v : list) {
            emplace_back(std::move(v));
        }     
        return *this;
    }
    inline __list__& operator += (__link__ val) {
        this->emplace_back(val);
        return *this;
    }
} List;
/**
 * ----------------------------------------------------------------------------
 * Map
 * ----------------------------------------------------------------------------
 */
typedef class __map__ : public std::unordered_map<__key__, __link__> {
public:
    using __super__ = std::unordered_map<__key__, __link__>;
    /**
     * super
     */ 
    using __super__::__super__;
    using __super__::operator=;
    /**
     * constructors
     */    
    __map__() = default;
    __map__(const __super__&  a): __super__(a) {}
    __map__(      __super__&& a): __super__(std::move(a)) {}
} Map;
/**
 * ------------------------------------------------------------------------------------------------
 * Object interfaces
 * ------------------------------------------------------------------------------------------------
 **/
class __interface__ {
    friend class __var__;
    friend class __obj__;
    virtual Link operator[](String ) { throw std::logic_error(__func__); }
    virtual Link operator[](Integer) { throw std::logic_error(__func__); }
    virtual Integer __to_integer()   { throw std::logic_error(__func__); }
    virtual Float   __to_float  ()   { throw std::logic_error(__func__); }
    virtual String  __to_string ()   { throw std::logic_error(__func__); }
    virtual Buffer  __to_buffer ()   { throw std::logic_error(__func__); }
    virtual List    __to_list   ()   { throw std::logic_error(__func__); }
    virtual Map    __to_map     ()   { throw std::logic_error(__func__); }
    virtual int    __type       ()   { throw std::logic_error(__func__); }
};
/**
 * ------------------------------------------------------------------------------------------------
 * Objects
 * ------------------------------------------------------------------------------------------------
 **/
typedef class __obj__ : public __link__ {
    friend class __var__;
    /**
     * ------------------------------------------------------------------------
     * private types
     * ------------------------------------------------------------------------
     */
    class __boolean : public __boolean__, public __interface__ {
    public:
        using __boolean__::__boolean__;
        using __boolean__::operator=;
        /**
         * constructors
         */
        __boolean() = default; 
        __boolean(const __boolean__&  b): __boolean__(b) {}
        __boolean(      __boolean__&& b): __boolean__(std::move(b)) {}
        /**
         * interface
         */
        inline __integer__ __to_integer() override { 
            return __integer__(this->_val);    
        }
        inline __float__ __to_float  () override {
            return __float__(this->_val);      
        }
        inline __string__ __to_string () override { 
            return __string__::ValueOf(*this); 
        }
        inline int __type () override { 
            throw BOOLEAN; 
        }
    };
    class __integer : public __integer__, public __interface__ {
    public:
        using __integer__::__integer__;
        using __integer__::operator=;
        /**
         * constructors
         */
        __integer() = default; 
        __integer(const __integer__&  i): __integer__(i) {}
        __integer(      __integer__&& i): __integer__(std::move(i)) {}
        /**
         * interface
         */
        inline __float__ __to_float() override {
            return __float__(this->_val);
        }
        inline __string__ __to_string() override {
            return __string__::ValueOf(*this);
        }
        inline __integer__ __to_integer() override {
            return *this;
        }
        inline int __type () override { 
            return INTEGER; 
        }
    };
    class __float : public __float__, public __interface__ {
    public:
        using __float__::__float__;
        using __float__::operator=;
        /**
         * constructors
         */
        __float() = default; 
        __float(const __float__&  f): __float__(f) {}
        __float(      __float__&& f): __float__(std::move(f)) {}
        /**
         * interface
         */
        inline __integer__ __to_integer() override {
            return __integer__(this->_val); 
        }
        inline __string__ __to_string() override {
            return __string__::ValueOf(*this); 
        }
        inline __float__ __to_float() override {
            return *this; 
        }
        inline int __type() override {
            return FLOAT; 
        }
    };
    class __string : public __string__, public __interface__ {
    public:
        using __string__::__string__;
        using __string__::operator=;
        /**
         * constructors
         */
        __string() = default; 
        __string(const __string__&  s): __string__(s) {}
        __string(      __string__&& s): __string__(std::move(s)) {}
        /**
         * interface
         */
        inline __integer__ __to_integer() override {
            return __integer__::ValueOf(*this);
        }
        inline __float__ __to_float() override {
            return __float__::ValueOf(*this);
        }
        inline __string__ __to_string() override {
            return *this;
        }
        inline int __type() override {
            return STRING;  
        }
    };
    class __buffer : public __buffer__, public __interface__ {
    public:
        using __buffer__::__buffer__;
        using __buffer__::operator=;
        /**
         * constructors
         */
        __buffer() = default; 
        __buffer(const __buffer__&  b): __buffer__(b) {}
        __buffer(      __buffer__&& b): __buffer__(std::move(b)) {}
        /**
         * interface
         */
        inline __integer__ __to_integer() override {
            return __integer__::ValueOf(*this);
        }
        inline __float__ __to_float() override {
            return __float__(0);
        }
        inline __string__ __to_string() override {
            return __string__::ValueOf(*this);
        }
        inline __buffer__ __to_buffer() override {
            return *this;
        }
        inline int __type() override {
            return INTEGER;  
        }
    };
    class __list : public __list__, public __interface__ {
    public:
        using __list__::__list__;
        using __list__::operator=;
        /**
         * constructors
         */
        __list() = default;
        __list(const __list__&  l): __list__(l) { }
        __list(      __list__&& l): __list__(std::move(l)) {}
        /**
         * interface
         */
        inline __link__ operator[](__string__ val) override {
            try { 
                return at(__integer__::ValueOf(val)); 
            } catch (...) { 
                return __link__(); 
            }
        }
        inline __link__ operator[](__integer__ val) override {
            try { 
                return at(val); 
            } catch (...) { 
                return __link__(); 
            }
        }
        inline __map__ __to_map() override {
            __map__ m; 
            for (__integer__ i = 0; size_t(i) < size(); ++i) {
                m[__string__::ValueOf(i)] = at(i);
            }
            return m;
        }
        inline __list__ __to_list() override {
            return *this;
        }
        inline int __type() override {
            return LIST;  
        }
    };
    class __map : public __map__, public __interface__ {
    public:
        using __map__::__map__;
        using __map__::operator=;
        /**
         * constructors
         */
        __map() = default; 
        __map(const __map__& m): __map__(m) {}
        __map(__map__&& m): __map__(std::move(m)) {}
        /**
         * interface
         */
        inline __link__ operator[](__string__ val) override {
            try { 
                return at(val); 
            } catch (...) { 
                return __link__(); 
            }
        }
        inline __link__ operator[](__integer__ val) override {
            try { 
                return at(__string__::ValueOf(val)); 
            } catch (...) { 
                return __link__(); 
            }
        }
        inline __list__ __to_list() override {
            __list__ l; 
            for (auto& v : * this) l.push_back(v.second); 
            return l;
        }
        inline __map__ __to_map() override {
            return *this;
        }
        inline int __type() override {
            return MAP; 
        }
    };
    class __link : public __link__, public __interface__ {
    public:
        using __link__::__link__;
        using __link__::operator=;
        /**
         * constructors
         */
        __link() = default; 
        __link(const __link__ &  l): __link__(l) {}
        __link(      __link__ && l): __link__(std::move(l)) {}
        /**
         * operators 
         */
        inline __link__ operator[](__string__ val) override {
            return get() ? get()->operator[](val) : __link__();
        }
        inline __link__ operator[](__integer__ val) override {
            return get() ? get()->operator[](val) : __link__();
        }
        inline __integer__ __to_integer() override {
            return get() ? get()->__to_integer() : __integer__();
        }
        inline __float__ __to_float() override {
            return get() ? get()->__to_float() : __float__();
        }
        inline __string__ __to_string() override {
            return get() ? get()->__to_string() : __string__();
        }
        inline __buffer__ __to_buffer() override {
            return get() ? get()->__to_buffer() : __buffer__();
        }
        inline __list__ __to_list() override {
            return get() ? get()->__to_list() : __list__();
        }
        inline __map__ __to_map() override {
            return get() ? get()->__to_map() : __map__();
        }
        inline int __type() override {
            return LINK;  
        }
    };
    using __link__::operator=;
public:
    __obj__() = delete;
    /**
     * --------------------------------------------------------------------------------------------
     * types identifiers 
     * --------------------------------------------------------------------------------------------
     */
    enum Type : int { 
        NONE = 0, BOOLEAN, INTEGER, FLOAT, STRING, BUFFER, LIST, MAP, LINK
    };
    /**
     * --------------------------------------------------------------------------------------------
     * Constructores
     * --------------------------------------------------------------------------------------------
     * native types
     */
    __obj__(nullptr_t    ) : __link__() {}
    __obj__(bool        v) : __link__(std::make_shared<__boolean>(v)) {}
    __obj__(int         v) : __link__(std::make_shared<__integer>(v)) {}
    __obj__(const char* v) : __link__(std::make_shared<__string >(v)) {}
    __obj__(float       v) : __link__(std::make_shared<__float  >(v)) {}
    __obj__(double      v) : __link__(std::make_shared<__float  >(v)) {}
    /**
     * base types
     */
    __obj__(__boolean__ v) : __link__(std::make_shared<__boolean>(std::move(v))) {}
    __obj__(__integer__ v) : __link__(std::make_shared<__integer>(std::move(v))) {}
    __obj__(__float__   v) : __link__(std::make_shared<__float  >(std::move(v))) {}
    __obj__(__string__  v) : __link__(std::make_shared<__string >(std::move(v))) {}
    __obj__(__buffer__  v) : __link__(std::make_shared<__buffer >(std::move(v))) {}
    __obj__(__list__    v) : __link__(std::make_shared<__list   >(std::move(v))) {}
    __obj__(__map__     v) : __link__(std::make_shared<__map    >(std::move(v))) {}
    /**
     * list initializer
     */
    __obj__(std::initializer_list<__map::value_type>  l) : __link__(std::make_shared<__map >(l)){}
    __obj__(std::initializer_list<__list::value_type> l) : __link__(std::make_shared<__list>(l)){}
    /**
     * --------------------------------------------------------------------------------------------
     * Helpers
     * --------------------------------------------------------------------------------------------
     * create link
     */
    static inline __link__ Link(__link__ l) {
        return std::make_shared<__link>(std::move(l));
    }
} Obj;
/**
 * ------------------------------------------------------------------------------------------------
 * Generic variable
 * ------------------------------------------------------------------------------------------------
 */
typedef class __var__ : public __link__ {
public:
    using __link__::__link__;
    using __link__::operator=;
    /**
     * get operators
     */
    inline const __var__ operator[](__string__ val) const {
        return __var__(get() ? (*get())[val] : __link__());
    }
    inline const __var__ operator[](const char * val) const {
        return __var__(get() ? (*get())[__string__(val)] : __link__());
    }
    inline const __var__ operator[](__integer__ val) const {
        return __var__(get() ? (*get())[val] : __link__());
    }
    inline const __var__ operator[](int val) const {
        return __var__(get() ? (*get())[__integer__(val)] : __link__());
    }
    /**
     * access operatores
     */
    inline const __var__ operator[](__var__ val) const {
        try {
            if (IsMap(*this)) {
                return Map(*this).at(ToString(val));
            }
            if (IsList(*this)) {
                return List(*this).at(ToInteger(val));
            }
            if (IsUndefined(*this)) {
                return __link__();
            }
        } catch(std::out_of_range& ex){
            return __link__();
        }
        throw std::logic_error(__func__);
    }
    /**
     * logic and arithmetic operators
     */
    inline __var__ operator+(__var__ val) const {
        if (IsInteger(*this)) {
            return Obj(::Integer(Integer(*this) + ToInteger(val)));
        }
        if (IsFloat(*this)) {
            return Obj(::Float(Float(*this) + ToFloat(val)));
        }
        if (IsString(*this)) {
            return Obj(::String(String(*this) + ToString(val)));
        }
        throw std::logic_error(__func__);
    }
#define VAR_OPERATOR_T1(_x_)                                         \
    inline __var__ operator _x_(__var__ val) const {                 \
        if (IsInteger(*this)) {                                      \
            return Obj(::Integer(Integer(*this) _x_ ToInteger(val)));\
        }                                                            \
        if (IsFloat(*this)) {                                        \
            return Obj(::Float(Float(*this) _x_ ToFloat(val)));      \
        }                                                            \
        throw std::logic_error(__func__);                            \
    }
    VAR_OPERATOR_T1(-);
    VAR_OPERATOR_T1(*);
    VAR_OPERATOR_T1(/);
#define VAR_OPERATOR_T2(_x_)                                         \
    inline __var__ operator _x_(__var__ val) const {                 \
        if (IsInteger(*this)) {                                      \
            return Obj(::Integer(Integer(*this) _x_ ToInteger(val)));\
        }                                                            \
        throw std::logic_error(__func__);                            \
    }
    VAR_OPERATOR_T2(^);
    VAR_OPERATOR_T2(&);
    VAR_OPERATOR_T2(|);
#define VAR_OPERATOR_T3(_x_)                                        \
    inline const __var__& operator _x_() const {                    \
        if (IsInteger(*this)) {                                     \
            _x_ Integer(*this); return *this;                       \
        }                                                           \
        if (IsFloat(*this)) {                                       \
            _x_ Float(*this); return *this;                         \
        }                                                           \
        throw std::logic_error(__func__);                           \
    }
    VAR_OPERATOR_T3(++);
    VAR_OPERATOR_T3(--);
#define VAR_OPERATOR_T4(_x_)                                        \
    inline bool operator _x_(__var__ val) const {                   \
        if (IsInteger(*this)) {                                     \
            return Integer(*this) _x_ ToInteger(val);               \
        }                                                           \
        if (IsFloat(*this)) {                                       \
            return Float(*this) _x_ ToFloat(val);                   \
        }                                                           \
        throw std::logic_error(__func__);                           \
    }
    VAR_OPERATOR_T4(<);
    VAR_OPERATOR_T4(>);
    VAR_OPERATOR_T4(<=);
    VAR_OPERATOR_T4(>=);
    /**
     * type operators
     */
    inline operator __integer__() const { return ToInteger(*this); }
    inline operator __float__  () const { return ToFloat(*this);   }
    inline operator __string__ () const { return ToString(*this);  }
    inline operator __buffer__ () const { return ToBuffer(*this);  }
    inline operator __list__   () const { return ToList(*this);    }
    inline operator __map__    () const { return ToMap(*this);     }
    /**
     * native type operators
     */
    explicit inline operator bool  () const { return ToInteger(*this); }
    explicit inline operator int   () const { return ToInteger(*this); }
    explicit inline operator float () const { return ToFloat(*this);   }
    explicit inline operator double() const { return ToFloat(*this);   }
    explicit inline operator const char*() const { return ToString(*this).data(); }
    /**
     * Type identifier
     */
    static inline auto Type(__var__ v) {
        return v.get() ? v.get()->__type() : Obj::NONE; 
    }
    /**
     * Empty
     */
    static inline bool IsEmpty(__var__ v) {
        switch(Type(v)) {
            case Obj::STRING : return String(v).empty(); 
            case Obj::BUFFER : return Buffer(v).empty(); 
            case Obj::LIST   : return List(v).empty(); 
            case Obj::MAP    : return Map(v).empty(); 
        }
        return IsUndefined(v); 
    }
    /**
     * Size
     */
    static inline auto Size(__var__ v) {
        switch(Type(v)) {
            case Obj::STRING : return String(v).size(); 
            case Obj::BUFFER : return Buffer(v).size(); 
            case Obj::LIST   : return List(v).size(); 
            case Obj::MAP    : return Map(v).size(); 
        }
        return size_t(0); 
    }
    /**
     * Test - Null
     */
    static inline __boolean__ IsUndefined(__var__ v) { return v.get() == nullptr; }
    static inline __boolean__ IsDefined  (__var__ v) { return v.get() != nullptr; }
    /**
     * Test - Types
     */
    static inline __boolean__ IsLink   (__var__ v) { return dynamic_cast<__link__*>    (v.get()); }
    static inline __boolean__ IsBoolean(__var__ v) { return dynamic_cast<__boolean__*> (v.get()); }
    static inline __boolean__ IsInteger(__var__ v) { return dynamic_cast<__integer__*> (v.get()); }
    static inline __boolean__ IsFloat  (__var__ v) { return dynamic_cast< __float__*>  (v.get()); }
    static inline __boolean__ IsString (__var__ v) { return dynamic_cast< __string__*> (v.get()); }
    static inline __boolean__ IsBuffer (__var__ v) { return dynamic_cast< __buffer__*> (v.get()); }
    static inline __boolean__ IsList   (__var__ v) { return dynamic_cast< __list__*>   (v.get()); }
    static inline __boolean__ IsMap    (__var__ v) { return dynamic_cast< __map__*>    (v.get()); }
    /**
     * Cast - Type
     */
    static inline __link__&    Link   (const __var__& v) { return *static_cast<__obj__::__link*>   (v.get()); }
    static inline __boolean__& Boolean(const __var__& v) { return *static_cast<__obj__::__boolean*>(v.get()); }
    static inline __integer__& Integer(const __var__& v) { return *static_cast<__obj__::__integer*>(v.get()); }
    static inline __float__&   Float  (const __var__& v) { return *static_cast<__obj__::__float*>  (v.get()); }
    static inline __string__&  String (const __var__& v) { return *static_cast<__obj__::__string*> (v.get()); }
    static inline __buffer__&  Buffer (const __var__& v) { return *static_cast<__obj__::__buffer*> (v.get()); }
    static inline __list__&    List   (const __var__& v) { return *static_cast<__obj__::__list*>   (v.get()); }
    static inline __map__&     Map    (const __var__& v) { return *static_cast<__obj__::__map*>    (v.get()); }
    /**
     * Convert - Type
     */
    static inline __boolean__ ToBoolean(__var__ v) { return v.get() ? v.get()->__to_integer():__boolean__(); }
    static inline __integer__ ToInteger(__var__ v) { return v.get() ? v.get()->__to_integer():__integer__(); }
    static inline __float__   ToFloat  (__var__ v) { return v.get() ? v.get()->__to_float()  : __float__ (); }
    static inline __string__  ToString (__var__ v) { return v.get() ? v.get()->__to_string() :__string__ (); }
    static inline __buffer__  ToBuffer (__var__ v) { return v.get() ? v.get()->__to_buffer() :__buffer__ (); }
    static inline __list__    ToList   (__var__ v) { return v.get() ? v.get()->__to_list()   :__list__   (); }
    static inline __map__     ToMap    (__var__ v) { return v.get() ? v.get()->__to_map()    :__map__    (); }
    /**
     * Creates a new variable
     * - if LINKS structure with links
     * - if INFO structure with new information instance
     */
    template<bool LINKS = false, bool INFO = false>
    static __var__ Clone(__var__ var)
    {
        return Clone(var,
            typename std::integral_constant<bool, LINKS>::type(),
            typename std::integral_constant<bool, INFO>::type()
        );
    }
    static __var__ Clone(__var__ var, std::false_type, std::false_type);
    static __var__ Clone(__var__ var, std::true_type,  std::false_type);
    static __var__ Clone(__var__ var, std::false_type, std::true_type );
    static __var__ Clone(__var__ var, std::true_type,  std::true_type );
    /**
     * Creates a new variable
     * - if DEEP all structure (including links)
     * - if INFO structure with new information instance
     */
    template<bool DEEP = false, bool INFO = false>
    static __var__ Copy(__var__ var)
    {
        return Copy(var,
            typename std::integral_constant<bool, DEEP>::type(),
            typename std::integral_constant<bool, INFO>::type()
        );
    }
    static inline __var__ Copy(__var__ var, std::false_type, std::false_type) {
        return Clone(var, std::false_type(), std::false_type());
    }
    static __var__ Copy(__var__ var, std::true_type,  std::false_type);
    static __var__ Copy(__var__ var, std::false_type, std::true_type );
    static __var__ Copy(__var__ var, std::true_type,  std::true_type );
    /**
     * Creates only a new variable structure without information
     * - if CLONE new structure
     * - if LINKS structure with links
     */
    template<bool CLONE = false, bool LINKS = false >
    static __var__ Strip(__var__ var) {
        return Strip(var,
            typename std::integral_constant<bool, CLONE>::type(),
            typename std::integral_constant<bool, LINKS>::type()
        );
    }
    static __var__ Strip(__var__ var, std::false_type, std::false_type);
    static __var__ Strip(__var__ var, std::false_type, std::true_type );
    static __var__ Strip(__var__ var, std::true_type,  std::false_type);
    static __var__ Strip(__var__ var, std::true_type,  std::true_type );
    /**
     * Cut all branches that are empty
     * - if CLONE new structure
     * - if LINKS structure with links
     */
    template<bool CLONE = false, bool LINKS = false >
    static __var__ Trim(__var__ var)
    {
        return Trim(var,
            typename std::integral_constant<bool, CLONE>::type(),
            typename std::integral_constant<bool, LINKS>::type()
        );
    }
    static __var__ Trim(__var__ var, std::false_type, std::false_type);
    static __var__ Trim(__var__ var, std::false_type, std::true_type );
    static __var__ Trim(__var__ var, std::true_type,  std::false_type);
    static __var__ Trim(__var__ var, std::true_type,  std::true_type );
    /**
     * get only structure
     * - if CLONE new structure
     * - if LINKS structure with links
     */
    template<bool CLONE = false, bool LINKS = false >
    static __var__ Shape(__var__ var)
    {
        return Shape(var,
            typename std::integral_constant<bool, CLONE>::type(),
            typename std::integral_constant<bool, LINKS>::type()
        );
    }
    static __var__ Shape(__var__ var, std::false_type, std::false_type);
    static __var__ Shape(__var__ var, std::false_type, std::true_type );
    static __var__ Shape(__var__ var, std::true_type,  std::false_type);
    static __var__ Shape(__var__ var, std::true_type,  std::true_type );
    /**
     * Join - remove links from var
     */
    static __var__ Join(__var__ var);
    /**
     * Assert
     */
    static inline __var__ Assert(__var__ var) {
        if (!IsDefined(var)){ throw std::logic_error(__func__); } return var;
    }
    static inline __map__& AssertMap(__var__ var) {
        if (!IsMap(var))    { throw std::logic_error(__func__); } return Map(var);
    }
    static inline __list__& AssertList(__var__ var) {
        if (!IsMap(var))    { throw std::logic_error(__func__); } return List(var);
    }
} Var;
/**
 * ------------------------------------------------------------------------------------------------
 * Utils types
 * ------------------------------------------------------------------------------------------------
 */
using KeyList = std::vector<__key__>;
using KeyMap  = std::unordered_map<__key__, __key__>;
/**
 * ------------------------------------------------------------------------------------------------
 * Utils operators
 * ------------------------------------------------------------------------------------------------
 */
inline __list__ operator+(__list__ list, __var__ var) {
    list.push_back(var);
    return list;
}
inline __var__ operator+(__obj__ obj, __var__ var) {
    return __var__(obj) + var;
}
/**
 * ------------------------------------------------------------------------------------------------ 
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* VARIABLE_H */

