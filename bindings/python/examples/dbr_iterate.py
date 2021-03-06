 #
 # Copyright (C) 2018, 2019 IBM Corporation
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #    http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
import os
import sys

from _dbr_interface import ffi
from dbr_module import dbr

dbr_name = "DBRtestname"
level = dbr.DBR_PERST_VOLATILE_SIMPLE
group_list = ffi.new('DBR_GroupList_t')
dbr_hdl = ffi.new('DBR_Handle_t*')
dbr_hdl = dbr.create(dbr_name, level, group_list)
group = dbr.DBR_GROUP_EMPTY

# query the DB to see if successful
dbr_state = ffi.new('DBR_State_t*')
res = dbr.query(dbr_hdl, dbr_state, dbr.DBR_STATE_MASK_ALL)

# put a set of keys
for i in range(10):
    key = "simple_key_"+str(i)
    print('Putting key: ' + key)
    res = dbr.put(dbr_hdl, "test-value-"+str(i), key,  dbr.DBR_GROUP_EMPTY)

iterator = dbr.DBR_ITERATOR_NEW
key, iterator = dbr.iterator(dbr_hdl, iterator, dbr.DBR_GROUP_EMPTY, "")

while iterator != dbr.DBR_ITERATOR_DONE:
    value, res = dbr.get(dbr_hdl, key, "", dbr.DBR_GROUP_EMPTY, dbr.DBR_FLAGS_NONE)
    print('On key ' + key + ' Get returned: ' + value)
    key, iterator = dbr.iterator(dbr_hdl, iterator, dbr.DBR_GROUP_EMPTY, "")


print('Delete Data Broker')
res = dbr.delete(dbr_name)
print('Exit Status: ' + dbr.getErrorMessage(res))
