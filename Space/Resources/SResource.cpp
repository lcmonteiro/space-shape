/**
 * ------------------------------------------------------------------------------------------------
 * File:   SResource.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 10:12 AM
 * ------------------------------------------------------------------------------------------------
 */
#include <unistd.h>
#include <poll.h>
/**
 * std
 */
#include <cstring>
/**
 * space
 */
#include "SResource.h"
#include "SStream.h"
/**
 * ----------------------------------------------------------------------------
 * constructors
 * ----------------------------------------------------------------------------
 */
SResource::SResource(Handler io): __ih(io), __oh(io) {
}
SResource::SResource(Handler i, Handler o): __ih(i), __oh(o) {
}
/**
 * ----------------------------------------------------------------------------
 * destructor
 * ----------------------------------------------------------------------------
 */
SResource::~SResource() {
    if(__ih > 0) close(__ih);
    if(__oh > 0) if (__oh != __ih) close(__oh);
}
/**
 * ----------------------------------------------------------------------------
 * wait for one
 * ----------------------------------------------------------------------------
 */
Integer SResource::WaitFor(SResource* pResource, Float timeout) {
    struct pollfd fds[1];
    /**
     * prepare
     */
    int n = 0;
    if (pResource && pResource->ihandler() > 0) {
        fds[0] = {
            .fd     = pResource->ihandler(),
            .events = POLLIN | POLLERR | POLLHUP
        };
        ++n;
    }
    /**
     * wait
     */
    int r;
    if ((r = poll(fds, n,
        timeout < 0 ? 0 : timeout < INFINITY ? int(double(timeout) * 1000) : -1)) < 0) {
        throw ResourceException(String("select: ") + strerror(errno));
    }
    /**
     * check
     */
    if (r > 0) {
        if (fds[0].revents & fds[0].events) {
            return 1;
        }
    }
    return 0;
}
/**
 * ----------------------------------------------------------------------------
 * wait for a list
 * ----------------------------------------------------------------------------
 */
std::vector<Integer> SResource::WaitFor(RList list, Float timeout) {
    std::vector<pollfd> fds(list.size());
    /**
     * prepare
     */
    int i = 0;
    for (auto& p : list) {
        if (p && p->ihandler() > 0) {
            fds[i] = {
                .fd      = p->ihandler(),
                .events  = POLLIN | POLLERR | POLLHUP,
                .revents = 0
            };
            ++i;
        }
    }
    /**
     * wait
     */
    int r;
    if ((r = poll(fds.data(), fds.size(),
        timeout < 0 ? 0 : timeout < INFINITY ? int(double(timeout) * 1000) : -1)) < 0) {
        throw ResourceException(String("select: ") + strerror(errno));
    }
    /**
     * check
     */
    std::vector<Integer> out;
    if (r > 0) {
        int i = 0, k = 0;
        for (auto& p : list) {
            if (p && p->ihandler() > 0) {
                if (fds[i].revents & fds[i].events) {   
                    out.emplace_back(k);
                }
                ++i;
            }
            ++k;
        }
    }
    return out;
}
/**
 * wait for a map
 */
std::vector<Integer> SResource::WaitFor(RMap map, Float timeout) {
    std::vector<pollfd> fds(map.size());
    /**
     * prepare
     */
    int i = 0;
    for (auto& p : map) {
        if (p.second && p.second->ihandler() > 0) {
            fds[i] = {
                .fd      =  p.second->ihandler(),
                .events  = POLLIN | POLLERR | POLLHUP,
                .revents = 0,
            };
            ++i;
        }
    }
    /**
     * wait
     */
    int r;
    if ((r = poll(fds.data(), fds.size(),
        timeout < 0 ? 0 : timeout < INFINITY ? int(double(timeout) * 1000) : -1)) < 0) {
        throw ResourceException(String("select: ") + strerror(errno));
    }
    /**
     * check
     */
    std::vector<Integer> out;
    if (r > 0) {
        int i = 0;
        for (auto& p : map) {
            if (p.second && p.second->ihandler() > 0) {
                if (fds[i].revents & fds[i].events) {
                    out.emplace_back(p.first);
                }
                ++i;
            }
        }
    }
    return out;
}
/**
 * ----------------------------------------------------------------------------
 * IO functions
 * ----------------------------------------------------------------------------
 */
size_t SResource::__write(void* p, size_t s) {
    auto n = ::write(__oh, p, s);
    if (n < 0) {
        throw OResourceException(String("write: ") + strerror(errno));
    }
    return n;
}
size_t SResource::__read(void* p, size_t s) {
    auto n = ::read(__ih, p, s);
    if (n < 0) {
        throw IResourceException(String("read: ") + strerror(errno));
    }
    return n;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */