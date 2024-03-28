/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:46:58 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/28 13:07:40 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplexer.hpp"

Multiplexer::Multiplexer(void) {}

Multiplexer::~Multiplexer(void) {}

void Multiplexer::addFd(int fd, short events)
{
    struct pollfd pollfd;
    pollfd.fd = fd;
    pollfd.events = events;
    pollfd.revents = 0;
    
    int index = this->findFDIndex(fd);
    if (index != -1) {
        this->_fds[index] = pollfd;
        return;   
    }
    this->_fds.push_back(pollfd);
}

void Multiplexer::removeFd(int fd)
{
    int index = this->findFDIndex(fd);
    if (index != -1)
        this->_fds.erase(this->_fds.begin() + index);
}

int Multiplexer::wait(int timeout)
{
    return (poll(this->_fds.data(), this->_fds.size(), timeout));
}

bool Multiplexer::canRead(int fd) const
{
    int index = findFDIndex(fd);
    if (index == -1)
        return false;
    return (this->_fds[index].revents & POLLIN);
}

bool Multiplexer::canWrite(int fd) const
{
    int index = this->findFDIndex(fd);
    if (index == -1)
        return false;
    return (this->_fds[index].revents & POLLOUT);
}

int Multiplexer::findFDIndex(int fd) const
{
    for (size_t i = 0; i < this->_fds.size(); i++)
        if (this->_fds[i].fd == fd)
        	return (i);
    return (-1);
}
