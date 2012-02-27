from mworks.conduit import *
import time
from nose.tools import assert_equals

server = IPCServerConduit("test_resource")
client = IPCClientConduit("test_resource")

# define some dummy functions
target_value = {}

def set_target_value(evt):
    global target_value
    target_value = evt.data


server.initialize()
client.initialize()

# have the server and client register different codes
# for the event "test"
server.register_local_event_code(4, "test")
time.sleep(0.2)

client.register_local_event_code(6, "test")
time.sleep(0.2)


server.register_callback_for_name("test", set_target_value)
time.sleep(0.2)


def send_and_compare(client, test_value):
    global target_value
    
    client.send_data(6, test_value)
    time.sleep(0.2)
    assert_equals(test_value, target_value)

send_and_compare(client, 42)

send_and_compare(client, 42.2)

send_and_compare(client, 'blah')

send_and_compare(client, [1,2,3])

send_and_compare(client, {'a':3})

send_and_compare(client, {'a': 3, 'b': {'c': 4}})


server.finalize()
client.finalize()
