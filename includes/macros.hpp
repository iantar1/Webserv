/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:47:43 by iantar            #+#    #+#             */
/*   Updated: 2024/03/13 14:48:26 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
 
# define POST 1
# define GET 2
# define DELETE 3 


// Reset
# define RESET   "\033[0m"         // Reset color to default
# define BUF_SIZE 1024
// Regular Colors
# define BLACK   "\033[0;30m"      // Black
# define RED     "\033[0;31m"      // Red
# define GREEN   "\033[0;32m"      // Green
# define YELLOW  "\033[0;33m"      // Yellow
# define BLUE    "\033[0;34m"      // Blue
# define MAGENTA "\033[0;35m"      // Magenta
# define CYAN    "\033[0;36m"      // Cyan
# define WHITE   "\033[0;37m"      // White

// Bold Colors
# define BOLDBLACK   "\033[1;30m"  // Bold Black
# define BOLDRED     "\033[1;31m"  // Bold Red
# define BOLDGREEN   "\033[1;32m"  // Bold Green
# define BOLDYELLOW  "\033[1;33m"  // Bold Yellow
# define BOLDBLUE    "\033[1;34m"  // Bold Blue
# define BOLDMAGENTA "\033[1;35m"  // Bold Magenta
# define BOLDCYAN    "\033[1;36m"  // Bold Cyan
# define BOLDWHITE   "\033[1;37m"  // Bold White


# define RESPONSE_STATUS 0
# define CONTENT_TYPE 1
# define CONTENT_LENGHT 2
# define DATE 3
# define SERVER 4
# define TRANSFER_ENCODING 5
# define LOCATION 6
# define SET_COOKIE 7
# define CACHE_CONTROL 8

# define OK 200
# define CREATED 201
# define NO_CONTENT 204
# define MOVED_PERMA 301
# define FOUND 302
# define UNAUTHORIZED 401
# define FORBIDDEN 403 // !return in delete
# define METHOD_NOT_ALLOWED 405
# define CONFLICT 409
# define NOT_FOUND 404
# define INTERNAL_ERR 500

# define NORMAL_POST 100
# define CHUNKED_POST 101
# define BOUNDRIES_POST 102

# define NORMAL 1000
# define CHUNKED 1001

// Request macros

# define NOT_IMPLEMENTED 501
# define BAD_REQUEST 400
# define LONG_URI_REQ 414
# define LARGE_REQ 413
# define MAX_URI_CHAR_SIZE 2048


// Delete Response Codes

# define  ACCEPTED 202

// ! delete method

# define SUCCESS 0
# define PERMISSION_DENIED 1
# define ERROR 2