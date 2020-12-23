
import asyncio

async def foo():
    print("foo")
    await asyncio.sleep(3)
    print("hello")

async def bar():
    await asyncio.sleep(2)
    print("bar")

loop = asyncio.get_event_loop()
loop.create_task(foo())
loop.create_task(bar())
asyncio.get_event_loop().run_forever()
