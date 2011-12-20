'''
Created on Dec 16, 2011

@author: shengb
'''
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals
from future_builtins import *

import sys
import sqlite3
import datetime as dt

from utils import checkConnection
from serviceconfig import (retrieveServiceConfigs)

def saveServiceEvent(conn, servicename, serviceconfigname, comment=None):
    """
    save an event config, and associate this event with given service and service config.
    
    >>> import sqlite3
    >>> from service import (saveService, retrieveServices)
    >>> from serviceconfig import (saveServiceConfig)
    >>> conn = sqlite3.connect(":memory:")
    >>> cur = conn.cursor()
    >>> SQL = '''CREATE TABLE "service" (
    ...        "service_id" INTEGER, 
    ...        "service_name" varchar(50) DEFAULT NULL, 
    ...        "service_desc" varchar(255) DEFAULT NULL, 
    ...        PRIMARY KEY ("service_id"));
    ...        CREATE TABLE "service_config" (
    ...        "service_config_id" INTEGER ,
    ...        "service_id" int(11) NOT NULL DEFAULT '0',
    ...        "service_config_name" varchar(50) DEFAULT NULL,
    ...        "service_config_desc" varchar(255) DEFAULT NULL,
    ...        "service_config_version" int(11) DEFAULT NULL,
    ...        "service_config_create_date" timestamp NOT NULL ,
    ...        PRIMARY KEY ("service_config_id")
    ...        CONSTRAINT "Ref_197" FOREIGN KEY ("service_id") REFERENCES "service" ("service_id") ON DELETE NO ACTION ON UPDATE NO ACTION
    ...        );
    ...        CREATE TABLE "service_event" (
    ...        "service_event_id" INTEGER ,
    ...        "service_config_id" int(11) NOT NULL DEFAULT '0',
    ...        "service_event_user_tag" varchar(255) DEFAULT NULL,
    ...        "service_event_UTC_time" timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    ...        "service_event_serial_tag" varchar(50) DEFAULT NULL,
    ...        PRIMARY KEY ("service_event_id")
    ...        CONSTRAINT "Ref_08" FOREIGN KEY ("service_config_id") REFERENCES "service_config" ("service_config_id") ON DELETE NO ACTION ON UPDATE NO ACTION
    ...        );'''
    >>> result = cur.executescript(SQL)
    >>> saveService(conn, 'masar1', desc='non-empty description')
    1
    >>> saveService(conn, 'masar2', desc='non-empty description')
    2
    >>> saveServiceConfig(conn, 'masar1', 'orbit C01', 'BPM horizontal readout for storage ring')
    1
    >>> saveServiceConfig(conn, 'masar1', 'orbit C02', 'BPM horizontal readout for storage ring')
    2
    >>> saveServiceConfig(conn, 'masar2', 'orbit C01', 'BPM horizontal readout for storage ring')
    3
    >>> saveServiceConfig(conn, 'masar2', 'orbit C02', 'BPM horizontal readout for storage ring')
    4
    >>> result = retrieveServiceConfigs(conn, servicename='masar1', serviceconfigname='orbit C01')
    >>> saveServiceEvent(conn, servicename='masar1', serviceconfigname='orbit C01', comment='a service event')
    1
    >>> conn.close()
    """
    if servicename is None or serviceconfigname is None:
        raise Exception('service or service config is empty. Can not associate the event with service and its config.')
    checkConnection(conn)
    if serviceconfigname is None:
        raise Exception("service config name is not specified for this event.")
    
    serviceconfigid = retrieveServiceConfigs(conn, servicename=servicename, serviceconfigname=serviceconfigname)
    if len(serviceconfigid) > 0:
        serviceconfigid = serviceconfigid[0][0]
    else:
        raise Exception('Can not find service config (%s) with service (%s)' %(serviceconfigname, servicename))
    sql = '''
    insert into service_event(service_event_id, service_config_id, service_event_user_tag, service_event_UTC_time, service_event_serial_tag)
    values (?, ?, ?, datetime('now'), ?)
    '''
    try:
        cur = conn.cursor()
        cur.execute(sql, (None, serviceconfigid, comment, None))
    except sqlite3.Error, e:
        print ('Error %s' %e.args[0])
        raise
    return cur.lastrowid


#def insertServiceEvent(connx,serviceConfigId,comment):
#    cursor = conn.cursor() 
#    SQLgetLastSerial = '''select service_event_serial_tag from service_event 
#        where service_config_id = '%s' 
#        order by service_event_serial_tag desc limit 1 '''
#    SQL = SQLgetLastSerial % serviceConfigId
#    print ("insertServiceEvent2", SQL)
#    cursor.execute(SQL)
#    if cursor.rowcount > 0: #first
#        nextSerialTag = cursor.fetchone()[0] + 1
#    else:
#        nextSerialTag = 1
#        print ("using default serial tag = 1")
#    print ("nextSerialTag",nextSerialTag)
#    SQLinsertServiceEvent = '''insert into service_event 
#        (service_config_id, service_event_user_tag,service_event_serial_tag) 
#        values (%s, '%s', %s)  '''
#    SQL = SQLinsertServiceEvent % (serviceConfigId, comment, nextSerialTag)
#    print (SQL)
#    try:
#        cursor.execute(SQL)
#        print ("success")
#    except Exception:
#        #?# need to get specific information on the exception.
#        print ('insertServiceEvent2 failed %s (%s)' % (Exception.message, type(Exception)))
#        return(-1,-1)
#    serviceEventId = cursor.lastrowid
#    return (serviceEventId,nextSerialTag)

