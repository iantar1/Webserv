/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:46:38 by iantar            #+#    #+#             */
/*   Updated: 2024/02/29 15:02:06 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <fstream>
# include <sstream>
# include <string.h>
# include <vector>
# include <map>
# include <cstdlib>
# include <ctime>
# include <fcntl.h>
# include <sys/select.h>
# include <sys/stat.h>
# include <sys/epoll.h>

# include "macros.hpp"

# include "Server.hpp"
# include "Client.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "VirtualServer.hpp"

//# include "../src/parseRequest.hpp"
