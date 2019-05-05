/**
 * ------------------------------------------------------------------------------------------------
 * File:   Native.h
 * Author: Luis Monteiro
 *
 * Created on April 21, 2015, 3:43 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef NATIVE_H
#define NATIVE_H
/**
 * std
 */
#include <sstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
/**
 * ------------------------------------------------------------------------------------------------
 * native type template
 * ------------------------------------------------------------------------------------------------
 */
template <class T> class Native {
public:
        /**
         * constructors
         */
        Native(T val) : _val(val) {
        }
        Native() : _val(0) {
        }
        /**
         */
        virtual ~Native() {
        }
        /**
         * operators
         */
        inline Native<T>& operator++() {
                ++this->_val;
                return *this;
        }
        inline Native<T>& operator--() {
                --this->_val;
                return *this;
        }
        inline Native<T> operator++(int) {
                Native<T> result(this->_val);
                ++this->_val;
                return result;
        }
        inline Native<T> operator--(int) {
                Native<T> result(this->_val);
                --this->_val;
                return result;
        }

#define BASE_OPERATOR_T1(_x_)                                   \
        inline Native<T> operator _x_(Native<T> val) {          \
                return Native<T>(this->_val _x_ val._val);      \
        }                                                       \
        inline Native<T> operator _x_(T val) {                  \
                return Native<T>(this->_val _x_ val);           \
        }
        BASE_OPERATOR_T1(^);
        BASE_OPERATOR_T1(&);
        BASE_OPERATOR_T1(|);
        BASE_OPERATOR_T1(+);
        BASE_OPERATOR_T1(-);
        BASE_OPERATOR_T1(*);
        BASE_OPERATOR_T1(/);

        inline Native<T>& operator<<=(const int val) {
                this->_val <<= val;
                return *this;
        }
        inline Native<T>& operator>>=(const int val) {
                this->_val >>= val;
                return *this;
        }

#define BASE_OPERATOR_T2(_x_)                                   \
        inline Native<T>& operator _x_(const Native<T> val) {   \
                this->_val _x_ val._val;                        \
                return *this;                                   \
        }                                                       \
        inline Native<T>& operator _x_(const T val) {           \
                this->_val _x_ val;                             \
                return *this;                                   \
        }
        BASE_OPERATOR_T2( =);
        BASE_OPERATOR_T2( +=);
        BASE_OPERATOR_T2( -=);
        BASE_OPERATOR_T2( *=);
        BASE_OPERATOR_T2( /=);
        BASE_OPERATOR_T2( |=);
        BASE_OPERATOR_T2( &=);
        BASE_OPERATOR_T2( ^=);
        /**
         *
         */
        inline operator T() const {
                return _val;
        }
        /**
         * references
         */
        inline T& ref() {
                return _val;
        }
        inline const T* data() {
                return (const T*) &_val;
        }
        /**
         * sizeof
         */
        static inline size_t size() {
                return sizeof (T);
        }
protected:
        T _val;
};
/**
 * ------------------------------------------------------------------------------------------------
 * native types
 * ------------------------------------------------------------------------------------------------
 */
using __B__ = Native<bool>;
using __I__ = Native<int>;
using __F__ = Native<double>;
using __S__ = std::string;
using __V__ = std::vector<unsigned char>;
/**
 * ----------------------------------------------------------------------------
 * bolean
 * ----------------------------------------------------------------------------
 */
typedef class __boolean__ : public __B__ {
public:
        using __B__::__B__;
        /**
         * constructors
         */
        __boolean__() : __B__() {
        }
        __boolean__(__B__&& var) : __B__(var) {
        }
        __boolean__(const __B__& var) : __B__(var) {
        }
} Boolean;
/**
 * ----------------------------------------------------------------------------
 * Integer
 * ----------------------------------------------------------------------------
 */
typedef class __integer__ : public __I__ {
public:
        using __I__::__I__;
        /**
         * constructors
         */
        __integer__() : __I__() {
        }
        __integer__(__I__&& var) : __I__(var) {
        }
        __integer__(const __I__& var) : __I__(var) {
        }
        /**
         * translations
         */
        static inline __integer__ ValueOf(__F__ val) {
                return __integer__(val);
        }
        static inline __integer__ ValueOf(__S__ val) {
                size_t n = 0;
                if (val.empty()) {
                        return __integer__();
                }
                __integer__ v(std::stoi(val, &n));
                if (n != val.size()) {
                        throw std::invalid_argument(__func__);
                }
                return v;
        }
        static inline __integer__ ValueOf(__V__ val) {
                __I__ result = 0;
                for (__V__::const_iterator i = val.begin(); i != val.end(); ++i) {
                        result <<= 8;
                        result |= (int) *i;
                }
                return __integer__(result);
        }
        static inline __integer__ ValueOf(__V__::const_iterator beg, __V__::const_iterator end) {
                __I__ result = 0;
                for (auto it = beg; it != end; ++it) {
                        result <<= 8;
                        result |= (int) *it;
                }
                return __integer__(result);
        }
} Integer;
/**
 * ----------------------------------------------------------------------------
 * Float
 * ----------------------------------------------------------------------------
 */
typedef class __float__ : public __F__ {
public:
        using __F__::__F__;
        /**
         * constructors
         */
        __float__() : __F__() {
        }
        __float__(__F__&& var) : __F__(var) {
        }
        __float__(const __F__& var) : __F__(var) {
        }
        /**
         * translations
         */
        static __F__ ValueOf(__I__ val) {
                return __F__(val);
        }
        static __float__ ValueOf(__S__ val) {
                size_t n = 0;
                if (val.empty()) {
                        return __float__();
                }
                __float__ v(std::stof(val, &n));
                if (n != val.size()) {
                        throw std::invalid_argument(__func__);
                }
                return v;
        }
        static __float__ ValueOf(__V__ val) {
                __F__ result = 0;
                for (__V__::const_iterator i = val.begin(); i != val.end(); ++i) {
                        static_cast<__I__>(result) <<= 8;
                        static_cast<__I__>(result) |= (int) *i;
                }
                return __float__(result);
        }
} Float;
/**
 * ----------------------------------------------------------------------------
 * String
 * ----------------------------------------------------------------------------
 */
typedef class __string__ : public __S__ {
public:
        using __S__::__S__;
        /**
         * constructors
         */        
        __string__() : __S__() {
        }
        __string__(__S__&& var) : __S__(var) {
        }
        __string__(const __S__& var) : __S__(var) {
        }        
        /**
         * translations
         */
        static __string__ ValueOf(__B__ val) {
                std::ostringstream result;
                result << val;
                return result.str();
        }
        static __string__ ValueOf(__I__ val) {
                std::ostringstream result;
                result << val;
                return result.str();
        }
        static __string__ ValueOf(__I__ val, size_t size) {
                std::ostringstream result;
                result << std::setfill('0') << std::setw (size) << val;
                return result.str();
        }
        static __string__ ValueOf(__F__ val) {
                std::ostringstream result;
                result << std::setprecision(12) << val;
                return result.str();
        }
        static __string__ ValueOf(__F__ val, size_t precision) {
                std::ostringstream result;
                result << std::setprecision(precision) << std::fixed << val;
                return result.str();
        }
        static __string__ ValueOf(__V__ val, int n = 1, __string__ spacer = "") {
                std::ostringstream result;
                result.width(2);
                int i = n;
                for (__V__::const_iterator it = val.begin(); it != val.end(); ++it, --i) {
                        if (!i) {
                                result << spacer;
                                i = n;
                        }
                        result << std::hex << std::setfill('0') << std::setw(2);
                        result << (int) *it;
                }
                return result.str();
        }
        /**
         * templates
         */
        template< typename T >
        static __string__ hexOf(T val) {
                std::stringstream stream;
                stream << std::uppercase << std::hex << val;
                return stream.str();
        }
        template<typename... Ts>
        static __string__ build(Ts... args){
                std::ostringstream result;
                char steps[] = { 
                        (result << args, 'y')... 
                };
                return result.str();
        }
        /**
         * utilities
         */
        inline __string__& update(__string__::value_type from, __string__::value_type to) {
                std::replace(begin(), end(), from, to);
                return *this;
        }
        inline __string__& update(
                __string__::value_type beg, __string__::value_type end,  
                __string__::value_type from, __string__::value_type to
        ) {
                auto b = find(beg), e = find(end);
                if(b == std::string::npos){
                        throw std::range_error(std::string("not found(beg=") + beg + ")");
                }
                if(e == std::string::npos){
                        throw std::range_error(std::string("not found(end=") + end + ")");
                }
                std::replace(std::next(begin() + b), begin() + e, from, to);
                return *this;
        }
        /**
         *  push and pop on back
         */
        inline __string__& detete(size_t n) {
                for (auto it = rbegin(), end = rend(); it != end && n > 0; ++it, --n) {
                         pop_back();
                }
                return *this;
        }
} String;
/**
 * utils
 */
