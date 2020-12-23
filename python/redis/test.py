
import redis
import sys 

try:
    # r =  redis.StrictRedis("sfu.kedacom.com", 6379, 0, "KedaRedis16", 5, 5)
    # r =  redis.StrictRedis("172.16.185.183", 6379, 0, "KedaRedis16", 5, 5)
    # r =  redis.StrictRedis("10.67.18.134", 6379, 0, "KedaRedis16", 5, 5)

    r =  redis.StrictRedis("172.16.179.120", 6379, 0, "KedaRedis16", 5, 5)
    # f = open("./test.lua", 'r')
    f = open("./checkworkers.lua", 'r')
    str = f.read()
    keys = ["1111", "aaa", "11"]
    sha = r.script_load(str)
    print(sha)
    rlt = r.eval(str, 0, *keys)
    print(rlt)
except Exception as identifier:
    print(identifier)

