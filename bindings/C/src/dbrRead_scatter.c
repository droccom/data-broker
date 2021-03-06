/*
 * Copyright © 2018, 2019 IBM Corporation
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

#include "errorcodes.h"
#include "libdbrAPI.h"
#include "libdatabroker_ext.h"

#include <stdlib.h>
#include <string.h>

DBR_Errorcode_t
dbrRead_scatter( DBR_Handle_t dbr_handle,
                 void *va_ptr[],
                 size_t size[],
                 int len,
                 DBR_Tuple_name_t tuple_name,
                 DBR_Tuple_template_t match_template,
                 DBR_Group_t group,
                 int flags )
{
  if(( len <= 0 ) || (va_ptr == NULL) || (size==NULL))
    return DBR_ERR_INVALID;


  dbrDA_Request_chain_t *req = (dbrDA_Request_chain_t*)calloc( 1, sizeof( dbrDA_Request_chain_t ) + len * sizeof( dbBE_sge_t ) );
  if( req == NULL )
    return DBR_ERR_NOMEMORY;

  req->_key = tuple_name;
  req->_next = NULL;
  req->_size = 0;
  req->_sge_count = len;
  int n;
  for( n=0; n<len; ++n )
  {
    req->_value_sge[ n ].iov_base = (void*)va_ptr[ n ];
    req->_value_sge[ n ].iov_len = size[ n ];
    req->_size += size[ n ];
  }

  int64_t outsize = 0;
  DBR_Errorcode_t rc = libdbrRead( dbr_handle,
                                   req,
                                   &outsize,
                                   match_template,
                                   group,
                                   (flags & DBR_FLAGS_NOWAIT) ? 0 : 1 );

  free( req );
  return rc;

}


DBR_Errorcode_t
dbrRead_v( DBR_Handle_t dbr_handle,
           struct iovec *sge,
           const int len,
           DBR_Tuple_name_t tuple_name,
           DBR_Tuple_template_t match_template,
           DBR_Group_t group,
           int flags )
{
  if(( len <= 0 ) || ( sge == NULL ))
    return DBR_ERR_INVALID;

  dbrDA_Request_chain_t *req = (dbrDA_Request_chain_t*)calloc( 1, sizeof( dbrDA_Request_chain_t ) + len * sizeof( struct iovec ));
  req->_next = NULL;
  req->_key = tuple_name;
  req->_sge_count = len;
  memcpy( req->_value_sge, sge, len * sizeof( struct iovec ) );

  int n;
  req->_size = 0;
  for( n=0; n<len; ++n )
    req->_size += sge[n].iov_len;

  int64_t outsize = 0;
  DBR_Errorcode_t rc = libdbrRead( dbr_handle,
                                   req,
                                   &outsize,
                                   match_template,
                                   group,
                                   (flags & DBR_FLAGS_NOWAIT) ? 0 : 1 );
  free( req );
  return rc;
}
