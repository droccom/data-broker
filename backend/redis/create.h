/*
 * Copyright © 2018,2019 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef BACKEND_REDIS_CREATE_H_
#define BACKEND_REDIS_CREATE_H_

#include "transports/sr_buffer.h"
#include "request.h"

/*
 * convert a Redis request into a sequence of SGEs resembling the command string
 * returns the number of bytes placed into the buffer or negative error
 */
int dbBE_Redis_create_command_sge( dbBE_Redis_request_t *request,
                                   dbBE_Redis_sr_buffer_t * buf,
                                   dbBE_sge_t *cmd );

int dbBE_Redis_create_key( dbBE_Redis_request_t *request, char *keybuf, uint16_t size );

#endif /* BACKEND_REDIS_CREATE_H_ */
