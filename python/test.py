
import time
import asyncio

def fun():
    print("hello, world")

loop = asyncio.get_event_loop()
loop.call_later(0, fun)
time.sleep(5)
print("hi")
loop.run_forever()
