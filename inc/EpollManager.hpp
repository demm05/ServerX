#pragma once
#include <stdint.h>
#include <sys/epoll.h>

class EpollManager {
public:
    EpollManager();
    ~EpollManager();

    void addFd(int fd, uint32_t events);
    void removeFd(int fd);
    void modifyFd(int fd, uint32_t events);

    int waitForEvents(struct epoll_event *events, int max_events, int timeout = -1);

private:
    int epollFd_;

    EpollManager(const EpollManager &other);
    EpollManager &operator=(const EpollManager &other);
};
