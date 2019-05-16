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
    using Super = std::shared_ptr<__interface__>;
    /**
     * super
     */ 
    using Super::Super;
    using Super::operator=;
    /**
     * constructors
     */  
    __link__() = default;
    __link__(const Super& a) : Super(a) {}
    __link__(Super &&a): Super(std::move(a)) {}
} Link;
/**
 * ------------------------------------------------------------------------------------------------
 * Containers
 * ------------------------------------------------------------------------------------------------
 * List
 * ----------------------------------------------------------------------------
 **/
typedef class __list__ : public std::vector<Link> {
public:
    using Super = std::vector<Link>;
    /**
     * super
     */ 
    using Super::Super;
    using Super::operator=;
    /**
     * operators
     */
    inline __list__ operator + (Link b) {
        auto res = __list__(*this);
        res.emplace_back(b);
        return std::move(res);
    }
    /**
     * utilities
     */
    inline __list__& insert(Super&& list) {
        for(auto& v : list) {
            emplace_back(std::move(v));
        }
        list.clear();     
        return *this;
    }
    inline __link__ remove() {
        if(empty()) {
            return Link();
        }     
        auto tmp = std::move(front()); erase(begin()); return tmp;
    }
} List;
typedef class __map__ : public std::unordered_map<Key, Link> {
    typedef std::unordered_map<Key, Link> Base;
public:
    using Super = std::unordered_map<Key, Link> ;
    /**
     * super
     */ 
    using Super::Super;
    using Super::operator=;
    /**
     * constructors
     */    
    __map__() = default;
    __map__(const Super& a) : Super(a) {}
    __map__(Super &&a): Super(std::move(a)) {}
} Map;
/**
 * ------------------------------------------------------------------------------------------------
 * Interface with objects
 * ------------------------------------------------------------------------------------------------
 **/
