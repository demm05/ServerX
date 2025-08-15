#include "EpollManager.hpp"
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <unistd.h>

EpollManager::EpollManager() : epollFd_(-1) {
    epollFd_ = epoll_create1(0);
    if (epollFd_ < 0) {
        throw std::runtime_error("Failed to create epoll instance: " +
                                 std::string(strerror(errno)));
    }
}

EpollManager::~EpollManager() {
    if (epollFd_ >= 0) {
        close(epollFd_);
    }
}

void EpollManager::addFd(int fd, uint32_t events) {
    struct epoll_event eventStruct;
    eventStruct.events = events;
    eventStruct.data.fd = fd;

    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &eventStruct) < 0) {
        throw std::runtime_error("Failed to add fd to epoll: " + std::string(strerror(errno)));
    }
}

void EpollManager::removeFd(int fd) {
    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, NULL) < 0) {
        throw std::runtime_error("Failed to remove fd to epoll: " + std::string(strerror(errno)));
    }
}

void EpollManager::modifyFd(int fd, uint32_t events) {
    struct epoll_event eventStruct;
    eventStruct.events = events;
    eventStruct.data.fd = fd;

    if (epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &eventStruct) < 0) {
        throw std::runtime_error("Failed to modify fd in epoll: " + std::string(strerror(errno)));
    }
}

int EpollManager::waitForEvents(struct epoll_event *events, int maxEvents, int timeout) {
    return epoll_wait(epollFd_, events, maxEvents, timeout);
}
