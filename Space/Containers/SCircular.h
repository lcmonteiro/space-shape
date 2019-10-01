/**
 * ------------------------------------------------------------------------------------------------
 * File:   Circlar.h
 * Author: LuisMonteiro
 *
 * Created on Oct 1, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef CIRCULAR_H
#define	CIRCULAR_H
/**
 * std
 */
#include <list>
/**
 * -----------------------------------------------------------------------------------------------
 * Circular
 * -----------------------------------------------------------------------------------------------
 */
template <typename T, size_t N=1>
class Circular : public std::list<T> {
    using base = std::list<T>;
public:
    /**
     * ------------------------------------------------------------------------
     * constructors
     * ------------------------------------------------------------------------
     */
    Circular()                  = default;
    Circular(const Circular& v) = default;
    Circular(Circular&& v)      = default;
    /**
     * ------------------------------------------------------------------------
     * new
     * ------------------------------------------------------------------------
     */
    template <class... Args>
    void emplace_back(Args&&... args) {
        base::emplace_back(std::forward<Args>(args)...);
        while (base::size() > N)
            base::pop_front();
    }
    template <class... Args>
    void emplace_front(Args&&... args) {
        base::emplace_front(std::forward<Args>(args)...);
        while (base::size() > N)
            base::pop_back();
    }
protected:
    /**
     * ------------------------------------------------------------------------
     * hidden
     * ------------------------------------------------------------------------
     */
    using base::merge;
    using base::push_back;
    using base::push_front;
    using base::emplace_back;
    using base::emplace_front;
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* CIRCULAR_H */

