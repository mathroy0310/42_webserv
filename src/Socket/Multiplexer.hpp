/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:46:27 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/27 17:42:38 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.h"

#include <vector>
#include <poll.h>

class Multiplexer {
public:
    // Constructors & Destructors
    Multiplexer(void);
    ~Multiplexer(void);

    // Methods
    void addFd(int fd, short events);
    void removeFd(int fd);
    int wait(int timeout = 2500);
    bool canRead(int fd) const;
    bool canWrite(int fd) const;
private:
    // Attributes
    std::vector<struct pollfd> _fds;
    
    // Methods
    int findFDIndex(int fd) const;
};
