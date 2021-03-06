from _conduit import _IPCClientConduit, _IPCServerConduit, _IPCAccumulatingClientConduit, Event
import numpy


class _ConduitMixin(object):
       
    def initialize(self):
        # register a pure python handler for the codec
        #self.register_callback_for_code(0, lambda e:self.handle_codec(self, e))
        
        # call the C++ initialize method
        self._initialize()

    def send_data(self, code, data):
        dtype = type(data)
        if dtype in (float, numpy.float32, numpy.float64):
            self.send_float(code, data)
        elif dtype in (int, numpy.int16, numpy.int32, numpy.int64):
            self.send_integer(code, data)
        else:
            self.send_object(code, data)

    #def handle_codec_(self, evt):
    #    self.reverse_codec = {}
    #    self.codec = {}
    #
    #    codec_datum = evt.value()
    #    
    #    for k in codec_datum.keys():
    #        name = codec_datum[k]["tagname"]
    #        self.codec[k] = name 
    #        self.reverse_codec[name] = k


class IPCClientConduit(_ConduitMixin, _IPCClientConduit):
    pass


class IPCServerConduit(_ConduitMixin, _IPCServerConduit):
    pass


class IPCAccumClientConduit(_ConduitMixin, _IPCAccumulatingClientConduit):
    pass

Event.__module__ = __name__  # So help() thinks Event is part of this module
Event.data = property(lambda self:
                          self._convert_mw_datum_to_python(self._mw_datum))