namespace std {
        template <>
        struct hash<String> {
                inline std::size_t operator()(const String& k) const {
                        std::hash<std::string> h;
                        return h(k);
                }
        };
}
/**
 * ----------------------------------------------------------------------------
 * Buffer
 * ----------------------------------------------------------------------------
 */
typedef class __buffer__ : public __V__ {
public:
        using __V__::__V__;
        /**
         * constructors
         */
        __buffer__() : __V__() {
        }
        __buffer__(__V__&& var) : __V__(var) {
        }
        __buffer__(const __V__& var) : __V__(var) {
        }
        __buffer__(value_type* ptr, size_type sz) : __V__(ptr, ptr + sz) {
        }        
        /**
         * operators
         */
        inline __buffer__& operator^=(const __buffer__ val) {
                for (size_t i = 0; i < size() && i < val.size(); i++) {
                        at(i) ^= val.at(i);
                }
                return *this;
        }
        inline __buffer__ operator^(__buffer__ val) {
                __buffer__ ret(*this);
                return ret ^= val;
        }
        /**
         *  push and pop on back
         */
        inline size_t push(__buffer__ buf) {
                for (auto it = buf.begin(), end = buf.end(); it != end; ++it) {
                        emplace_back(*it);
                }
                return buf.size();
        }
        inline __buffer__ pop(size_t n) {
                __buffer__ ret(n);
                for (auto it = ret.rbegin(), end = ret.rend(); it != end; pop_back(), ++it) {
                        *it = back();
                }
                return ret;
        }
        /**
         * update
         */
        inline __buffer__& update(size_t index, __buffer__::value_type val) {
                assign(index, val);
                return *this;
        }
        /**
         * translations
         */
        static __buffer__ ValueOf(__S__ val, char spacer) {
                __V__ result;
                std::istringstream ss(val);
                std::string word;
                while (std::getline(ss, word, spacer)) {
                        result.push_back(std::stoi(word));
                }
                return __buffer__(result);
        }
        static __buffer__ ValueOf(__S__ val) {
                __V__ result;
                std::istringstream ss(val);
                while (ss) {
                        __buffer__::value_type val;
                        ss >> std::hex >> val;
                        result.push_back(std::move(val));
                }
                return __buffer__(result);
        }
        static __buffer__ ValueOf(__I__ val, int size = __I__::size()) {
                __V__ result(size);
                for (__V__::iterator i = result.end(); i != result.begin(); val >>= 8) {
                        *(--i) = (unsigned char) val;
                }
                return __buffer__(result);
        }

} Buffer;
/**
 * ----------------------------------------------------------------------------
 * Others
 * ----------------------------------------------------------------------------
 * Void
 */
typedef void Void;
/**
 * Key
 */
typedef String Key;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* NATIVE_H */

