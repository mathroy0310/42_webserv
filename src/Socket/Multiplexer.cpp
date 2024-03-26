/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:46:58 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/26 17:39:55 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/Multiplexer.hpp"

Multiplexer::Multiplexer(void) {}

Multiplexer::~Multiplexer(void) {}

void Multiplexer::addFd(int fd, short events)
{
    struct pollfd pollfd;
    pollfd.fd = fd;
    pollfd.events = events;
    pollfd.revents = 0;
    
    int index = findFDIndex(fd);
    if (index != -1) {
        _fds[index] = pollfd;
        return;   
    }
    _fds.push_back(pollfd);
}

void Multiplexer::removeFd(int fd)
{
    int index = findFDIndex(fd);
    if (index != -1)
        _fds.erase(_fds.begin() + index);
}

int Multiplexer::wait(int timeout)
{
    return poll(_fds.data(), _fds.size(), timeout);
}

bool Multiplexer::canRead(int fd) const
{
    int index = findFDIndex(fd);
    if (index == -1) {
        return false;
    }
    return _fds[index].revents & POLLIN;
}

bool Multiplexer::canWrite(int fd) const
{
    int index = findFDIndex(fd);
    if (index == -1)
        return false;
    return _fds[index].revents & POLLOUT;
}

int Multiplexer::findFDIndex(int fd) const
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        if (_fds[i].fd == fd)
            return i;
    }
    return -1;
}