def retrieveServiceEvents(conn, start=None, end=None, comment=None):
    """
    retrieve an service event with given user tag within given time frame.
    Both start and end time should be in UTC time format.
    If end time is not specified, use current time. If start is not specified, use one week before end time.
    It return a tuple array with format like:
    [(service_event_id, service_config_id, service_event_user_tag, service_event_UTC_time, service_event_serial_tag)]
    
    >>> import sqlite3
    >>> from service import (saveService, retrieveServices)
    >>> from serviceconfig import (saveServiceConfig)
    >>> conn = sqlite3.connect(":memory:")
    >>> cur = conn.cursor()
    >>> SQL = '''CREATE TABLE "service" (
    ...        "service_id" INTEGER, 
    ...        "service_name" varchar(50) DEFAULT NULL, 
    ...        "service_desc" varchar(255) DEFAULT NULL, 
    ...        PRIMARY KEY ("service_id"));
    ...        CREATE TABLE "service_config" (
    ...        "service_config_id" INTEGER ,
    ...        "service_id" int(11) NOT NULL DEFAULT '0',
    ...        "service_config_name" varchar(50) DEFAULT NULL,
    ...        "service_config_desc" varchar(255) DEFAULT NULL,
    ...        "service_config_version" int(11) DEFAULT NULL,
    ...        "service_config_create_date" timestamp NOT NULL ,
    ...        PRIMARY KEY ("service_config_id")
    ...        CONSTRAINT "Ref_197" FOREIGN KEY ("service_id") REFERENCES "service" ("service_id") ON DELETE NO ACTION ON UPDATE NO ACTION
    ...        );
    ...        CREATE TABLE "service_event" (
    ...        "service_event_id" INTEGER ,
    ...        "service_config_id" int(11) NOT NULL DEFAULT '0',
    ...        "service_event_user_tag" varchar(255) DEFAULT NULL,
    ...        "service_event_UTC_time" timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    ...        "service_event_serial_tag" varchar(50) DEFAULT NULL,
    ...        PRIMARY KEY ("service_event_id")
    ...        CONSTRAINT "Ref_08" FOREIGN KEY ("service_config_id") REFERENCES "service_config" ("service_config_id") ON DELETE NO ACTION ON UPDATE NO ACTION
    ...        );'''
    >>> result = cur.executescript(SQL)
    >>> saveService(conn, 'masar1', desc='non-empty description')
    1
    >>> saveService(conn, 'masar2', desc='non-empty description')
    2
    >>> saveServiceConfig(conn, 'masar1', 'orbit C01', 'BPM horizontal readout for storage ring')
    1
    >>> saveServiceConfig(conn, 'masar1', 'orbit C02', 'BPM horizontal readout for storage ring')
    2
    >>> saveServiceConfig(conn, 'masar2', 'orbit C01', 'BPM horizontal readout for storage ring')
    3
    >>> saveServiceConfig(conn, 'masar2', 'orbit C02', 'BPM horizontal readout for storage ring')
    4
    >>> result = retrieveServiceConfigs(conn, servicename='masar1', serviceconfigname='orbit C01')
    >>> import datetime as dt
    >>> saveServiceEvent(conn, servicename='masar1', serviceconfigname='orbit C01', comment='a service event1')
    1
    >>> start = dt.datetime.utcnow()
    >>> import time
    >>> time.sleep(1)
    >>> saveServiceEvent(conn, servicename='masar1', serviceconfigname='orbit C01', comment='a service event2')
    2
    >>> end = dt.datetime.utcnow()
    >>> time.sleep(1)
    >>> saveServiceEvent(conn, servicename='masar1', serviceconfigname='orbit C01', comment='a service event3')
    3
    >>> results = retrieveServiceEvents(conn, comment='a service event1')
    >>> for result in results:
    ...    print (result[0], result[1], result[2])
    1 1 a service event1
    >>> results = retrieveServiceEvents(conn)
    >>> for result in results:
    ...    print (result[0], result[1], result[2])
    1 1 a service event1
    2 1 a service event2
    3 1 a service event3
    >>> results = retrieveServiceEvents(conn, start=start, end=end)
    >>> for result in results:
    ...    print (result[0], result[1], result[2])
    2 1 a service event2
    >>> conn.close()
    """
    checkConnection(conn)
    results = None
    try:
        cur = conn.cursor()
        sql = '''
        select service_event_id, service_config_id, service_event_user_tag, service_event_UTC_time, service_event_serial_tag
        from service_event where service_event_UTC_time > ? and service_event_UTC_time < ?  
        '''
        
        if end is None:
            end = dt.datetime.utcnow()
        if start is None:
            start = end - dt.timedelta(weeks=1)
        
        if start > end:
            raise Exception('Time range error')
        
        if comment is None:
            cur.execute(sql, (start, end,))
        else:
            sql += ' and service_event_user_tag like ?'
            cur.execute(sql, (start, end, comment,))
        results = cur.fetchall()
    except sqlite3.Error, e:
        print ("Error %s" %e.args[0])
        raise
    
    return results

if __name__ == '__main__':
    import doctest
    doctest.testmod()