class __interface__ {
    friend class __var__;
    friend class __obj__;
    virtual __link__ operator[](__string__ val) {
        throw std::logic_error(__func__);
    }
    virtual __link__ operator[](__integer__ val) {
        throw std::logic_error(__func__);
    }
    virtual __integer__ to_integer() {
        throw std::logic_error(__func__);
    }
    virtual __float__ to_float() {
        throw std::logic_error(__func__);
    }
    virtual __string__ to_string() {
        throw std::logic_error(__func__);
    }
    virtual __buffer__ to_buffer() {
        throw std::logic_error(__func__);
    }
    virtual __list__ to_list() {
        throw std::logic_error(__func__);
    }
    virtual __map__ to_map() {
        throw std::logic_error(__func__);
    }
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
        __boolean(const __boolean__& b): __boolean__(b) {}
        __boolean(__boolean__&& b): __boolean__(std::move(b)) {}
        /**
         * interface
         */
        inline __integer__ to_integer() {
            return __integer__(this->_val);
        }
        inline __float__ to_float() {
            return __float__(this->_val);
        }
        inline __string__ to_string() {
            return __string__::ValueOf(*this);
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
        __integer(const __integer__& i): __integer__(i) {}
        __integer(__integer__&& i): __integer__(std::move(i)) {}
        /**
         * interface
         */
        inline __integer__ to_integer() {
            return *this;
        }
        inline __float__ to_float() {
            return __float__(this->_val);
        }
        inline __string__ to_string() {
            return __string__::ValueOf(*this);
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
        __float(const __float__& f): __float__(f) {}
        __float(__float__&& f): __float__(std::move(f)) {}
        /**
         * interface
         */
        inline __float__ to_float() {
            return *this;
        }
        inline __integer__ to_integer() {
            return __integer__(this->_val);
        }
        inline __string__ to_string() {
            return __string__::ValueOf(*this);
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
        __string(const __string__& s): __string__(s) {}
        __string(__string__&& s): __string__(std::move(s)) {}
        /**
         * interface
         */
        inline __string__ to_string() {
            return *this;
        }
        inline __float__ to_float() {
            return __float__::ValueOf(*this);
        }
        inline __integer__ to_integer() {
            return __integer__::ValueOf(*this);
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
        __buffer(const __buffer__& b): __buffer__(b) {}
        __buffer(__buffer__&& b): __buffer__(std::move(b)) {}
        /**
         * interface
         */
        inline __buffer__ to_buffer() {
            return *this;
        }
        inline __float__ to_float() {
            return __float__(0);
        }
        inline __integer__ to_integer() {
            return __integer__::ValueOf(*this);
        }
        inline __string__ to_string() {
            return __string__::ValueOf(*this);
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
        __list(const __list__& l): __list__(l) { }
        __list(__list__&& l): __list__(std::move(l)) {}
        /**
         * interface
         */
        inline __link__ operator[](__string__ val) {
            try {
                return at(__integer__::ValueOf(val));
            } catch (...) {
                return __link__();
            }
        }
        inline __link__ operator[](__integer__ val) {
            try {
                return at(val);
            } catch (...) {
                return __link__();
            }
        }
        inline __list__ to_list() {
            return *this;
        }
        inline __map__ to_map() {
            __map__ m;
            for (__integer__ i = 0; size_t(i) < size(); ++i) {
                m[__string__::ValueOf(i)] = at(i);
            }
            return m;
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
        inline __link__ operator[](__string__ val) {
            try {
                return at(val);
            } catch (...) {
                return __link__();
            }
        }
        inline __link__ operator[](__integer__ val) {
            try {
                return at(__string__::ValueOf(val));
            } catch (...) {
                return __link__();
            }
        }
        inline __map__ to_map() {
            return *this;
        }
        inline __list__ to_list() {
            __list__ l;
            for (auto& v : * this) {
                l.push_back(v.second);
            }
            return l;
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
        __link(const __link__ & l): __link__(l) {}
        __link(__link__ && l): __link__(std::move(l)) {}
        /**
         * operators 
         */
        inline __link__ operator[](__string__ val) {
            return __link__::operator bool() ? get()->operator[](val) : __link__();
        }
        inline __link__ operator[](__integer__ val) {
            return __link__::operator bool() ? get()->operator[](val) : __link__();
        }
        inline __integer__ to_integer() {
            return __link__::operator bool() ? get()->to_integer() : __integer__();
        }
        inline __float__ to_float() {
            return __link__::operator bool() ? get()->to_float() : __float__();
        }
        inline __string__ to_string() {
            return __link__::operator bool() ? get()->to_string() : __string__();
        }
        inline __buffer__ to_buffer() {
            return __link__::operator bool() ? get()->to_buffer() : __buffer__();
        }
        inline __list__ to_list() {
            return __link__::operator bool() ? get()->to_list() : __list__();
        }
        inline __map__ to_map() {
            return __link__::operator bool() ? get()->to_map() : __map__();
        }
    };
public:
    __obj__() = delete;
    /**
     * native types constructors
     */
    __obj__(nullptr_t) : __link__() {
    }
    __obj__(bool v) : __link__(std::make_shared<__boolean>(v)) {
    }
    __obj__(int v) : __link__(std::make_shared<__integer>(v)) {
    }
    __obj__(const char* v) : __link__(std::make_shared<__string>(v)) {
    }
    __obj__(float v) : __link__(std::make_shared<__float>(v)) {
    }
    __obj__(double v) : __link__(std::make_shared<__float>(v)) {
    }
    /**
     * base types constructors
     */
    __obj__(__boolean__ v) : __link__(std::make_shared<__boolean>(v)) {
    }
    __obj__(__integer__ v) : __link__(std::make_shared<__integer>(v)) {
    }
    __obj__(__float__ v) : __link__(std::make_shared<__float>(v)) {
    }
    __obj__(__string__ v) : __link__(std::make_shared<__string>(v)) {
    }
    __obj__(__buffer__ v) : __link__(std::make_shared<__buffer>(v)) {
    }
    __obj__(__list__ v) : __link__(std::make_shared<__list>(v)) {
    }
    __obj__(__map__ v) : __link__(std::make_shared<__map>(v)) {
    }
    __obj__(std::initializer_list<__map::value_type> l) : __link__(std::make_shared<__map>(l)){
    }
    __obj__(std::initializer_list<__list::value_type> l) : __link__(std::make_shared<__list>(l)){
    }
    /**
     * Null
     */
    static inline __link__ Null() {
        return __link__();
    }
    /**
     * Link
     */
    template<typename...Args>
    static inline __link__ Link(Args &&...args) {
        return std::make_shared<__link>(std::forward<Args>(args)...);
    }
    /**
     * Boolean
     */
    template<typename...Args>
    static inline __link__ Boolean(Args &&...args) {
        return std::make_shared<__boolean>(std::forward<Args>(args)...);
    }
    static inline __link__ Boolean(__link__ l) {
        return std::make_shared<__boolean>(l->to_integer());
    }
    /**
     * Integer
     */
    template<typename...Args>
    static inline __link__ Integer(Args &&...args) {
        return std::make_shared<__integer>(std::forward<Args>(args)...);
    }
    static inline __link__ Integer(__link__ l) {
        return std::make_shared<__integer>(l->to_integer());
    }
    /**
     * Float
     */
    template<typename...Args>
    static inline __link__ Float(Args &&...args) {
        return std::make_shared<__float>(std::forward<Args>(args)...);
    }
    static inline __link__ Float(__link__ l) {
        return std::make_shared<__float>(l->to_float());
    }
    /**
     * String
     */
    template<typename...Args>
    static inline __link__ String(Args &&...args) {
        return std::make_shared<__string>(std::forward<Args>(args)...);
    }
    static inline __link__ String(__link__ l) {
        return std::make_shared<__string>(l->to_string());
    }
    /**
     * Buffer
     */
    template<typename...Args>
    static inline __link__ Buffer(Args &&...args) {
        return std::make_shared<__buffer>(std::forward<Args>(args)...);
    }
    static inline __link__ Buffer(__link__ l) {
        return std::make_shared<__buffer>(l->to_buffer());
    }
    /**
     * List
     */
    template<typename...Args>
    static inline __link__ List(Args &&...args) {
        return std::make_shared<__list>(std::forward<Args>(args)...);
    }
    static inline __link__ List(std::initializer_list<__link__> l) {
        return std::make_shared<__list>(l);
    }
    static inline __link__ List(__link__ l) {
        return std::make_shared<__list>(l->to_list());
    }
    /**
     * Map
     */
    template<typename...Args>
    static inline __link__ Map(Args &&...args) {
        return std::make_shared<__map>(std::forward<Args>(args)...);
    }
    static inline __link__ Map(std::initializer_list<std::pair<const __string__, __link__>> l) {
        return std::make_shared<__map>(l);
    }
    static inline __link__ Map(__link__ l) {
        return std::make_shared<__map>(l->to_map());
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
        return __var__(Link::operator bool() ? get()->operator[](val) : __link__());
    }
    inline const __var__ operator[](const char * val) const {
        return __var__(Link::operator bool() ? get()->operator[](__string__(val)) : __link__());
    }
    inline const __var__ operator[](__integer__ val) const {
        return __var__(Link::operator bool()? get()->operator[](val) : __link__());
    }
    inline const __var__ operator[](int val) const {
        return __var__(Link::operator bool() ? get()->operator[](__integer__(val)) : __link__());
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
            if (IsNull(*this)) {
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
            return Obj::Integer(Integer(*this) + ToInteger(val));
        }
        if (IsFloat(*this)) {
            return Obj::Float(Float(*this) + ToFloat(val));
        }
        if (IsString(*this)) {
            return Obj::String(String(*this) + ToString(val));
        }
        throw std::logic_error(__func__);
    }
#define VAR_OPERATOR_T1(_x_)                                        \
    inline __var__ operator _x_(__var__ val) const {                \
        if (IsInteger(*this)) {                                     \
            return Obj::Integer(Integer(*this) _x_ ToInteger(val)); \
        }                                                           \
        if (IsFloat(*this)) {                                       \
            return Obj::Float(Float(*this) _x_ ToFloat(val));       \
        }                                                           \
        throw std::logic_error(__func__);                           \
    }
    VAR_OPERATOR_T1(-);
    VAR_OPERATOR_T1(*);
    VAR_OPERATOR_T1(/);
#define VAR_OPERATOR_T2(_x_)                                        \
    inline __var__ operator _x_(__var__ val) const {                \
        if (IsInteger(*this)) {                                     \
            return Obj::Integer(Integer(*this) _x_ ToInteger(val)); \
        }                                                           \
        throw std::logic_error(__func__);                           \
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
    inline operator __integer__() const {
        return ToInteger(*this);
    }
    inline operator __float__() const {
        return ToFloat(*this);
    }
    inline operator __string__() const {
        return ToString(*this);
    }
    inline operator __buffer__() const {
        return ToBuffer(*this);
    }
    inline operator __list__() const {
        return ToList(*this);
    }
    inline operator __map__() const {
        return ToMap(*this);
    }
    /**
     * native type operators
     */
    explicit inline operator bool() const {
        return ToInteger(*this);
    }
    explicit inline operator int() const {
        return ToInteger(*this);
    }
    explicit inline operator float() const {
        return ToFloat(*this);
    }
    explicit inline operator double() const {
        return ToFloat(*this);
    }
    explicit inline operator const char*() const {
        return ToString(*this).data();
    }
    /**
     *  Null
     */
    static inline __boolean__ IsNull(__var__ v) {
        return v.get() == nullptr;
    }
    static inline __boolean__ IsUndefined(__var__ v) {
        return v.get() == nullptr;
    }
    static inline __boolean__ IsDefined(__var__ v) {
        return v.get() != nullptr;
    }
    /**
     *  Empty
     */
    static inline __boolean__ IsEmpty(__var__ v) {
        if(IsUndefined(v)){
            return true;
        }
        if (IsMap(v)){
            return Map(v).empty();
        }
        if (IsList(v)){
            return List(v).empty();
        }
        if (IsString(v)){
            return String(v).empty();
        }
        if (IsBuffer(v)){
            return Buffer(v).empty();
        }
        return false;
    }
    /**
     *  Size
     */
    static inline __integer__ Size(__var__ v) {
        if (IsMap(v)){
            return Map(v).size();
        }
        if (IsList(v)){
            return List(v).size();
        }
        if (IsString(v)){
            return String(v).size();
        }
        if (IsBuffer(v)){
            return Buffer(v).size();
        }
        return 0;
    }
    /**
     * Link
     */
    static inline __boolean__ IsLink(__var__ v) {
        return dynamic_cast< __link__*> (v.get());
    }
    static inline __link__& Link(const __var__& v) {
        return *static_cast<__obj__::__link*> (v.get());
    }
    /**
     * Boolean
     */
    static inline __boolean__ IsBoolean(__var__ v) {
        return dynamic_cast< __boolean__*> (v.get());
    }
    static inline __boolean__& Boolean(const __var__& v) {
        return *static_cast<__obj__::__boolean*> (v.get());
    }
    static inline __boolean__ ToBoolean(__var__ v) {
        return v ? __boolean__() : v.get()->to_integer();
    }
    /**
     * Integer
     */
    static inline __boolean__ IsInteger(__var__ v) {
        return dynamic_cast< __integer__*> (v.get());
    }
    static inline __integer__& Integer(const __var__& v) {
        return *static_cast<__obj__::__integer*> (v.get());
    }
    static inline __integer__ ToInteger(__var__ v) {
        return (v.get() == nullptr) ? __integer__() : v.get()->to_integer();
    }
    /**
     * Float
     */
    static inline __boolean__ IsFloat(__var__ v) {
        return dynamic_cast< __float__*> (v.get());
    }
    static inline __float__& Float(const __var__& v) {
        return *static_cast<__obj__::__float*> (v.get());
    }
    static inline __float__ ToFloat(__var__ v) {
        return (v.get() == nullptr) ? __float__() : v.get()->to_float();
    }
    /**
     * String
     */
    static inline __boolean__ IsString(__var__ v) {
        return dynamic_cast< __string__*> (v.get());
    }
    static inline __string__& String(const __var__& v) {
        return *static_cast<__obj__::__string*> (v.get());
    }
    static inline __string__ ToString(__var__ v) {
        return (v.get() == nullptr) ? __string__() : v.get()->to_string();
    }
    /**
     * Buffer
     */
    static inline __boolean__ IsBuffer(__var__ v) {
        return dynamic_cast< __buffer__*> (v.get());
    }
    static inline __buffer__& Buffer(const __var__& v) {
        return *static_cast<__obj__::__buffer*> (v.get());
    }
    static inline __buffer__ ToBuffer(__var__ v) {
        return (v.get() == nullptr) ? __buffer__() : v.get()->to_buffer();
    }
    /**
     * List
     */
    static inline __boolean__ IsList(__var__ v) {
        return dynamic_cast< __list__*> (v.get());
    }
    static inline __list__& List(const __var__& v) {
        return *static_cast<__obj__::__list*> (v.get());
    }
    static inline __list__ ToList(__var__ v) {
        return (v.get() == nullptr) ? __list__() : v.get()->to_list();
    }
    /**
     * Map
     */
    static inline __boolean__ IsMap(__var__ v) {
        return dynamic_cast< __map__*> (v.get());
    }
    static inline __map__& Map(const __var__& v) {
        return *static_cast<__obj__::__map*> (v.get());
    }
    static inline __map__ ToMap(__var__ v) {
        return (v.get() == nullptr) ? __map__() : v.get()->to_map();
    }
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
    static __var__ Clone(__var__ var, std::true_type, std::false_type);
    static __var__ Clone(__var__ var, std::false_type, std::true_type);
    static __var__ Clone(__var__ var, std::true_type, std::true_type);
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
    static __var__ Copy(__var__ var, std::true_type, std::false_type);
    static __var__ Copy(__var__ var, std::false_type, std::true_type);
    static __var__ Copy(__var__ var, std::true_type, std::true_type);
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
    static __var__ Strip(__var__ var, std::false_type, std::true_type);
    static __var__ Strip(__var__ var, std::true_type, std::false_type);
    static __var__ Strip(__var__ var, std::true_type, std::true_type);
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
    static __var__ Trim(__var__ var, std::false_type, std::true_type);
    static __var__ Trim(__var__ var, std::true_type, std::false_type);
    static __var__ Trim(__var__ var, std::true_type, std::true_type);
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
    static __var__ Shape(__var__ var, std::false_type, std::true_type);
    static __var__ Shape(__var__ var, std::true_type, std::false_type);
    static __var__ Shape(__var__ var, std::true_type, std::true_type);
    /**
     * Join - remove links from var
     */
    static __var__ Join(__var__ var);
    /**
     * Move
     */
    static inline __var__ Move(__var__ var) {
        return std::move(var);
    }
    /**
     * Assert
     */
    static inline __var__ Assert(__var__ var) {
        if (!IsDefined(var)) {
            throw std::logic_error(__func__);
        }
        return var;
    }
    static inline __map__& AssertMap(__var__ var) {
        if (!IsMap(var)) {
            throw std::logic_error(__func__);
        }
        return Map(var);
    }
    static inline __list__& AssertList(__var__ var) {
        if (!IsMap(var)) {
            throw std::logic_error(__func__);
        }
        return List(var);
    }
} Var;
/**
 * ------------------------------------------------------------------------------------------------
 * Utils types
 * ------------------------------------------------------------------------------------------------
 */
using ListKey = std::vector<Key>;
using MapKey  = std::unordered_map<Key, Key>;
/**
 * ------------------------------------------------------------------------------------------------
 * Utils operators
 * ------------------------------------------------------------------------------------------------
 */
inline List operator+(List list, Var var) {
    list.push_back(var);
    return list;
}
inline Var operator+(Obj obj, Var var) {
    return Var(obj) + var;
}
/**
 * ------------------------------------------------------------------------------------------------ 
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* VARIABLE_H */

