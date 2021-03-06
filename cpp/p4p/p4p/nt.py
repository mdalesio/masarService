
import logging
_log = logging.getLogger(__name__)

import time
from operator import itemgetter
from .wrapper import Type, Value

class NTScalar(object):
    Value = Value
    @staticmethod
    def buildType(valtype, extra=[]):
        return Type(id="epics:nt/NTScalar:1.0",
                    spec=[
            ('value', valtype),
            ('alarm', ('s', None, [
                ('severity', 'i'),
                ('status', 'i'),
                ('message', 's'),
            ])),
            ('timeStamp', ('s', None, [
                ('secondsPastEpoch', 'l'),
                ('nanoseconds', 'i'),
                ('userTag', 'i'),
            ])),
        ]+extra)

    def __init__(self, valtype='d'):
        self.type = self.buildType(valtype)

    def wrap(self, value):
        if isinstance(value, dict):
            return self.Value(self.type, value)
        else:
            return self.Value(self.type, {
                'value': value,
                'timeStamp': {
                    'secondsPastEpoch': time.time(),
                },
            })

class NTMultiChannel(object):
    Value = Value
    @staticmethod
    def buildType(valtype, extra=[]):
        return Type(id="epics:nt/NTMultiChannel:1.0",
                    spec=[
            ('value', valtype),
            ('channelName', 'as'),
            ('descriptor', 's'),
            ('alarm', ('s', None, [
                ('severity', 'i'),
                ('status', 'i'),
                ('message', 's'),
            ])),
            ('timeStamp', ('s', None, [
                ('secondsPastEpoch', 'l'),
                ('nanoseconds', 'i'),
                ('userTag', 'i'),
            ])),
            ('severity', 'ai'),
            ('status', 'ai'),
            ('message', 'as'),
            ('secondsPastEpoch', 'al'),
            ('nanoseconds', 'ai'),
            ('userTag', 'ai'),
            ('isConnected', 'a?'),
        ]+extra)

class NTTable(object):
    Value = Value
    @staticmethod
    def buildType(columns=[], extra=[]):
        return Type(id="epics:nt/NTTable:1.0",
                    spec=[
            ('labels', 'as'),
            ('value', ('s', None, columns)),
            ('descriptor', 's'),
            ('alarm', ('s', None, [
                ('severity', 'i'),
                ('status', 'i'),
                ('message', 's'),
            ])),
            ('timeStamp', ('s', None, [
                ('secondsPastEpoch', 'l'),
                ('nanoseconds', 'i'),
                ('userTag', 'i'),
            ])),
        ]+extra)

    def __init__(self, columns=[], extra=[]):
        self.labels = []
        C = []
        for col, type in columns:
            if type[0]=='a':
                raise ValueError("NTTable column types may not be array")
            C.append((col, 'a'+type))
            self.labels.append(col)
        self.type = self.buildType(C, extra=extra)

    def wrap(self, values):
        cols = dict([(L, []) for L in self.labels])
        try:
            # unzip list of dict
            for V in values:
                for L in self.labels:
                    try:
                        cols[L].append(V[L])
                    except (IndexError, KeyError):
                        pass
            # allow omit empty columns
            for L in self.labels:
                V = cols[L]
                if len(V)==0:
                    del cols[L]

            try:
                return Value(self.type, {
                    'labels': self.labels,
                    'value': cols,
                })
            except:
                _log.error("Failed to encode '%s' with %s", cols, self.labels)
                raise
        except:
            if hasattr(values[0], 'keys'):
                _log.error("Columns")
            _log.exception("Failed to wrap: %s", values)
            raise
