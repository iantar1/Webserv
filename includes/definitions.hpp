/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:30:14 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/21 20:15:27 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define PORT 8080

#define RESPONSE_STATUS 0
#define CONTENT_TYPE 1
#define CONTENT_LENGHT 2
#define DATE 3
#define SERVER 4
#define TRANSFER_ENCODING 5
#define LOCATION 6
#define SET_COOKIE 7
#define CACHE_CONTROL 8

#define OK 200
#define CREATED 201
#define NO_CONTENT 204
#define MOVED_PERMA 301
#define FOUND 302
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define CONFLICT 409
#define LARGE_REQ 413
#define LONG_URI_REQ 414
#define INTERNAL_ERR 500
#define NOT_IMPLEMENTED 501

#define NORMAL_POST 100
#define CHUNKED_POST 101
#define BOUNDRIES_POST 102
